#ifndef REDSQUARE_CLIENT_CLIENTENTITY_H
#define REDSQUARE_CLIENT_CLIENTENTITY_H

#include "../common/Entity.h"
#include "Inventory.h"

#include <gf/Entity.h>

namespace redsquare
{
    class ClientEntity: public gf::Entity, public redsquare::Entity
    {
    public:
        ClientEntity( gf::Id entityID, const EntityType entityType, const EntitySubType entitySubType );

        Inventory& getInventory();

    protected:
        Inventory m_Inventory;
    };
}

#endif