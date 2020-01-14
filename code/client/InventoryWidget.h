#ifndef REDSQUARE_CLIENT_INVENTORYWIDGET_H
#define REDSQUARE_CLIENT_INVENTORYWIDGET_H

#include <gf/Widgets.h>
#include <gf/Texture.h>

namespace redsquare
{
    class InventoryWidget: public gf::SpriteWidget
    {
    public:
        InventoryWidget();
        InventoryWidget(const gf::Texture &defaultTexture, const gf::Texture &selectedTexture, const gf::Texture &disabledTexture);

        bool currDragging;
    };
}

#endif