#include "ItemHolder.h"

namespace redsquare
{
    ItemHolder::ItemHolder(gf::Id entityID, ItemType itemTypeHolding, gf::Vector2i pos)
    : ServerEntity(entityID, EntityType::ItemHolder, EntitySubType::Unknow)
    , m_ItemTypeHolding(itemTypeHolding)
    {
        m_Pos = pos;
    }

    ItemType ItemHolder::getItemTypeHolding() const
    {
        return m_ItemTypeHolding;
    }
}