#ifndef REDSQUARE_CLIENT_INVENTORYSLOT_H
#define REDSQUARE_CLIENT_INVENTORYSLOT_H

#include "InventoryWidget.h"
#include "../common/Packet.h"
#include "Item.h"

namespace redsquare
{
    class InventorySlot
    {
    public:
        InventorySlot(InventorySlotType slotType, uint pos = 0);

        void render(gf::RenderTarget& target, const gf::RenderStates& states);

        void update(gf::Vector2f pos, gf::Vector2f scale);

        bool contains(gf::Vector2f coords);

        InventoryWidget* getItemWidget(gf::Vector2f coords);

        InventorySlotType getSlotType() const;
        uint getSlotPos() const;

        bool haveItem() const;

        Item *getItem() const;

        void setItem(Item *item);

    private:
        gf::Texture &m_BackgroundTexture;
        InventoryWidget m_BackgroundWidget;
        Item *m_Item;
        InventoryWidget m_ItemWidget;

        InventorySlotType m_SlotType;
        uint m_SlotPos;
    };
}

#endif