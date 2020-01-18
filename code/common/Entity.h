#ifndef REDSQUARE_COMMON_ENTITY_H
#define REDSQUARE_COMMON_ENTITY_H

#include "Packet.h"

#include <gf/Vector.h>
#include <gf/Id.h>

namespace redsquare
{
    class Entity
    {
    public:
        gf::Id m_EntityID;
        EntitySubType m_TypeOfEntity;

        gf::Vector2i m_Pos;
        gf::Vector2u m_Size;

        int m_LifePoint;
        int m_MaxLifePoint;

        int m_AttackPoint;
        int m_MaxAttackPoint;
        
        int m_DefensePoint;
        int m_MaxDefensePoint;
        
        int m_Range;

        int m_Level;

        Entity();
        Entity(gf::Id entityID);
        Entity(gf::Id entityID, const EntitySubType typeOfEntity);

        bool isInsideMe(gf::Vector2i pos);
        bool isInsideMe(Entity &entity);
    };
}

#endif