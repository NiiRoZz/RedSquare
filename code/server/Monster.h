#ifndef REDSQUARE_SERVER_MONSTER_H
#define REDSQUARE_SERVER_MONSTER_H

#include "ServerEntity.h"
#include "../common/Packet.h"
#include <gf/Id.h>

namespace redsquare
{
    class Monster: public ServerEntity
    {
    public:
        const uint8_t m_TypeOfMonster;

        Monster(gf::Id monsterID);

        virtual void createCarPacket(Packet &packet) override;

        void playTurn();

    private:
        gf::Id m_MonsterID;
    };
}

#endif