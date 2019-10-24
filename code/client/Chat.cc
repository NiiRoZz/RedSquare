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
        
        static gf::Color4f comboColor1 = gf::Color::fromRgba32(255, 255, 255, 100);
       if( chatUI.begin("Chat", gf::RectF(0, 360, 220, 220), gf::UIWindow::Border | gf::UIWindow::Title|gf::UIWindow::NoScrollbar)){
           
           static gf::UICollapse groupCollapsed = gf::UICollapse::Minimized;
           
                chatUI.layoutRowStatic(180, 210, 1);
                if (chatUI.groupBegin("", gf::UIWindow::Border))
                {
                    chatUI.layoutRowStatic(18, 100, 1);
                    chatUI.label("test");
                    chatUI.label("test\n ggg");
                    chatUI.label("test");
                    chatUI.label("test");
                    chatUI.label("test");
                    chatUI.label("test");
                    chatUI.label("test");
                    chatUI.label("test");

                    chatUI.label("test");
                    chatUI.label("test");

                    chatUI.groupEnd();
                }
            
            chatUI.end();
       }
    }
}
