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
        Hud(gf::Font &fontChat);

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        void processEvent(const gf::Event &event);
        
    private:
        Chat chat;
        gf::UI uiChat;
        gf::Font m_Font;
    };

}


#endif