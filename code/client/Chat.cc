#include "Chat.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <vector>

namespace redsquare
{
    Chat::Chat(gf::Font &font, char *port, char *hostname,const char* name)
    : m_HoveringChat(false)
    , m_TypingInChat(false)
    , m_UI(font)
    , m_Name(name)
    , m_ChatCom(hostname, port, m_ChatQueue)
    {
        m_ChatCom.start();
    }

    
    void Chat::update(gf::Time time)
    {
        Message packet;
        while(m_ChatQueue.poll(packet))
        {
            std::string from = packet.from;
            std::string message = packet.message;
            std::string text = from + " : "+ message;
            gf::UICharBuffer box(512);
            box.append(text);
            m_tabCharBuffer.push_back(std::move(box));
        }
    }

    void Chat::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
                    
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
            if (m_UI.groupBegin("MessageReceive",gf::UIWindow::ScrollAutoHide))
            {
                for(gf::UICharBuffer &curr : m_tabCharBuffer)
                {
                    m_UI.layoutRowStatic(sizeMessageReceiveChat[0],sizeMessageReceiveChat[1], 1);
                    m_UI.edit(gf::UIEditType::Box | gf::UIEdit::ReadOnly, curr);
                }
            }
            m_UI.groupEnd();

            m_UI.layoutRowStatic(sizeMessageBackground[0], sizeMessageBackground[1], 1);

            if (m_UI.groupBegin("MessageToSend", gf::UIWindow::Border|gf::UIWindow::NoScrollbar ))
            {
                m_UI.layoutRowBegin(gf::UILayout::Dynamic, sizeMessageToSend[0], 2);
                m_UI.layoutRowPush(0.7f);
                
                gf::UIEditEventFlags flags = m_UI.edit(gf::UIEditType::Field | gf::UIEdit::SigEnter, text, gf::UIEditFilter::Ascii);

                //Detect if we are typing in the chat
                m_TypingInChat = (flags & gf::UIEditEvent::Active);

                m_UI.layoutRowPush(0.30f);
                
                if ( m_UI.buttonLabel("Submit") || flags.test(gf::UIEditEvent::Commited) )
                {
                    Message sendPacket;

                    std::size_t length = m_Name.copy(sendPacket.from, m_Name.length());
                    sendPacket.from[length]='\0';

                    std::string message = text.asString();
                    length = message.copy(sendPacket.message, message.length());
                    sendPacket.message[length]='\0';

                    m_ChatCom.sendPacket(sendPacket);

                    text.clear();
                }
                m_UI.layoutRowEnd();
            } 
            m_UI.groupEnd();

            m_UI.end();
        }

        gf::RenderStates localChatStates = states;
        target.draw(m_UI, localChatStates);
    }

    void Chat::processEvent(const gf::Event &event)
    {
        m_UI.processEvent(event);
    }
}
