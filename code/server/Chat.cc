#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include "Chat.h"

namespace redsquare
{
    void Chat::sendMessageToAll(Message& packet)
    {
        auto it = m_PlayersSocket.begin();
        // Iterate over the map using Iterator till end.
        if(packet.to!= ""){
            while ( it != m_PlayersSocket.end() )
            {
                it->second.send(packet);
                ++it;
            }
        }else{
            while ( it != m_PlayersSocket.end() )
            {
                it->second.send(packet);
                ++it;
            }
        }
        
    }


    void Chat::chatThread()
    {
        for(;;)
        {
            Message packet;
            while ( m_chatQueue.poll(packet))
            {
                sendMessageToAll(packet);
            }
        }
    }

    void Chat::receiveMessagePacket(SocketTcp& socket){
        for(;;)
        {
            Message packet;
            socket.receive(packet);

            if(socket.getState()==SocketState::Disconnected) return; 

            m_chatQueue.push(std::move(packet));
        }
        
    }

    void Chat::startChat()
    {
        std::thread(&Chat::chatThread, this).detach();
    }
    
    void Chat::addPlayer(gf::Id idPlayer, std::string namePlayer , SocketTcp socket)
    {
        m_PlayersSocket.insert(std::make_pair(idPlayer, std::move(socket)));
        m_PlayersName.insert(std::make_pair(idPlayer,namePlayer));// /!\ nom attendu, modifier la socket
        std::thread(&Chat::receiveMessagePacket, this, std::ref(m_PlayersSocket[idPlayer])).detach();
    }

}
