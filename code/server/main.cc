#include "../common/World.h"
#include "../common/Singletons.h"
#include "Player.h"
#include "Game.h"

#include <gf/Clock.h>
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

	gf::Clock clock;
	for(;;)
	{
		// 1. update
		gf::Time time = clock.restart();
		game.doUpdate( time );
	}

	return 0;
}
