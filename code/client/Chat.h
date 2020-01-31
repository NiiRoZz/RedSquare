#ifndef REDSQUARE_CLIENT_CHAT_H
#define REDSQUARE_CLIENT_CHAT_H

#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/StaticString.h>
#include <imgui.h>
#include <imgui_impl_gf.h>

#include "ThreadCom.h"


namespace redsquare
{
    constexpr ImGuiWindowFlags DefaultWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;

    class Chat
    {
    public:
        Chat(gf::Font &,char *port, char *hostname,const char* name);

        void update(gf::Time time);
        void chatThread();
        void render(gf::RenderTarget& target, const gf::RenderStates& states);
        void processEvent(const gf::Event &event);
        std::string grepPrivateMessage(std::string str);
        std::string eraseWord(std::string str);

        bool m_HoveringChat;

    private:
        gf::UI m_UI;
        std::string m_Name;
        //Thread chat communication
        gf::Queue<Message> m_ChatQueue;

        //queue of message
        ThreadCom<Message> m_ChatCom;

        std::vector<Message> m_ChatMessageBuffer;

        gf::StaticString<MAX_SIZE_MESSAGE_CHAT> m_LineBuffer;
        bool m_AutoScroll;
    };
}


#endif