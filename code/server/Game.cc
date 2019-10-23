#include "Game.h"
#include "../common/Singletons.h"

#include <gf/Random.h>
#include <iostream>

namespace redsquare
{
    Game::Game()
    {
        std::cout << "Game::Game" << std::endl;
    }

    void Game::addNewPlayer(SocketTcp socket)
    {
        // Generate a new ID
        gf::Id id = generateId();
        std::map<gf::Id, Player>::iterator itNewPlayer;

        // Create a new player
        std::tie(itNewPlayer, std::ignore) = m_Players.emplace(id, Player(std::move(socket), id, m_World.getSpawnPoint()));

        // Send to the client his ID
        auto size = m_World.m_World.getSize();

        NewPlayer packetNewPlayer( m_World.m_World, id );
        itNewPlayer->second.sendPacket(packetNewPlayer);

        //HACKY, too, sending fake move to all other players INCLUDE HIMSELF!!! Should be reworked
        Packet packet;

        packet.type = PacketType::SpawnEntity;
        packet.spawnEntity.playerID = id;
        packet.spawnEntity.typeEntity = EntityType::Player;
        packet.spawnEntity.typeOfEntity = itNewPlayer->second.m_TypeOfPlayer;
        packet.spawnEntity.posX = itNewPlayer->second.m_Pos[0];
        packet.spawnEntity.posY = itNewPlayer->second.m_Pos[1];
        sendPacketToAllPlayers( packet );

        itNewPlayer->second.createCarPacket(packet);
        itNewPlayer->second.sendPacket(packet);

        //HACKY, find best way, fake a move of all players inside the game to make them apparear in the new client
        auto it = m_Players.begin();
        // Iterate over the map using Iterator till end.
        while (it != m_Players.end())
        {
            if ( it->first != id )
            {
                packet.type = PacketType::SpawnEntity;
                packet.spawnEntity.playerID = it->first;
                packet.spawnEntity.typeEntity = EntityType::Player;
                packet.spawnEntity.typeOfEntity = it->second.m_TypeOfPlayer;
                packet.spawnEntity.posX = it->second.m_Pos[0];
                packet.spawnEntity.posY = it->second.m_Pos[1];
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

    bool Game::processPackets( Packet &packet )
    {
        switch ( packet.type )
        {
            case PacketType::RequestMove:
            {
                Player *player = getPlayer( packet.requestMove.playerID );
                if ( player != nullptr )
                {
                    bool moved = player->applyMove( packet.requestMove.dirX,packet.requestMove.dirY, m_World.m_SquareWorld );

                    if ( moved )
                    {
                        Packet sendPacket;
                        sendPacket.type = PacketType::ReceiveMove;
                        sendPacket.receiveMove.playerID = packet.requestMove.playerID;
                        sendPacket.receiveMove.posX = player->m_Pos[0];
                        sendPacket.receiveMove.posY = player->m_Pos[1];

                        sendPacketToAllPlayers( sendPacket );
                    }

                    return moved;
                }
                break;
            }
        }

        return true;
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
