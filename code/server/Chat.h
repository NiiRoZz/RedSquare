#ifndef REDSQUARE_SERVER_CHAT_H
#define REDSQUARE_SERVER_CHAT_H


#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Id.h>
#include <thread>
#include "Player.h"


namespace redsquare
{
    class Chat
    {
        public :
            void addPlayer(gf::Id idPlayer, SocketTcp socket);
            void sendMessageToAll(Packet& packet);
            void startChat();
            void chatThread();
            void receiveMessagePacket(SocketTcp& socket);
            
        private : 

            std::map<gf::Id, SocketTcp> m_PlayersSocket;
            gf::Queue<Packet> m_chatQueue;



    };
}

#endif //REDSQUARE_SERVER_CHAT_H