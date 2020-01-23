#ifndef REDSQUARE_SERVER_SERVERENTITY_H
#define REDSQUARE_SERVER_SERVERENTITY_H

#include "../common/Entity.h"
#include "../common/Packet.h"
#include "Inventory.h"

namespace redsquare
{
    class ServerEntity: public Entity
    {
    public:
        ServerEntity(gf::Id entityID, const EntityType entityType, const EntitySubType entitySubType);

        virtual void createCarPacket(Packet &packet);

        Inventory& getInventory();

        //Send update of the inventory to client.  pos is used only if slotType == Cargo
        Packet&& createUpdateItemPacket(InventorySlotType slotType, bool remove, uint pos = 0u);

        virtual void defaultInventoryStuff();

    protected:
        Inventory m_Inventory;
    };
}

#endif