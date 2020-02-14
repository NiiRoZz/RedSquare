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

    void ServerEntity::createCarPacket(EntityCharacteristicData &characteristics)
    {
        characteristics.id = getEntityID();
        characteristics.entityType = getEntityType();

        characteristics.m_LifePoint = m_LifePoint;
        characteristics.m_MaxLifePoint = m_MaxLifePoint;

        characteristics.m_AttackPoint = m_AttackPoint;
        characteristics.m_DefensePoint = m_DefensePoint;

        characteristics.m_MaxAttackPoint = m_MaxAttackPoint;
        characteristics.m_MaxDefensePoint = m_MaxDefensePoint;

        characteristics.m_Range = m_Range;
        characteristics.m_Level = m_Level;

        if (getEntityType() == EntityType::Player)
        {
            Player *player = static_cast<Player*>(this);
            if (player != nullptr)
            {
                characteristics.m_ManaPoint = player->m_ManaPoint;
                characteristics.m_MaxManaPoint = player->m_MaxManaPoint;
                characteristics.m_XP = player->m_XP;
                characteristics.m_MaxXP = player->m_MaxXP;
                characteristics.m_Spells = player->m_SpellTab;
            }
        }
    }

    RedsquareServerUpdateItem ServerEntity::createUpdateItemPacket(InventorySlotType slotType, bool remove, uint pos)
    {
        RedsquareServerUpdateItem packet;
        packet.id = m_EntityID;
        packet.entityType = m_EntityType;
        packet.slotType = slotType;
        packet.pos = pos;
        packet.removeItem = remove;

        ServerItem* item = m_Inventory.getItem(slotType, pos);
        if (item != nullptr)
        {
            packet.typeItem = item->getType();
            packet.slotMask = item->getSlotMask();
            packet.baseFloorItem = item->getBaseFloor();
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
        gf::Log::debug("ServerEntity::onMovedItem 1\n");
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
            gf::Log::debug("ServerEntity::onMovedItem 2\n");
            m_MaxAttackPoint += item.m_GiveAttackPoint;
            m_AttackPoint += item.m_GiveAttackPoint;
            m_MaxDefensePoint += item.m_GiveDefensePoint;
            m_DefensePoint += item.m_GiveDefensePoint; 
            m_MaxLifePoint += item.m_GiveLifePoint;
            m_LifePoint += item.m_GiveLifePoint;

            gf::Log::debug("ServerEntity::onMovedItem 3\n");
            Player *player = static_cast<Player*>(this);
            gf::Log::debug("ServerEntity::onMovedItem 4\n");
            if (player != nullptr)
            {
                gf::Log::debug("ServerEntity::onMovedItem 5\n");
                player->m_MaxManaPoint += item.m_GiveManaPoint;
            }
        }

        gf::Log::debug("ServerEntity::onMovedItem 6\n");
        UpdateEntityCharacteristic message;
        message.entity = this;
        gf::Log::debug("ServerEntity::onMovedItem 7\n");
        gMessageManager().sendMessage(&message);
        gf::Log::debug("ServerEntity::onMovedItem 8\n");
    }
}