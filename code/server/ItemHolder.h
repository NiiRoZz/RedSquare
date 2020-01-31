#ifndef REDSQUARE_SERVER_ITEMHOLDER_H
#define REDSQUARE_SERVER_ITEMHOLDER_H

#include "ServerEntity.h"

namespace redsquare
{
    class ItemHolder: public ServerEntity
    {
    public :
        ItemHolder(gf::Id entityID, ItemType itemTypeHolding, uint itemHoldingBaseFloor, gf::Vector2i pos);

        ItemType getItemTypeHolding() const;

        uint getItemHoldingBaseFloor() const;

    private:
        ItemType m_ItemTypeHolding;
        uint m_ItemHoldingBaseFloor;
    };
}

#endif //REDSQUARE_SERVER_ITEMHOLDER_H