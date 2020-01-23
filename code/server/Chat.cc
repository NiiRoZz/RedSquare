#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include "Chat.h"

namespace redsquare
{
    void Chat::sendMessageToAll(Packet& packet)
    {
        auto it = m_PlayersSocket.begin();
 
        // Iterate over the map using Iterator till end.
        while ( it != m_PlayersSocket.end() )
        {
            it->second.send(packet);
            ++it;
        }
    }


    void Chat::chatThread()
    {
        for(;;){
            Packet packet;
            while ( m_chatQueue.poll(packet))
            {
                if(packet.type != PacketType::Message){
                    continue;
                }
                std::cout<< "Chat::chatThread : "<< packet.receiveMessage.from << " " << packet.receiveMessage.message << std::endl;
                sendMessageToAll(packet);
            }
            
        }
        
    }

    void Chat::receiveMessagePacket(SocketTcp& socket){
        for(;;)
        {
            Packet packet;

            socket.receive(packet);

            if(socket.getState()==SocketState::Disconnected) return; 

            m_chatQueue.push(std::move(packet));
        }
        
    }

    void Chat::startChat()
    {
        std::thread(&Chat::chatThread, this).detach();
    }
    
    void Chat::addPlayer(gf::Id idPlayer, SocketTcp socket)
    {
        m_PlayersSocket.insert(std::make_pair(idPlayer, std::move(socket)));
        std::thread(&Chat::receiveMessagePacket, this, std::ref(m_PlayersSocket[idPlayer])).detach();
    }

}
