#include "ServerItem.h"

namespace redsquare
{
    ServerItem::ServerItem()
    : Item(ItemType::Unknow, 1u)
    {

    }
    ServerItem::ServerItem(ItemType type, uint baseFloor)
    : Item(type, baseFloor)
    {
        
    }

    uint8_t ServerItem::getSlotMask() const
    {
        return m_SlotMask;
    }
}