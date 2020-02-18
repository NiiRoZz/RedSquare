#ifndef REDSQUARE_SERVER_MONSTER_H
#define REDSQUARE_SERVER_MONSTER_H

#include "ServerEntity.h"
#include "../common/ProtocolData.h"
#include <gf/Id.h>
#include <gf/Vector.h>
#include <gf/VectorOps.h>

namespace redsquare
{
    class Monster: public ServerEntity
    {
    public:
        Monster(gf::Id entityID);

        gf::Vector2i m_bait;
        gf::Vector2i m_Routine;
        bool checkRoutine();
        void attack(ServerEntity *target);
        void levelUp(uint m_Floor);
        int Variance(int range);
        //void createSystemMessage(std::string message, std::string to,std::string name);
       

    private:
        
    };
}

#endif