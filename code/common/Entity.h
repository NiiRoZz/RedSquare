#ifndef REDSQUARE_COMMON_ENTITY_H
#define REDSQUARE_COMMON_ENTITY_H

#include <gf/Vector.h>
#include <gf/Id.h>

namespace redsquare
{
    class Entity
    {
    public:
        const gf::Id m_EntityID;
        const uint8_t m_TypeOfEntity;

        gf::Vector2i m_Pos;

        int m_LifePoint;
        int m_MaxLifePoint;

        int m_AttackPoint;
        int m_MaxAttackPoint;
        
        int m_DefensePoint;
        int m_MaxDefensePoint;
        
        int m_Range;

        int m_Level;

        Entity();
        Entity(gf::Id entityID, const uint8_t typeOfEntity);
    };
}

#endif