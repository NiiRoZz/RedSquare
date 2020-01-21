#ifndef REDSQUARE_CLIENT_INVENTORY_H
#define REDSQUARE_CLIENT_INVENTORY_H

#include "../common/Message.h"
#include "InventorySlot.h"

#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>
#include <gf/Entity.h>
#include <iostream>

namespace redsquare
{
    //Forward classes
    class Game;
    
    class Inventory
    {
    public:
        Inventory(gf::Font &font, Game &game);

        void update(gf::Time time);

        void render(gf::RenderTarget& target, const gf::RenderStates& states);

        void processEvent(const gf::Event &event);

        bool m_ShowInventory;

    private:
        Game &m_Game;
        gf::UI m_UI;

        gf::MessageStatus onInventoryUpdate(gf::Id id, gf::Message *msg);
        gf::MessageStatus onItemUpdate(gf::Id id, gf::Message *msg);
        gf::MessageStatus onItemMove(gf::Id id, gf::Message *msg);
        gf::MessageStatus onMyPlayerReceived(gf::Id id, gf::Message *msg);

        void addItem(InventorySlotType slotType, ClientItem &&item, uint pos = 0);

        const float IconSize = 16.f;

        const uint PlayerTextureSize = 16.f;
        const float StartPercentagePosPlayerWidget = 0.042;
        const float EndPercentagePosPlayerWidget = 0.38;
        const float WidthPlayerWidget = 0.3;
        InventoryWidget m_PlayerWidget;

        const float StartPercentageGearStuff = 0.07;
        const float WidthPercentageSpecialSlot = 0.1;
        const float HeightPercentageSpecialSlot = 0.3;
        const float WidthPositionGearStuff = 0.35;
        std::map<InventorySlotType, InventorySlot> m_SpecialSlots;
        std::map<InventorySlotType, ClientItem> m_SpecialItems;

        const uint RowCargoSlotNmb = 6;
        const uint ColumnCargoSlotNmb = 10;
        const float SpaceBetweenSlots = 5.f;
        const float StartPercentagePos = 0.4;
        std::map<uint, InventorySlot> m_CargoSlots;
        std::map<uint, ClientItem> m_CargoItems;

        //For drag
        InventorySlot *m_OldSlot;
        InventoryWidget *m_CurrMovingWidget;
        ClientItem *m_CurrMovingItem;
        gf::Vector2f m_OffsetDrag;
    };
}


#endif