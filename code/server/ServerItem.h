#ifndef REDSQUARE_SERVER_SERVERITEM_H
#define REDSQUARE_SERVER_SERVERITEM_H

#include "../common/Item.h"

namespace redsquare
{
    class ServerItem: public redsquare::Item
    {
    public:
        ServerItem();
        ServerItem(ItemType type, uint baseFloor);

        uint8_t getSlotMask() const;
    };
}

#endif