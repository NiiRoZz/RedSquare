#include "Chat.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
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
    Chat::Chat(gf::Font &font, char *port, char *hostname,const char* name)
    : m_HoveringChat(false)
    , m_TypingInChat(false)
    , m_UI(font)
    , m_ChatShader(VertexShader, FragmentShader)
    , m_ChatCom(hostname, port, m_ChatQueue)
    {
        m_Name = name;
        m_ChatShader.setUniform("u_backgroundColor", gf::Color::Opaque(1.0f));
        m_ChatCom.start();

    }

    
    void Chat::update(gf::Time time)
    {
        Packet packet;
        while(m_ChatQueue.poll(packet)){
             if(packet.type != PacketType::Message){
                    continue;
            }
            std::string from = packet.receiveMessage.from;
            std::cout <<from<<std::endl;
            std::string message = packet.receiveMessage.message;
            std::string text = from + " : "+message;
            std::cout<< text<<std::endl;
            gf::UICharBuffer box(512);
            box.append(text);
            m_tabCharBuffer.push_back(std::move(box));
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
            m_ChatShader.setUniform("u_backgroundColor", gf::Color::Opaque(1.0f));
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
        m_UI.setPredefinedStyle(gf::UIPredefinedStyle::Dark);

        if( m_UI.begin("Chat", gf::RectF::fromPositionSize( coordinates.getRelativePoint({ 0.00f,0.595f }),ChatWindowSize),gf::UIWindow::Title|gf::UIWindow::NoScrollbar))
        {
            m_HoveringChat = m_UI.isWindowHovered();
            
            m_UI.layoutRowStatic(sizeInterChat[0], sizeInterChat[1], 1);
            if (m_UI.groupBegin("",gf::UIWindow::ScrollAutoHide))
            {
                m_UI.layoutRowStatic(sizeMessageReceiveChat[0],sizeMessageReceiveChat[1], 1);
                m_UI.edit(gf::UIEditType::Box | gf::UIEdit::ReadOnly,box);
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
                    Packet sendPacket;
                    sendPacket.type = PacketType::Message;
                    std::cout<<"name : "<<m_Name;
                    //strcpy(sendPacket.receiveMessage.from,m_Name);
                    strncpy(sendPacket.receiveMessage.from, m_Name, 30);

                    //strcpy(sendPacket.receiveMessage.message,text.asString().c_str());
                    strncpy(sendPacket.receiveMessage.message, text.asString().c_str(), 1024);
                    m_ChatCom.sendPacket(sendPacket);
                    std::cout<< "envoyé : " <<sendPacket.receiveMessage.from <<std::endl;
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
