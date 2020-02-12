#ifndef REDSQUARE_CLIENT_INVENTORYUI_H
#define REDSQUARE_CLIENT_INVENTORYUI_H

#include "Message.h"
#include "InventorySlot.h"
#include "ClientEntity.h"

#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>
#include <gf/Entity.h>
#include <gf/Text.h>
#include <iostream>

namespace redsquare
{
    //Forward classes
    class GameScene;
    
    class InventoryUI
    {
    public:
        InventoryUI(gf::Font &font, GameScene &game);

        void update(gf::Time time);

        void render(gf::RenderTarget& target, const gf::RenderStates& states);

        void processEvent(const gf::Event &event);

        void setVinicityObject(ClientEntity *vinicityObject);

    private:
    
        gf::MessageStatus onItemUpdateUI(gf::Id id, gf::Message *msg);
        gf::MessageStatus onMyPlayerReceived(gf::Id id, gf::Message *msg);

        GameScene &m_Game;
        gf::UI m_UI;

        Player *m_PlayerEntity;
        ClientEntity *m_VinicityEntity;

        const float IconSize = 16.f;

        const uint PlayerTextureSize = 16.f;
        const float StartPercentagePosPlayerWidget = 0.042;
        const float EndPercentagePosPlayerWidget = 0.38;
        const float WidthPlayerWidget = 0.3;
        InventoryWidget m_PlayerWidget;

        const float StartPercentageGearStuff = 0.01;
        const float WidthPercentageSpecialSlot = 0.1;
        const float HeightPercentageSpecialSlot = 0.3;
        const float WidthPositionGearStuff = 0.35;
        std::map<InventorySlotType, InventorySlot> m_PlayerSpecialSlots;

        const float m_SpaceBetweenPlayerSlotsCargo = 5.f;
        const float m_StartPercentagePosPlayerCargo = 0.4;
        std::map<uint, InventorySlot> m_PlayerCargoSlots;

        const float m_SpaceBetweenVinictySlotsCargo = 5.f;
        const float m_StartPercentagePosVinicityCargo = 0.4;
        std::map<uint, InventorySlot> m_VinicityCargoSlots;

        const float StartPercentagePosVinicityWidget = 0.042;
        const float EndPercentagePosVinicityWidget = 0.38;
        const float WidthVinictyWidget = 0.3;
        InventoryWidget m_VinictyWidget;

        //For drag
        InventorySlot *m_OldSlot;
        InventoryWidget *m_CurrMovingWidget;
        ClientItem *m_CurrMovingItem;
        gf::Vector2f m_OffsetDrag;
        ClientEntity *m_DraggingFromEntity;

        //For display Information
        InventorySlot *m_HoveringSlot;
        gf::Text m_NameText;
        gf::RectangleShape m_NameBackgroundShape;
        gf::Text m_DescriptionText;
        gf::RectangleShape m_DescriptionBackgroundShape;

        //For right click
        InventorySlot *m_RightClickedSlot;
        ClientEntity *m_RightClickedFromEntity;
        gf::TextButtonWidget m_DropButton;
        gf::TextButtonWidget m_UseButton;
        gf::RectangleShape m_RightClickedBackground;

        //For display Information of life
        gf::Text m_CharacteristicText;
    };
}


#endif