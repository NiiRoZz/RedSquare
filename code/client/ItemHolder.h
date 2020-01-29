#ifndef REDSQUARE_CLIENT_ITEMHOLDER_H
#define REDSQUARE_CLIENT_ITEMHOLDER_H

#include "ClientEntity.h"

namespace redsquare
{
    class ItemHolder: public ClientEntity
    {
    public:
        ItemHolder( gf::Id entityID, ItemType itemHoldingType, gf::Vector2i pos );

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
    };
}

#endif