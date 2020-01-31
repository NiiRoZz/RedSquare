#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include "Chat.h"

namespace redsquare
{
    void Chat::sendMessageToAll(Message& packet)
    {
        auto it = m_PlayersName.begin();
        // Iterate over the map using Iterator till end.
        std::string to = packet.to;
        std::cout << "Message reçu : "<<packet.message<<std::endl<<"from : "<<packet.from<<std::endl<<"to : "<<packet.to<<std::endl;
        if(to != "system"){
            std::cout<<"It's a private message"<<std::endl;
            while ( it != m_PlayersName.end() )
            {
                std::cout << "Message privé from : "<<packet.from<<"; to : "<<packet.to<<std::endl;
                if(it->second == packet.to || it->second == packet.from  ){
                    std::cout<<it->first<<std::endl;
                    m_PlayersSocket.at(it->first).send(packet);
                }
                ++it;
            }
        }else{
            std::cout<<"It's  not a private message"<<std::endl;
            while ( it != m_PlayersName.end() )
            {
                m_PlayersSocket.at(it->first).send(packet);
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
    
    void Chat::addPlayer(gf::Id idPlayer,  SocketTcp socket)
    {
        SendNameIdToChat packet;
        socket.receive(packet);
        m_PlayersSocket.insert(std::make_pair(idPlayer, std::move(socket)));
        m_PlayersName.insert(std::make_pair(idPlayer,std::move(std::string(packet.from))));

        std::thread(&Chat::receiveMessagePacket, this, std::ref(m_PlayersSocket[idPlayer])).detach();
    }

}
