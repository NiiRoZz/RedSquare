#include "World.h"
#include "../common/Singletons.h"
#include "../common/Packet.h"
#include "Player.h"
#include "Game.h"
#include "Chat.h"
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
	if ( nmbPlayers <= 0 || nmbPlayers > 9  ) // number of player between 0 and 9 
	{
		std::cerr << "Number of players should be > 0 and < 9" << std::endl;
		return 3;
	}

	srand(time(nullptr));

	// Init singleton
  	gf::SingletonStorage<gf::Random> storageForRandom(gRandom);

    Game game;
	Chat myChat;
	
	
	boost::asio::io_service m_IoService;
    boost::asio::ip::tcp::acceptor m_Acceptor(m_IoService, tcp::endpoint(tcp::v4(), port));


	boost::asio::io_service m_IoServiceChat;
    boost::asio::ip::tcp::acceptor m_AcceptorChat(m_IoServiceChat, tcp::endpoint(tcp::v4(), port+1));

	while( game.m_Players.size() != nmbPlayers )
	{
		tcp::socket socket(m_IoService);
		m_Acceptor.accept(socket);

		tcp::socket socketChat(m_IoServiceChat);
		m_AcceptorChat.accept(socketChat);
		

		SocketTcp wrapper(std::move(socket));
		SocketTcp wrapperChat(std::move(socketChat));

		gf::Id idPlayer = game.addNewPlayer(std::move(wrapper));
		myChat.addPlayer(idPlayer,std::move(wrapperChat));
	}

	myChat.startChat();

	//Start the game and play until all players has disconnected
	while ( game.m_Players.size() > 0 )
	{	
		
		for (auto it = game.m_Players.begin(); it != game.m_Players.end(); ++it)
		{
			//1: send to the player it's his turn
			it->second.m_PointInRound = 2;
			it->second.m_MovedInRound = false;

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

					game.m_Players.erase(it++);

					Packet sendPacket;
					sendPacket.type = PacketType::EntityDisconnected;
					sendPacket.entityDisconnected.typeEntity = EntityType::Player;
                	sendPacket.entityDisconnected.entityID = disconnectedID;

					game.sendPacketToAllPlayers( sendPacket );

					break;
				}

				//2: wait until his reply
				it->second.receivePacket( packet );

				//Detect if the connection has been closed
				if ( it->second.playerDisconnected() )
				{
					gf::Id disconnectedID = it->first;

					game.m_Players.erase(it++);

					Packet sendPacket;
					sendPacket.type = PacketType::EntityDisconnected;
					sendPacket.entityDisconnected.typeEntity = EntityType::Player;
                	sendPacket.entityDisconnected.entityID = disconnectedID;

					game.sendPacketToAllPlayers( sendPacket );

					break;
				}

				game.processPackets( packet );
			}
		}

		for (auto it = game.m_Monsters.begin(); it != game.m_Monsters.end(); ++it)
		{
			bool hasFocus = false;
			bool isTarget = false;
			bool hasAttacked = false;
			game.m_World.m_SquareWorld.clearFieldOfVision();
			game.m_World.m_SquareWorld.computeLocalFieldOfVision(it->second.m_Pos,10);

			auto it2 = game.m_Players.begin();
			while ( it2 != game.m_Players.end() )
			{
				if( game.m_World.m_SquareWorld.isInFieldOfVision(it2->second.m_Pos) )
				{
					hasFocus = true;
					break;
				}
				++it2;
			}

			if( hasFocus )
			{
				if( game.canAttack(it->second, it2->second.m_Pos) )
				{
                    ServerEntity *targetServerEntity = dynamic_cast<ServerEntity*>(&it2->second);
					it->second.attack(targetServerEntity);

					hasAttacked = true;

					//Check if target is dead
					if ( it2->second.m_LifePoint <= 0 )
					{
						Packet sendPacket;
						sendPacket.type = PacketType::PlayerDead;
						it2->second.sendPacket(sendPacket);

						gf::Id disconnectedID = it2->first;

						game.m_Players.erase(it2++);

						sendPacket.type = PacketType::EntityDisconnected;
						sendPacket.entityDisconnected.typeEntity = EntityType::Player;
						sendPacket.entityDisconnected.entityID = it2->first;

						game.sendPacketToAllPlayers( sendPacket );

						hasAttacked = false;
						isTarget = false;
					}
					else
					{
						Packet sendPacket;
						it2->second.createCarPacket( sendPacket );

						game.sendPacketToAllPlayers( sendPacket );
					}
				}
				else
				{
					it->second.m_Routine = it2->second.m_Pos;
					isTarget = true;
				}
			}

			if( !hasAttacked )
			{
				if( it->second.checkRoutine() )
				{
					game.m_World.drawRoutine(it->second);
				}
				else
				{
					if( isTarget )
					{
						game.m_World.m_SquareWorld.setWalkable(it->second.m_Routine, true);
						game.m_World.m_SquareWorld.setTransparent(it->second.m_Routine, true);
					}

					if( !game.m_World.m_SquareWorld.isWalkable(it->second.m_Routine) )
					{
						game.m_World.drawRoutine(it->second);
					}
					
					game.m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(&(it->second)), true);
					game.m_World.m_SquareWorld.setTransparent(it->second.m_Pos, true);

					std::vector<gf::Vector2i> points = game.m_World.m_SquareWorld.computeRoute(it->second.m_Pos, it->second.m_Routine, 0.0);

					if (points.empty())
					{
						game.m_World.drawRoutine(it->second);
					}
					else
					{
						it->second.m_Pos = points[1];
						if( isTarget )
						{
							game.m_World.m_SquareWorld.setWalkable(it->second.m_Routine, false);
						}

						game.m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(&(it->second)), false);

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
		}
	}
	return 0;
}
