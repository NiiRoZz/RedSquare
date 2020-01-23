#include "ClientEntity.h"

namespace redsquare
{
    ClientEntity::ClientEntity(gf::Id entityID, const EntityType entityType, const EntitySubType entitySubType)
    : redsquare::Entity(entityID,entityType,entitySubType)
    , m_Inventory(entityID)
    {

    }

    Inventory& ClientEntity::getInventory()
    {
        return m_Inventory;
    }
}