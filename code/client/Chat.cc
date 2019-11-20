#include "Chat.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>

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
    Chat::Chat(gf::Font &font)
    : m_HoveringChat(false)
    , m_TypingInChat(false)
    , m_UI(font)
    , m_ChatShader(VertexShader, FragmentShader)
    {
        m_ChatShader.setUniform("u_backgroundColor", gf::Color::Opaque(0.75f));
    }

    void Chat::update(gf::Time time)
    {
        static gf::UICharBuffer box(512);
        static gf::UICharBuffer text(64);

        m_HoveringChat = false;
        m_TypingInChat = false;

        m_UI.setCharacterSize(12);

        if( m_UI.begin("Chat", gf::RectF::fromPositionSize( {0, 350}, {220, 220} ),  gf::UIWindow::Movable |gf::UIWindow::Title|gf::UIWindow::NoScrollbar))
        {
            m_HoveringChat = m_UI.isWindowHovered();
            
            m_UI.layoutRowStatic(150, 210, 1);
            if (m_UI.groupBegin(""))
            {
                m_UI.layoutRowStatic(30, 185, 1);
                m_UI.edit(gf::UIEditType::Box | gf::UIEdit::ReadOnly  , box);

                m_UI.layoutRowStatic(30, 185, 1);
                m_UI.edit(gf::UIEditType::Box | gf::UIEdit::ReadOnly, box);

                m_UI.groupEnd();
            }
            m_UI.layoutRowStatic(50, 200, 1);

            if (m_UI.groupBegin(""))
            {
                m_UI.layoutRowBegin(gf::UILayout::Static, 25, 2);
                m_UI.layoutRowPush(113);
                
                gf::UIEditEventFlags flags = m_UI.edit(gf::UIEditType::Field | gf::UIEdit::SigEnter, text, gf::UIEditFilter::Ascii);
                m_TypingInChat = (flags & gf::UIEditEvent::Active);

                m_UI.layoutRowPush(60);
                
                if ( m_UI.buttonLabel("Submit") || flags.test(gf::UIEditEvent::Commited) )
                {
                    std::cout << "envoyé" << std::endl;
                    //box.append(text);
                    //text.clear();
                }
                m_UI.layoutRowEnd();
                m_UI.groupEnd();
            } 
            m_UI.end();
        }
    }

    void Chat::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        if (m_HoveringChat || m_TypingInChat)
        {
            m_ChatShader.setUniform("u_backgroundColor", gf::Color::Opaque(1.0f));
        }
        else
        {
            m_ChatShader.setUniform("u_backgroundColor", gf::Color::Opaque(0.75f));
        }
        
        gf::RenderStates localChatStates = states;
        localChatStates.shader = &m_ChatShader;
        target.draw(m_UI, localChatStates);
    }

    void Chat::processEvent(const gf::Event &event)
    {
        m_UI.processEvent(event);
    }
}
