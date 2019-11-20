#include "Entity.h"

#include <gf/VectorOps.h>

namespace redsquare
{
    Entity::Entity()
    : m_TypeOfEntity(EntitySubType::Magus)
    , m_EntityID(0)
    , m_Size({1,1})
    {
        
    }

    Entity::Entity(gf::Id entityID)
    : m_EntityID(entityID)
    , m_TypeOfEntity(EntitySubType::Magus)
    , m_Size({1,1})
    {
        
    }

    Entity::Entity(gf::Id entityID, const EntitySubType typeOfEntity)
    : m_EntityID(entityID)
    , m_TypeOfEntity(typeOfEntity)
    , m_Size({1,1})
    {
        
    }

    bool Entity::isInsideMe(gf::Vector2i pos)
    {
        for(uint i = 0; i < m_Size[0]; ++i)
        {
            for(uint j = 0; j < m_Size[1]; ++j)
            {
                if (pos == gf::Vector2i({m_Pos[0] + (int)i, m_Pos[1] + (int)j}))
                {
                    return true;
                }
            }
        }

        return false;
    }
}