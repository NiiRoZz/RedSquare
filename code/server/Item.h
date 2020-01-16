#ifndef REDSQUARE_SERVER_ITEM_H
#define REDSQUARE_SERVER_ITEM_H

#include "../common/Packet.h"

namespace redsquare
{
    class Item
    {
    public:
        Item(ItemType type);

        ItemType getType() const;

    private:
        ItemType m_Type;
    };
}

#endif