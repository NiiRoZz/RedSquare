#ifndef REDSQUARE_GAME_FACTORY_H
#define REDSQUARE_GAME_FACTORY_H

#include <memory>

#include "GameInstance.h"

namespace redsquare
{
    class GameFactory
    {
    public:
        virtual ~GameFactory();
        virtual std::unique_ptr<GameInstance> createInstance() = 0;
    };
}

#endif // REDSQUARE_GAME_FACTORY_H
