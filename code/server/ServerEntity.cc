#include "ServerEntity.h"

namespace redsquare
{
    ServerEntity::ServerEntity(gf::Id entityID, const EntityType entityType, const EntitySubType entitySubType)
    : Entity(entityID,entityType,entitySubType)
    {

    }

    void ServerEntity::createCarPacket(Packet &packet)
    {

    }

    Packet&& ServerEntity::createUpdateItemPacket(InventorySlotType slotType, bool remove, uint pos)
    {
        Packet packet;
        packet.type = PacketType::UpdateItem;
        packet.updateItem.entityID = m_EntityID;
        packet.updateItem.entityType = m_EntityType;
        packet.updateItem.slotType = slotType;
        packet.updateItem.pos = pos;
        packet.updateItem.removeItem = remove;

        ServerItem* item = m_Inventory.getItem(slotType, pos);
        if (item != nullptr)
        {
            packet.updateItem.typeItem = item->getType();
            packet.updateItem.slotMask = item->getSlotMask();
            packet.updateItem.baseFloorItem = item->getBaseFloor();
        }

        return std::move(packet);
    }

    Inventory& ServerEntity::getInventory()
    {
        return m_Inventory;
    }

    void ServerEntity::defaultInventoryStuff()
    {

    }
}