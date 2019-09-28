#include "World.h"
#include "../common/Singletons.h"
#include "../common/Packet.h"
#include "Player.h"
#include "Game.h"

#include <iostream>

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

	// Init singleton
  	gf::SingletonStorage<gf::Random> storageForRandom(gRandom);

    Game game(port);

	//Maybe a way more perf friendly ?
	while( game.m_Players.size() != nmbPlayers )
	{

	}

	//Start the game and play until all players has disconnected
	while ( game.m_Players.size() > 0 )
	{
		for (auto it = game.m_Players.begin(); it != game.m_Players.end(); ++it)
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
                sendPacket.type = PacketType::PlayerDisconnected;
                sendPacket.receiveMove.playerID = disconnectedID;

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
                sendPacket.type = PacketType::PlayerDisconnected;
                sendPacket.receiveMove.playerID = disconnectedID;

                game.sendPacketToAllPlayers( sendPacket );

				continue;
			}

			//3: check if his action is possible, if it's not go to 1 and if it is just finish the turn
			bool actionMade = game.processPackets( packet );
			while ( !actionMade )
			{
				//3.1: send to the player it's his turn
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
					sendPacket.type = PacketType::PlayerDisconnected;
					sendPacket.receiveMove.playerID = disconnectedID;

					game.sendPacketToAllPlayers( sendPacket );

					continue;
				}

				//3.2: wait until his reply
				it->second.receivePacket( packet );

				//Detect if the connection has been closed
				if ( it->second.playerDisconnected() )
				{
					gf::Id disconnectedID = it->first;

					game.m_Players.erase(it--);

					Packet sendPacket;
					sendPacket.type = PacketType::PlayerDisconnected;
					sendPacket.receiveMove.playerID = disconnectedID;

					game.sendPacketToAllPlayers( sendPacket );

					continue;
				}

				actionMade = game.processPackets( packet );
			}
		}
	}

	return 0;
}
