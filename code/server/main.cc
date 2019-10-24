#include "World.h"
#include "../common/Singletons.h"
#include "../common/Packet.h"
#include "Player.h"
#include "Game.h"
#include <time.h>
#include <stdlib.h>

#include <iostream>
#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

using namespace redsquare;

int main( int argc, char **argv )
{
	if ( argc != 3 )
	{
		std::cerr << "Usage : ./RedSquare-Server port numberOfPlayers" << std::endl;
		return 1;
	}

	int port = atoi( argv[1] );
	if ( port < 1024 )
	{
		std::cerr << "Port should be > 1024" << std::endl;
		return 2;
	}

	int nmbPlayers = atoi( argv[2] );
	if ( nmbPlayers <= 0 )
	{
		std::cerr << "Number of players should be > 0" << std::endl;
		return 3;
	}

	srand(time(nullptr));

	// Init singleton
  	gf::SingletonStorage<gf::Random> storageForRandom(gRandom);

    Game game;

	game.addNewMonsters(10); // 10 monster TODO

	boost::asio::io_service m_IoService;
    boost::asio::ip::tcp::acceptor m_Acceptor(m_IoService, tcp::endpoint(tcp::v4(), port));

	while( game.m_Players.size() != nmbPlayers )
	{
		tcp::socket socket(m_IoService);

		m_Acceptor.accept(socket);

		SocketTcp wrapper(std::move(socket));

		game.addNewPlayer(std::move(wrapper));
	}

	//Start the game and play until all players has disconnected
	while ( game.m_Players.size() > 0 )
	{
		for (auto it = game.m_Players.begin(); it != game.m_Players.end(); ++it)
		{
			//1: send to the player it's his turn
			it->second.m_PointInRound = 2;
			it->second.m_AttackedInRound = false;

			while ( it->second.m_PointInRound > 0 )
			{
				//1: send to the player it's his turn
				Packet packet;
				packet.type = PacketType::PlayerTurn;
				packet.playerTurn.playerTurn = true;
				it->second.sendPacket( packet );

				//Detect if the connection has been closed
				if ( it->second.playerDisconnected() )
				{
					gf::Id disconnectedID = it->first;

					game.m_Players.erase(it--);

					Packet sendPacket;
					sendPacket.type = PacketType::EntityDisconnected;
					sendPacket.entityDisconnected.typeEntity = EntityType::Player;
                	sendPacket.entityDisconnected.entityID = disconnectedID;

					game.sendPacketToAllPlayers( sendPacket );

					continue;
				}

				//2: wait until his reply
				it->second.receivePacket( packet );

				//Detect if the connection has been closed
				if ( it->second.playerDisconnected() )
				{
					gf::Id disconnectedID = it->first;

					game.m_Players.erase(it--);

					Packet sendPacket;
					sendPacket.type = PacketType::EntityDisconnected;
					sendPacket.entityDisconnected.typeEntity = EntityType::Player;
                	sendPacket.entityDisconnected.entityID = disconnectedID;

					game.sendPacketToAllPlayers( sendPacket );

					continue;
				}

				game.processPackets( packet );
			}
		}

		for (auto it = game.m_Monsters.begin(); it != game.m_Monsters.end(); ++it)
		{
			if(it->second.checkRoutine())
			{
            	it->second.drawRoutine(game.m_World);
			}
			else
			{
				std::vector<gf::Vector2i> points = game.m_World.m_SquareWorld.computeRoute(it->second.m_Pos, it->second.m_Routine, 0.0);
				it->second.m_Pos = points[1];

				Packet sendPacket;
				sendPacket.type = PacketType::ReceiveMove;
				sendPacket.receiveMove.entityID = it->second.m_EntityID;
				sendPacket.receiveMove.typeEntity = EntityType::Monster;
				sendPacket.receiveMove.posX = it->second.m_Pos[0];
				sendPacket.receiveMove.posY = it->second.m_Pos[1];

				game.sendPacketToAllPlayers( sendPacket );
			}
		}
	}

	return 0;
}
