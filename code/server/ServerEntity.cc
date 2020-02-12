#include "ServerEntity.h"
#include "Player.h"
#include "Message.h"
#include "../common/Singletons.h"

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

    void ServerEntity::onMovedItem(ServerItem &item, bool remove)
    {
        if (remove)
        {
            m_MaxAttackPoint -= item.m_GiveAttackPoint;
            m_AttackPoint -= item.m_GiveAttackPoint;
            m_MaxDefensePoint -= item.m_GiveDefensePoint;
            m_DefensePoint -= item.m_GiveDefensePoint; 
            m_MaxLifePoint -= item.m_GiveLifePoint;
            m_LifePoint -= item.m_GiveLifePoint;

            Player *player = static_cast<Player*>(this);
            if (player != nullptr)
            {
                player->m_MaxManaPoint -= item.m_GiveManaPoint;
                if (player->m_ManaPoint > player->m_MaxManaPoint)
                {
                    player->m_ManaPoint = player->m_MaxManaPoint;
                }
            }
        }
        else
        {
            m_MaxAttackPoint += item.m_GiveAttackPoint;
            m_AttackPoint += item.m_GiveAttackPoint;
            m_MaxDefensePoint += item.m_GiveDefensePoint;
            m_DefensePoint += item.m_GiveDefensePoint; 
            m_MaxLifePoint += item.m_GiveLifePoint;
            m_LifePoint += item.m_GiveLifePoint;

            Player *player = static_cast<Player*>(this);
            if (player != nullptr)
            {
                player->m_MaxManaPoint += item.m_GiveManaPoint;
            }
        }

        UpdateEntityCharacteristic message;
        message.entity = this;
        gMessageManager().sendMessage(&message);
    }
}