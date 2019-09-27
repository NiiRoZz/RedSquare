#include "Game.h"
#include "../common/Singletons.h"

#include <gf/Random.h>
#include <iostream>

namespace redsquare
{
    Game::Game(std::uint16_t port)
    : m_Listener(port)
    {
        std::cout << "Game::Game" << std::endl;
        m_Listener.handleNewConnection([this](SocketTcp clientSocket){
            this->addNewPlayer(std::move(clientSocket));
        });
    }

    void Game::addNewPlayer(SocketTcp socket)
    {
        // Generate a new ID
        gf::Id id = generateId();
        std::map<gf::Id, Player>::iterator itNewPlayer;

        // Create a new player
        {
            std::tie(itNewPlayer, std::ignore) = m_Players.emplace(id, Player(std::move(socket), m_ComQueue, id));
        }
        itNewPlayer->second.initialize();

        // Send to the client his ID
        Packet packet;
        packet.type = PacketType::NewPlayer;
        packet.newPlayer.playerID = id;
        itNewPlayer->second.sendPacket(packet);

        //HACKY, too, sending fake move to all other players INCLUDE HIMSELF!!! Should be reworked
        packet.type = PacketType::ReceiveMove;
        packet.receiveMove.playerID = id;
        packet.receiveMove.posX = itNewPlayer->second.m_Pos[0];
        packet.receiveMove.posY = itNewPlayer->second.m_Pos[1];
        sendPacketToAllPlayers( packet );

        //HACKY, find best way, fake a move of all players inside the game to make them apparear in the new client
        auto it = m_Players.begin();
        // Iterate over the map using Iterator till end.
        while (it != m_Players.end())
        {
            if ( it->first != id )
            {
                packet.type = PacketType::ReceiveMove;
                packet.receiveMove.playerID = it->first;
                packet.receiveMove.posX = it->second.m_Pos[0];
                packet.receiveMove.posY = it->second.m_Pos[1];
                itNewPlayer->second.sendPacket( packet );
            }

            ++it;
        }
    }

    gf::Id Game::generateId() const
    {
        uint64_t min = std::numeric_limits<uint64_t>::min();
        uint64_t max = std::numeric_limits<uint64_t>::max();
        uint64_t number = gRandom().computeUniformInteger(min, max);

        return number;
    }

    void Game::doUpdate( gf::Time time )
    {
        processPackets();
    }

    void Game::processPackets()
    {
        Packet packet;
        while ( m_ComQueue.poll(packet) )
        {
            switch ( packet.type )
            {
                case PacketType::RequestMove:
                {
                    Player *player = getPlayer( packet.requestMove.playerID );
                    if ( player != nullptr )
                    {
                        bool moved = player->applyMove( packet.requestMove.dir );

                        if ( moved )
                        {
                            Packet sendPacket;
                            sendPacket.type = PacketType::ReceiveMove;
                            sendPacket.receiveMove.playerID = packet.requestMove.playerID;
                            sendPacket.receiveMove.posX = player->m_Pos[0];
                            sendPacket.receiveMove.posY = player->m_Pos[1];

                            sendPacketToAllPlayers( sendPacket );
                        }
                    }
                    break;
                }
            }
        }
    }

    void Game::sendPacketToAllPlayers( Packet &packet )
    {
        auto it = m_Players.begin();
 
        // Iterate over the map using Iterator till end.
        while ( it != m_Players.end() )
        {
            it->second.sendPacket( packet );

            ++it;
        }
    }

    Player* Game::getPlayer( gf::Id playerID )
    {
        auto player = m_Players.find( playerID );

        if ( player != m_Players.end() )
        {
            return &player->second;
        }

        return nullptr;
    }
}