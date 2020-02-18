#ifndef REDSQUARE_CLIENT_CLIENTITEM_H
#define REDSQUARE_CLIENT_CLIENTITEM_H

#include "../common/ProtocolData.h"
#include "../common/Item.h"

#include <gf/Texture.h>
#include <gf/Path.h>

namespace redsquare
{
    class ClientItem: public redsquare::Item
    {
    public:
        static gf::Texture& getTexture( ItemType type );

        ClientItem();
        ClientItem(ItemType type, uint8_t slotMask, uint baseFloor);

        gf::Texture& getIcon() const;

    private:
        gf::Texture &m_Icon;
    };
}

#endif