#ifndef REDSQUARE_CLIENT_CHAT_H
#define REDSQUARE_CLIENT_CHAT_H

#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>

namespace redsquare
{
    class Chat
    {
    public:
        Chat();

        void updateChat(gf::UI &chatUI);

        bool m_HoveringChat;
    };
}


#endif