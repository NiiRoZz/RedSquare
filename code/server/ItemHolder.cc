#include "ItemHolder.h"

namespace redsquare
{
    ItemHolder::ItemHolder(gf::Id entityID, ItemType itemTypeHolding)
    : ServerEntity(entityID, EntityType::ItemHolder, EntitySubType::Unknow)
    , m_ItemTypeHolding(itemTypeHolding)
    {

    }

    ItemType ItemHolder::getItemTypeHolding() const
    {
        return m_ItemTypeHolding;
    }
}