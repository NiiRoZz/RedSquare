#ifndef REDSQUARE_CLIENT_HUD_H
#define REDSQUARE_CLIENT_HUD_H

#include "Chat.h"
#include "Game.h"
#include "MainMenu.h"
#include "Inventory.h"
#include "../common/Message.h"
#include "SpellWidget.h"

#include <gf/Font.h>
#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Entity.h>

namespace redsquare
{
    class Hud: public gf::Entity
    {
    public:
        Hud(Game &game, gf::Font &font,char* port, char *hostname, gf::ExtendView &view,const char* name);

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        void processEvent(const gf::Event &event);

        bool hoveringChat();
        bool typingInChat();

        void showMap();
        void hideChat();

        MainMenu m_MainMenu;
        
    private:
        Game &m_Game;
        Chat m_Chat;
        Inventory m_Inventory;
       
        gf::Font &m_Font;
        //View of the game
        gf::ExtendView &m_View;
        std::vector<SpellWidget> m_SpellsWidgets;
        SpellWidget *m_SpellWidgetHover;
        gf::Vector2f m_MouseHoverPostionOnSpell;

        bool m_ShowMap;
        bool m_HideChat;

        gf::MessageStatus onSpellUpdate(gf::Id id, gf::Message *msg);
    };

}


#endif
