#include "Entity.h"

namespace redsquare
{
    Entity::Entity()
    : m_TypeOfEntity(0)
    , m_EntityID(0)
    {
        
    }

    Entity::Entity(gf::Id entityID, const uint8_t typeOfEntity)
    : m_EntityID(entityID)
    , m_TypeOfEntity(typeOfEntity)
    {
        
    }
}