#include "Hud.h"

#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Color.h>
#include <gf/Text.h>

namespace redsquare
{
    

    Hud::Hud(gf::Font &fontChat)
    : uiChat(fontChat)
    
    {
       // m_Pos = gf::Vector2i( 0, 0 );
    }

    void Hud::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        target.draw(uiChat);
         gf::Text m_etage;
        m_etage.setCharacterSize(8);
        m_etage.setColor(gf::Color::Black);
        m_etage.setFont(m_Font);
        m_etage.setString("Etage : 1");
       // m_etage.setPosition(m_Pos);
        m_etage.setAnchor(gf::Anchor::CenterRight);
        target.draw(m_etage, states);
    }

    void Hud::update(gf::Time time)
    {
        chat.updateChat(uiChat);
    }

    void Hud::processEvent(const gf::Event &event)
    {
        uiChat.processEvent(event);
    }
}