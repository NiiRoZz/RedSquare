#ifndef REDSQUARE_CLIENT_ITEM_H
#define REDSQUARE_CLIENT_ITEM_H

#include "../common/Packet.h"

#include <gf/Texture.h>
#include <gf/Path.h>

namespace redsquare
{
    class Item
    {
    public:
        static gf::Texture& getTexture( ItemType type );

        Item(ItemType type);

        gf::Texture& getIcon() const;


    private:
        ItemType m_Type;
        gf::Texture &m_Icon;
    };
}

#endif