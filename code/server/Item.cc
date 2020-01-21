#include "Item.h"

namespace redsquare
{
    Item::Item(ItemType type)
    : m_Type(type)
    {
        
    }

    ItemType Item::getType() const
    {
        return m_Type;
    }
}