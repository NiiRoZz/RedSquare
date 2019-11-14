#include "Hud.h"
#include "../common/Singletons.h"

#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Color.h>
#include <gf/Text.h>
#include <vector> 

namespace redsquare
{
    

    Hud::Hud(gf::Font &font)
    : uiChat(font)
    , m_Font(font)
    {
        m_spellsTextures.push_back(&gResourceManager().getTexture("img/redsquare.png"));
        m_spellsTextures.push_back(&gResourceManager().getTexture("img/redsquare.png"));
        m_spellsTextures.push_back(&gResourceManager().getTexture("img/redsquare.png"));
        m_spellsTextures.push_back(&gResourceManager().getTexture("img/redsquare.png"));
    }

    static constexpr float HudPadding = 20.0f;
    static constexpr float HudSpellSize = 50.0f;
    static constexpr float HudSpellTextureSize = 16.0f;
    static constexpr float HudSpellSpace = 10.0f;

    void Hud::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        target.draw(uiChat);

        gf::Coordinates coordinates(target);

        gf::Text text;
        text.setFont(m_Font);
        text.setOutlineColor(gf::Color::White);
        text.setOutlineThickness(coordinates.getRelativeSize({ 1.0f, 0.002f }).height);
        text.setCharacterSize(coordinates.getRelativeCharacterSize(0.05f));
        text.setString("Etage :");
        text.setAnchor(gf::Anchor::TopLeft);
        text.setPosition(coordinates.getRelativePoint({ 0.03f, 0.05f }));
        target.draw(text, states);

        gf::Vector2f position(HudPadding, HudPadding);
        position = coordinates.getAbsolutePoint({ HudPadding, HudSpellSize + HudSpellSpace + HudSpellSize + HudPadding }, gf::Anchor::BottomLeft);
        std::size_t index=2;
        for (auto it= m_spellsTextures.begin(); it!=m_spellsTextures.end();++it){
            
            gf::Sprite sprite;
            sprite.setTexture(**it);
            sprite.setPosition(position + HudSpellSize +  gf::Vector2f(index, 0) * HudSpellTextureSize  );
            sprite.setScale(HudSpellSize / HudSpellTextureSize);
            target.draw(sprite, states);
            index+=3.5;
        }


         
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