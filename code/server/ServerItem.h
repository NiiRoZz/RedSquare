#ifndef REDSQUARE_SERVER_SERVERITEM_H
#define REDSQUARE_SERVER_SERVERITEM_H

#include "../common/Item.h"

namespace redsquare
{
    class ServerItem: public redsquare::Item
    {
    public:
        ServerItem(ItemType type);

        uint8_t getSlotMask() const;
    };
}

#endif