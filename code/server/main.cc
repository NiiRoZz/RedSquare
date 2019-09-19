#include "../common/World.h"
#include "Square.h"
#include "Game.h"

#include <gf/Clock.h>
#include <iostream>

int main()
{
    redsquare::Game game(6025);

	std::cout << "Hello World!" << std::endl;

	gf::Clock clock;
	for(;;)
	{
		// 1. update
		gf::Time time = clock.restart();
		game.doUpdate( time );
	}

	return 0;
}
