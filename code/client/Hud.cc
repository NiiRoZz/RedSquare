#include "Hud.h"
#include "../common/Singletons.h"
#include "Game.h"

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
    Hud::Hud(Game &game, gf::Font &font)
    : m_Game(game)
    , m_UiChat(font)
    , m_Font(font)
    , m_ChatShader(VertexShader, FragmentShader)
    {
        gMessageManager().registerHandler<SpellUpdateMessage>(&Hud::onSpellUpdate, this);
        
        m_ChatShader.setUniform("u_backgroundColor", gf::Color::Opaque(0.75f));
    }

    static constexpr float HudSpellSize = 55.0f;
    static constexpr float HudSpellTextureSize = 16.0f;
    
    void Hud::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        //std::cout << "m_Chat.m_HoveringChat : " << m_Chat.m_HoveringChat << std::endl;
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
        text.setString("Etage : " + std::to_string(m_Game.m_Floor));
        text.setAnchor(gf::Anchor::TopLeft);
        text.setPosition(coordinates.getRelativePoint({ 0.03f, 0.05f }));
        target.draw(text, states);

        float x = 0;
        float y = 0;
        uint index = 1;

        for (auto it = m_spellsTextures.begin(); it != m_spellsTextures.end(); ++it)
        {
            if (*it != nullptr)
            {
                gf::Sprite sprite;
                sprite.setTexture(**it);
                sprite.setAnchor(gf::Anchor::TopLeft);
                sprite.setPosition(coordinates.getRelativePoint({ 0.43f, 0.86f })+gf::Vector2f(x, y)+ HudSpellSize*gf::Vector2f(x, y)*coordinates.getRelativeSize({ 0.001f, 0.001f }).height);
                sprite.setScale((HudSpellSize / HudSpellTextureSize)*coordinates.getRelativeSize({ 0.001f, 0.001f }).height);
                target.draw(sprite, states);
                x += 1.2;

                if ( index % 4 == 0 )
                {
                    y += 1.2;
                    x = 0;
                }

                index++;
            }
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

    gf::Texture* Hud::getTextureFromSpellType(SpellType type)
    {
        return &gResourceManager().getTexture("img/redsquare.png");
    }

    gf::MessageStatus Hud::onSpellUpdate(gf::Id id, gf::Message *msg)
    {
        assert(id == SpellUpdateMessage::type);

        auto message = static_cast<SpellUpdateMessage*>(msg);
        
        m_spellsTextures.clear();

        for(auto it = message->spells.begin(); it != message->spells.end(); ++it)
        {
            if (*it != SpellType::Unknow)
            {
                m_spellsTextures.push_back(getTextureFromSpellType(*it));
            }
        }

        return gf::MessageStatus::Keep;
    }
}