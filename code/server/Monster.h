#ifndef REDSQUARE_SERVER_MONSTER_H
#define REDSQUARE_SERVER_MONSTER_H

#include "../common/Entity.h"

namespace redsquare
{
    class Monster: public Entity
    {
    public:
        Monster();

        void playTurn();
    };
}

#endif