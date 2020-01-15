#include "Chat.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>

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
    Chat::Chat(gf::Font &font, char *port, char *hostname)
    : m_HoveringChat(false)
    , m_TypingInChat(false)
    , m_UI(font)
    , m_ChatShader(VertexShader, FragmentShader)
    , m_ChatCom(hostname, port, m_ChatQueue)
    {
        m_hostname = hostname;
        m_ChatShader.setUniform("u_backgroundColor", gf::Color::Opaque(0.75f));
        m_ChatCom.start();

    }

    void Chat::update(gf::Time time)
    {
        
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
                    
        gf::Coordinates coordinates(target);
        gf::Vector2f ChatWindowSize=coordinates.getRelativeSize({ 0.22f,0.40f });
        gf::Vector2f sizeInterChat=ChatWindowSize*gf::Vector2f(0.700,0.954);
        gf::Vector2f sizeMessageReceiveChat=ChatWindowSize*gf::Vector2f(0.136,0.840);
        gf::Vector2f sizeMessageBackground=ChatWindowSize*gf::Vector2f(0.200,0.954);
        gf::Vector2f sizeMessageToSend=ChatWindowSize*gf::Vector2f(0.100,0.113);
        
        static gf::UICharBuffer box(512);
        static gf::UICharBuffer text(64);

        m_HoveringChat = false;
        m_TypingInChat = false;

        m_UI.setCharacterSize(12);

        if( m_UI.begin("Chat", gf::RectF::fromPositionSize( coordinates.getRelativePoint({ 0.00f,0.595f }),ChatWindowSize),gf::UIWindow::Title|gf::UIWindow::NoScrollbar))
        {
            m_HoveringChat = m_UI.isWindowHovered();
            
            m_UI.layoutRowStatic(sizeInterChat[0], sizeInterChat[1], 1);
            if (m_UI.groupBegin("",gf::UIWindow::ScrollAutoHide))
            {
                m_UI.layoutRowStatic(sizeMessageReceiveChat[0],sizeMessageReceiveChat[1], 1);
                m_UI.edit(gf::UIEditType::Box | gf::UIEdit::ReadOnly  , box);

                m_UI.layoutRowStatic(sizeMessageReceiveChat[0],sizeMessageReceiveChat[1], 1);
                m_UI.edit(gf::UIEditType::Box | gf::UIEdit::ReadOnly, box);

                m_UI.groupEnd();
            }
            m_UI.layoutRowStatic(sizeMessageBackground[0], sizeMessageBackground[1], 1);

            if (m_UI.groupBegin("",gf::UIWindow::Border))
            {
                m_UI.layoutRowBegin(gf::UILayout::Dynamic, sizeMessageToSend[0], 2);
                m_UI.layoutRowPush(0.7f);
                
                gf::UIEditEventFlags flags = m_UI.edit(gf::UIEditType::Field | gf::UIEdit::SigEnter, text, gf::UIEditFilter::Ascii);
                m_TypingInChat = (flags & gf::UIEditEvent::Active);

                m_UI.layoutRowPush(0.30f);
                
                if ( m_UI.buttonLabel("Submit") || flags.test(gf::UIEditEvent::Commited) )
                {
                    std::cout << "envoyé" << std::endl;
                    Packet sendPacket;
                    sendPacket.type = PacketType::Message;
                    strcpy(sendPacket.reveiveMessage.from,m_hostname);
                    char cpy[256];
                    std::string s = box.asString();
                    strcpy(cpy,s.c_str()) ;
                    strcpy(sendPacket.reveiveMessage.message,cpy);
                    m_ChatCom.sendPacket(sendPacket);
                }
                m_UI.layoutRowEnd();
                m_UI.groupEnd();
            } 
            m_UI.end();
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
