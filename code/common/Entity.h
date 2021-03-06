#ifndef REDSQUARE_COMMON_ENTITY_H
#define REDSQUARE_COMMON_ENTITY_H

#include "Protocol.h"

#include <gf/Vector.h>
#include <gf/Id.h>

namespace redsquare
{
    class Entity
    {
    public:
        EntitySubType m_EntitySubType;

        gf::Vector2i m_Pos;
        gf::Vector2u m_Size;

        int m_LifePoint;
        int m_MaxLifePoint;

        int m_AttackPoint;
        
        int m_DefensePoint;
        
        int m_Range;

        int m_Level;

        uint m_RowCargoSlotNmb;
        uint m_ColumnCargoSlotNmb;

        Entity( gf::Id entityID, const EntityType entityType, const EntitySubType entitySubType );

        bool isInsideMe(gf::Vector2i pos);
        bool isInsideMe(Entity &entity);

        gf::Id getEntityID() const;

        EntityType getEntityType() const;

    protected:

        gf::Id m_EntityID;
        EntityType m_EntityType;
    };
}

#endif