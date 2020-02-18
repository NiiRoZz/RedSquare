#ifndef REDSQUARE_SERVER_SERVERENTITY_H
#define REDSQUARE_SERVER_SERVERENTITY_H

#include "../common/Entity.h"
#include "../common/ProtocolData.h"
#include "../common/RedsquareProtocol.h"
#include "Inventory.h"
#include "ServerItem.h"

namespace redsquare
{
    class ServerEntity: public Entity
    {
    public:
        ServerEntity(gf::Id entityID, const EntityType entityType, const EntitySubType entitySubType);

        void createCarPacket(EntityCharacteristicData &characteristics);

        Inventory& getInventory();

        //Send update of the inventory to client.  pos is used only if slotType == Cargo
        RedsquareServerUpdateItem createUpdateItemPacket(InventorySlotType slotType, bool remove, uint pos = 0u);

        virtual void defaultInventoryStuff();

    protected:
        Inventory m_Inventory;
    };
}

#endif