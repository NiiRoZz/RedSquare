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
}