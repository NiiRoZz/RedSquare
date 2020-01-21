#include "InventoryWidget.h"

namespace redsquare
{
    InventoryWidget::InventoryWidget()
    : gf::SpriteWidget()
    , currDragging( false )
    {

    }

    InventoryWidget::InventoryWidget(const gf::Texture &texture)
    : gf::SpriteWidget( texture, texture, texture )
    , currDragging( false )
    {
        
    }

    InventoryWidget::InventoryWidget(const gf::Texture &defaultTexture, const gf::Texture &selectedTexture, const gf::Texture &disabledTexture)
    : gf::SpriteWidget( defaultTexture, selectedTexture, disabledTexture )
    , currDragging( false )
    {

    }

    void InventoryWidget::unsetTextures()
    {
        unsetDisabledSprite();
        unsetSelectedSprite();
        unsetDefaultSprite();
    }
}