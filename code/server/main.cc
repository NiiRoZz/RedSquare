#include "../common/World.h"
#include "Player.h"
#include "Game.h"

#include <gf/Clock.h>
#include <iostream>

int main( int argc, char **argv )
{
	if ( argc != 2 )
	{
		std::cerr << "Usage : ./RedSquare-Server port" << std::endl;
		return 1;
	}

	int port = atoi( argv[1] );
	if ( port < 1024 )
	{
		std::cerr << "Port should be > 1024" << std::endl;
		return 2;
	}

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
