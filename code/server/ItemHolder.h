#ifndef REDSQUARE_SERVER_ITEMHOLDER_H
#define REDSQUARE_SERVER_ITEMHOLDER_H

#include "ServerEntity.h"

namespace redsquare
{
    class ItemHolder: public ServerEntity
    {
    public :
        ItemHolder(gf::Id entityID, ItemType itemTypeHolding);

        ItemType getItemTypeHolding() const;

    private:
        ItemType m_ItemTypeHolding;
    };
}

#endif //REDSQUARE_SERVER_ITEMHOLDER_H