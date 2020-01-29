#ifndef REDSQUARE_CLIENT_INVENTORYSLOT_H
#define REDSQUARE_CLIENT_INVENTORYSLOT_H

#include "InventoryWidget.h"
#include "../common/Packet.h"
#include "ClientItem.h"

#include <gf/Time.h>

namespace redsquare
{
    class InventorySlot
    {
    public:
        InventorySlot(InventorySlotType slotType, uint pos = 0);

        void render(gf::RenderTarget& target, const gf::RenderStates& states);

        void update(gf::Vector2f pos, gf::Vector2f scale);

        void update(gf::Time time);

        bool contains(gf::Vector2f coords);

        InventoryWidget* getItemWidget(gf::Vector2f coords);

        InventorySlotType getSlotType() const;
        uint getSlotPos() const;

        bool haveItem() const;

        ClientItem *getItem() const;

        void setItem(ClientItem *item);

        void setMoveItemRequest();

        bool hasMoveItemRequest() const;

    private:
        void resetMoveItemRequest();

        gf::Texture &m_BackgroundTexture;
        InventoryWidget m_BackgroundWidget;
        ClientItem *m_Item;
        InventoryWidget m_ItemWidget;

        InventorySlotType m_SlotType;
        uint m_SlotPos;

        bool m_MoveItemRequested;
        gf::Time m_TimeSinceMoveItemRequest;
    };
}

#endif