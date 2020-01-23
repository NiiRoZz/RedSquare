#ifndef REDSQUARE_CLIENT_CHAT_H
#define REDSQUARE_CLIENT_CHAT_H

#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include "ThreadCom.h"


namespace redsquare
{
    class Chat
    {
    public:
        Chat(gf::Font &,char *port, char *hostname,const char* name);

        void update(gf::Time time);
        void chatThread();
        void render(gf::RenderTarget& target, const gf::RenderStates& states);
        void processEvent(const gf::Event &event);

        bool m_HoveringChat;
        bool m_TypingInChat;

    private:
        gf::UI m_UI;
        std::string m_Name;

        //Thread chat communication
        gf::Queue<Message> m_ChatQueue;

        //queue of message
        ThreadCom<Message> m_ChatCom;

        std::vector<gf::UICharBuffer> m_tabCharBuffer;
    };
}


#endif