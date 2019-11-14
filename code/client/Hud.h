#ifndef REDSQUARE_CLIENT_HUD_H
#define REDSQUARE_CLIENT_HUD_H

#include "Chat.h" 
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
        Hud(gf::Font &font);

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        void processEvent(const gf::Event &event);

        bool hoveringChat();
        
    private:
        Chat m_Chat;
        gf::UI m_UiChat;
        gf::Font &m_Font;
        std::vector<gf::Texture*>m_spellsTextures;
        gf::Shader m_ChatShader;
    };

}


#endif