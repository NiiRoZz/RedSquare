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
        chatUI.setCharacterSize(10);
        if( chatUI.begin("Chat", gf::RectF(0, 360, 220, 220), gf::UIWindow::Border | gf::UIWindow::Title|gf::UIWindow::NoScrollbar)){
            
            static gf::UICollapse groupCollapsed = gf::UICollapse::Minimized;
            
            chatUI.layoutRowStatic(130, 200, 1);
            if (chatUI.groupBegin("", gf::UIWindow::Border))
            {
                chatUI.layoutRowStatic(50, 175, 1);
                chatUI.edit(gf::UIEditType::Box | gf::UIEdit::ReadOnly, box);

                chatUI.layoutRowStatic(50, 175, 1);
                chatUI.edit(gf::UIEditType::Box | gf::UIEdit::ReadOnly, box);

                chatUI.groupEnd();
            }
            chatUI.layoutRowStatic(50, 200, 1);

            if (chatUI.groupBegin("test", gf::UIWindow::Border))
            {
                chatUI.layoutRowBegin(gf::UILayout::Static, 20, 2);
                chatUI.layoutRowPush(120);
                gf::UIEditEventFlags flags = chatUI.edit(gf::UIEditType::Field | gf::UIEdit::SigEnter, text, gf::UIEditFilter::Ascii);
                chatUI.layoutRowPush(50);
                
                if (chatUI.buttonLabel("Submit") || flags.test(gf::UIEditEvent::Commited))
                {
                    box.append(text);
                    text.clear();
                }
                chatUI.layoutRowEnd();
                chatUI.groupEnd();
            } 
            chatUI.end();
        }
    }
}
