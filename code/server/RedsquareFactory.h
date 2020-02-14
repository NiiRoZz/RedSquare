#ifndef REDSQUARE_REDSQUARE_FACTORY_H
#define REDSQUARE_REDSQUARE_FACTORY_H

#include "GameFactory.h"

namespace redsquare
{
    class RedsquareFactory : public GameFactory
    {
    public:
        std::unique_ptr<GameInstance> createInstance() override;
    };
}

#endif // REDSQUARE_REDSQUARE_FACTORY_H
