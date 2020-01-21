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
        void writeMessage(Packet packet);
        bool m_HoveringChat;
        bool m_TypingInChat;

    private:
        gf::Shader m_ChatShader;
        gf::UI m_UI;
        const char * m_Name;

        //Thread chat communication
        gf::Queue<Packet> m_ChatQueue;

        //queue of message
        ThreadCom m_ChatCom;

        std::vector<gf::UICharBuffer> m_tabCharBuffer;



    };
}


#endif