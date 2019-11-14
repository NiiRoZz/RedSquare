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

static const char *VertexShader = R"(
#version 100

attribute vec2 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoords;

varying vec4 v_color;
varying vec2 v_texCoords;

uniform mat3 u_transform;

void main(void) {
  v_texCoords = a_texCoords;
  v_color = a_color;
  vec3 worldPosition = vec3(a_position, 1);
  vec3 normalizedPosition = worldPosition * u_transform;
  gl_Position = vec4(normalizedPosition.xy, 0, 1);
}
)";

static const char *FragmentShader = R"(
#version 100

precision mediump float;

varying vec4 v_color;
varying vec2 v_texCoords;

uniform sampler2D u_texture;
uniform vec4 u_backgroundColor;

void main(void) {
  vec4 color = texture2D(u_texture, v_texCoords);
  gl_FragColor = color * v_color * u_backgroundColor;
}
)";

namespace redsquare
{
    Hud::Hud(gf::Font &font)
    : m_UiChat(font)
    , m_Font(font)
    {
        m_spellsTextures.push_back(&gResourceManager().getTexture("img/redsquare.png"));
        m_spellsTextures.push_back(&gResourceManager().getTexture("img/redsquare.png"));
        m_spellsTextures.push_back(&gResourceManager().getTexture("img/redsquare.png"));
        m_spellsTextures.push_back(&gResourceManager().getTexture("img/redsquare.png"));
        
        m_ChatShader.loadFromMemory(VertexShader, FragmentShader);
        m_ChatShader.setUniform("u_backgroundColor", gf::Color::Opaque(0.75f));
    }

    static constexpr float HudPadding = 20.0f;
    static constexpr float HudSpellSize = 50.0f;
    static constexpr float HudSpellTextureSize = 16.0f;
    static constexpr float HudSpellSpace = 10.0f;

    void Hud::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        std::cout << "m_Chat.m_HoveringChat : " << m_Chat.m_HoveringChat << std::endl;
        if (m_Chat.m_HoveringChat)
        {
            m_ChatShader.setUniform("u_backgroundColor", gf::Color::Opaque(1.0f));
        }
        else
        {
            m_ChatShader.setUniform("u_backgroundColor", gf::Color::Opaque(0.75f));
        }
        
        gf::RenderStates localChatStates = states;
        localChatStates.shader = &m_ChatShader;
        target.draw(m_UiChat, localChatStates);

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
        m_Chat.updateChat(m_UiChat);
    }

    void Hud::processEvent(const gf::Event &event)
    {
        m_UiChat.processEvent(event);
    }

    bool Hud::hoveringChat()
    {
        return m_Chat.m_HoveringChat;
    }
}