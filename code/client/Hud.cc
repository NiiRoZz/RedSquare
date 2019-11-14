#include "Hud.h"

#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Color.h>
#include <gf/Text.h>

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
    

    Hud::Hud(gf::Font &fontChat)
    : m_UiChat(fontChat)
    {
        m_ChatShader.loadFromMemory(VertexShader, FragmentShader);
        m_ChatShader.setUniform("u_backgroundColor", gf::Color::Opaque(0.75f));
       // m_Pos = gf::Vector2i( 0, 0 );
    }

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