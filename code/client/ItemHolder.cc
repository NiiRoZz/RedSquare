#include "ItemHolder.h"

namespace redsquare
{
    ItemHolder::ItemHolder( gf::Id entityID, ItemType itemHoldingType, gf::Vector2i pos )
    : ClientEntity(entityID, EntityType::ItemHolder, EntitySubType::Unknow)
    {
        m_Pos = pos;
        m_EntityTexture = &(ClientItem::getTexture(itemHoldingType));
    }

}