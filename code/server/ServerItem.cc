#include "ServerItem.h"

namespace redsquare
{
    ServerItem::ServerItem()
    : Item(ItemType::Unknow)
    {

    }
    ServerItem::ServerItem(ItemType type)
    : Item(type)
    {
        
    }

    uint8_t ServerItem::getSlotMask() const
    {
        return m_SlotMask;
    }
}