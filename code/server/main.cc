#include "World.h"
#include "../common/Singletons.h"
#include "../common/Packet.h"
#include "Player.h"
#include "Game.h"

#include <iostream>

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
  	gf::SingletonStorage<gf::Random> storageForRandom(redsquare::gRandom);

    redsquare::Game game(port);

	//Maybe a way more perf friendly ?
	while( game.m_Players.size() != nmbPlayers )
	{

	}

	//Start the game
	while ( game.m_Players.size() > 0 )
	{
		for (auto it = game.m_Players.begin(); it != game.m_Players.end(); ++it)
		{
			//1: send to the player it's his turn
			redsquare::Packet packet;
			packet.type = redsquare::PacketType::PlayerTurn;
			packet.playerTurn.playerTurn = true;
			it->second.sendPacket( packet );

			//2: wait until his reply
			it->second.receivePacket( packet );

			//3: check if his action is possible, if it's not go to 1 and if it is just finish the turn
			bool actionMade = game.processPackets( packet );
			while ( !actionMade )
			{
				//3.1: send to the player it's his turn
				redsquare::Packet packet;
				packet.type = redsquare::PacketType::PlayerTurn;
				packet.playerTurn.playerTurn = true;
				it->second.sendPacket( packet );

				//3.2: wait until his reply
				it->second.receivePacket( packet );

				actionMade = game.processPackets( packet );
			}
		}
	}

	return 0;
}
