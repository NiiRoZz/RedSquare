#include "Item.h"
#include "../common/Singletons.h"

namespace redsquare
{
    gf::Texture& Item::getTexture( ItemType type )
    {
        switch (type)
        {
            case ItemType::Sword:
            {
                return gResourceManager().getTexture("img/attackCursor.png");
                break;
            }
        }
    }

    Item::Item(ItemType type)
    : m_Type(type)
    , m_Icon(Item::getTexture(type))
    {
        
    }

    gf::Texture& Item::getIcon() const
    {
        return m_Icon;
    }
}