#ifndef REDSQUARE_SERVER_MONSTER_H
#define REDSQUARE_SERVER_MONSTER_H

#include "ServerEntity.h"
#include "World.h"
#include "../common/Packet.h"
#include <gf/Id.h>
#include <gf/Vector.h>
#include <gf/VectorOps.h>

namespace redsquare
{
    class Monster: public ServerEntity
    {
    public:
        Monster(gf::Id entityID);

        virtual void createCarPacket(Packet &packet) override;
        void monsterSpawn(std::map<gf::Id,Monster> &m_Monsters,World &world);
        bool nextToSpawn(gf::Vector2i playerSpawn, World &world);

        void drawRoutine(World &world);
        gf::Vector2i m_bait;
        gf::Vector2i m_Routine;
        bool checkRoutine();
        void attack(ServerEntity *target);
       

    private:
        
    };
}

#endif