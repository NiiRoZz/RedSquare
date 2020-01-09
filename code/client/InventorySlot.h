#ifndef REDSQUARE_CLIENT_INVENTORYSLOT_H
#define REDSQUARE_CLIENT_INVENTORYSLOT_H

#include "InventoryWidget.h"

namespace redsquare
{
    class InventorySlot
    {
    public:
        InventorySlot(bool item);

        void render(gf::RenderTarget& target, const gf::RenderStates& states);

        void update(gf::Vector2f pos, gf::Vector2f scale);

        bool contains(gf::Vector2f coords);

        InventoryWidget* getItemWidget(gf::Vector2f coords);

        bool haveItem;

    private:
        gf::Texture &m_BackgroundTexture;
        InventoryWidget m_BackgroundWidget;
        //TODO: Do item here : Item *m_Item and remove m_ItemTexture
        gf::Texture &m_ItemTexture;
        InventoryWidget m_ItemWidget;
    };
}

#endif