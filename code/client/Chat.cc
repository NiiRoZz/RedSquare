#include "Chat.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>

namespace redsquare
{
    Chat::Chat()
    {

    }

    void Chat::updateChat(gf::UI &chatUI)
    {
        static gf::UICharBuffer box(512);
        static gf::UICharBuffer text(64);

        static gf::Color4f comboColor1 = gf::Color::fromRgba32(255, 255, 255, 100);
        chatUI.setCharacterSize(12);
        if( chatUI.begin("Chat", gf::RectF(0, 350, 220, 220),  gf::UIWindow::Movable |gf::UIWindow::Title|gf::UIWindow::NoScrollbar)){
            
            static gf::UICollapse groupCollapsed = gf::UICollapse::Minimized;
            
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
                
                if (chatUI.buttonLabel("Submit") || flags.test(gf::UIEditEvent::Commited))
                {
                    std::cout <<"envoyé";
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
