#ifndef REDSQUARE_CLIENT_HUD_H
#define REDSQUARE_CLIENT_HUD_H

#include "Chat.h"
#include "Game.h"
#include "Inventory.h"
#include "../common/Message.h"
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
        Hud(Game &game, gf::Font &font);

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        void processEvent(const gf::Event &event);

        bool hoveringChat();
        
    private:
        Game &m_Game;
        Chat m_Chat;
        Inventory m_Inventory;
        gf::Font &m_Font;
        std::vector<gf::Texture*> m_spellsTextures;
        

        gf::Texture* getTextureFromSpellType(SpellType type);

        gf::MessageStatus onSpellUpdate(gf::Id id, gf::Message *msg);


    };

}


#endif