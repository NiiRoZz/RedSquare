#include "Chat.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>

namespace redsquare
{
    Chat::Chat()
    : m_HoveringChat(false)
    {

    }

    void Chat::updateChat(gf::UI &chatUI)
    {
        static gf::UICharBuffer box(512);
        static gf::UICharBuffer text(64);

        m_HoveringChat = false;

        chatUI.setCharacterSize(12);

        if( chatUI.begin("Chat", gf::RectF::fromPositionSize( {0, 350}, {220, 220} ),  gf::UIWindow::Movable |gf::UIWindow::Title|gf::UIWindow::NoScrollbar))
        {
            m_HoveringChat = chatUI.isWindowHovered();
            
            chatUI.layoutRowStatic(150, 210, 1);
            if (chatUI.groupBegin(""))
            {
                chatUI.layoutRowStatic(30, 185, 1);
                chatUI.edit(gf::UIEditType::Box | gf::UIEdit::ReadOnly  , box);

                chatUI.layoutRowStatic(30, 185, 1);
                chatUI.edit(gf::UIEditType::Box | gf::UIEdit::ReadOnly, box);

                chatUI.groupEnd();
            }
            chatUI.layoutRowStatic(50, 200, 1);

            if (chatUI.groupBegin(""))
            {
                chatUI.layoutRowBegin(gf::UILayout::Static, 25, 2);
                chatUI.layoutRowPush(113);
                
                gf::UIEditEventFlags flags = chatUI.edit(gf::UIEditType::Field | gf::UIEdit::SigEnter, text, gf::UIEditFilter::Ascii);
                chatUI.layoutRowPush(60);
                
                if ( chatUI.buttonLabel("Submit") || flags.test(gf::UIEditEvent::Commited) )
                {
                    std::cout << "envoyé" << std::endl;
                    //box.append(text);
                    //text.clear();
                }
                chatUI.layoutRowEnd();
                chatUI.groupEnd();
            } 
            chatUI.end();
        }
    }
}
