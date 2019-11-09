#include "Entity.h"

namespace redsquare
{
    Entity::Entity()
    : m_TypeOfEntity(EntityClass::Magus)
    , m_EntityID(0)
    {
        
    }

    Entity::Entity(gf::Id entityID)
    : m_EntityID(entityID)
    , m_TypeOfEntity(EntityClass::Magus)
    {
        
    }

    Entity::Entity(gf::Id entityID, const EntityClass typeOfEntity)
    : m_EntityID(entityID)
    , m_TypeOfEntity(typeOfEntity)
    {
        
    }
}