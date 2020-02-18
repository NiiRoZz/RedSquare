#ifndef REDSQUARE_CLIENT_LOBBY_CHAT_H
#define REDSQUARE_CLIENT_LOBBY_CHAT_H

#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/StaticString.h>
#include <gf/Id.h>
#include <imgui.h>
#include <imgui_impl_gf.h>

#include "../common/ProtocolData.h"


namespace redsquare
{
    //Forward class
    class ClientNetwork;

    class LobbyChat
    {
    public:
        static ImVec4 toColor(gf::Id id);
    public:
        LobbyChat(ClientNetwork& network);

        void appendMessage(const MessageData& message);
        void appendMessage(MessageData&& message);

        void display(int lines);

        bool hoveringChat() const;

    protected:
        ClientNetwork &m_Network;
        gf::StaticString<255> m_LineBuffer;
        std::vector<MessageData> m_Messages;
        bool m_AutoScroll;
    };
}


#endif //REDSQUARE_CLIENT_LOBBY_CHAT_H