#ifndef REDSQUARE_CLIENT_ITEMHOLDER_H
#define REDSQUARE_CLIENT_ITEMHOLDER_H

#include "ClientEntity.h"

#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>

namespace redsquare
{
    class ItemHolder: public ClientEntity
    {
    public:
        ItemHolder( gf::Id entityID, ItemType itemHoldingType, gf::Vector2i pos );

        void render(gf::RenderTarget& target, const gf::RenderStates& states);
    };
}

#endif