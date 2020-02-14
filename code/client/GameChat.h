#ifndef REDSQUARE_GAME_CHAT_H
#define REDSQUARE_GAME_CHAT_H

#include <vector>

#include <gf/StaticString.h>
#include <gf/Coordinates.h>

#include "ClientNetwork.h"
#include "LobbyChat.h"

namespace redsquare
{
    //Forward Classes
    class GameScene;

    class GameChat: public LobbyChat
    {
    public:
        GameChat(ClientNetwork& network, GameScene &game);

        void display(gf::Coordinates& coordinates);

        bool hoveringChat() const;

    private:
        GameScene &m_Game;
        std::string m_SelectedUserName;
        gf::Id m_SelectedUserId;
        bool m_HoveringChat;
    };

}

#endif // REDSQUARE_GAME_CHAT_H
