#include "Entity.h"

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
}