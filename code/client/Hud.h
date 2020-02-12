#ifndef REDSQUARE_CLIENT_HUD_H
#define REDSQUARE_CLIENT_HUD_H

#include "Chat.h"
#include "InventoryUI.h"
#include "Message.h"
#include "SpellWidget.h"

#include <gf/Font.h>
#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Entity.h>
#include <gf/Widgets.h>
#include <gf/WidgetContainer.h>

namespace redsquare
{
    //Forward classes
    class GameScene;
    struct Scenes;

    class Hud: public gf::Entity
    {
    public:
        Hud(Scenes &scenes, GameScene &game, gf::Font &font);

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        void processEvent(const gf::Event &event);

        bool hoveringChat();
        bool shownInventory();

        void showMap();
        void showChat();
        void showHelp();
        void showInventory();

        InventoryUI& getInventoryUI();
        Chat& getChat();
        
    private:
        gf::MessageStatus onSpellUpdate(gf::Id id, gf::Message *msg);
        gf::MessageStatus onPlayerDeadUpdate(gf::Id id, gf::Message *msg);

        Scenes &m_Scenes;
        GameScene &m_Game;
        Chat m_Chat;
        InventoryUI m_InventoryUI;
       
        gf::Font &m_Font;
        gf::UI m_UI;
        std::vector<SpellWidget> m_SpellsWidgets;
        SpellWidget *m_SpellWidgetHover;
        gf::Vector2f m_MouseHoverPostionOnSpell;

        bool m_ShowMap;
        bool m_ShowChat;
        bool m_ShowHelp;
        bool m_ShowInventory;
        bool m_PlayerDead;

        gf::TextWidget m_QuitWidget;
    };

}


#endif
