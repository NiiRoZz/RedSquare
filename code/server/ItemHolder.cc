#include "ItemHolder.h"

namespace redsquare
{
    ItemHolder::ItemHolder(gf::Id entityID, ItemType itemTypeHolding, uint itemHoldingBaseFloor, gf::Vector2i pos)
    : ServerEntity(entityID, EntityType::ItemHolder, EntitySubType::Unknow)
    , m_ItemTypeHolding(itemTypeHolding)
    , m_ItemHoldingBaseFloor(itemHoldingBaseFloor)
    {
        m_Pos = pos;
    }

    ItemType ItemHolder::getItemTypeHolding() const
    {
        return m_ItemTypeHolding;
    }

    uint ItemHolder::getItemHoldingBaseFloor() const
    {
        return m_ItemHoldingBaseFloor;
    }
}