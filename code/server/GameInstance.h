#ifndef REDSQUARE_GAME_INSTANCE_H
#define REDSQUARE_GAME_INSTANCE_H

#include "../common/ProtocolBytes.h"

#include "ServerGroup.h"

namespace redsquare
{
    class GameInstance : public ServerGroup
    {
    public:
        virtual void start() = 0;
        virtual bool isFinished() = 0;
    };
}

#endif // REDSQUARE_GAME_INSTANCE_H
