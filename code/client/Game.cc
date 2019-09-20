#include "Game.h"

#include <iostream>

namespace redsquare
{
    Game::Game( char* hostname, char *port )
    : m_ThreadCom(hostname, port, m_ComQueue)
    {
        m_DirMoving = MoveDirection::Nothing;
    }

    void Game::startThreadCom()
    {
        m_ThreadCom.start();
    }

    void Game::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        m_World.render( target, states );
        
        boost::ptr_map<gf::Id, Player>::iterator it = m_Players.begin();
 
        // Iterate over the map using Iterator till end.
        while (it != m_Players.end())
        {
            it->second->render( target, states );

            ++it;
        }
    }

    void Game::update(gf::Time time)
    {
        //Checking if we have received a message from server
        processPackets();

        //We update only the player we are controlling
        Player* player = getPlayer( m_PlayerID );

        if ( player != NULL )
        {
            player->update( time );

            //Player want to move
            if ( m_DirMoving != MoveDirection::Nothing )
            {
                Packet packet;
                packet.type = PacketType::RequestMove;
                packet.requestMove.playerID = m_PlayerID;
                packet.requestMove.dir = m_DirMoving;

                m_ThreadCom.sendPacket( packet );

                m_DirMoving = MoveDirection::Nothing;
            }
        }

        m_World.update( time );
    }

    void Game::processPackets()
    {
        Packet packet;
        while (m_ComQueue.poll(packet))
        {
            std::cout << "Game::processPackets 1" << std::endl;
            switch (packet.type)
            {
                case PacketType::NewPlayer:
                {
                    std::cout << "Game::processPackets 2" << std::endl;
                    m_PlayerID = packet.newPlayer.playerID;
                    Player *myNewPlayer = (Player *) malloc( sizeof(Player) );
                    new ((void*)myNewPlayer) Player();

                    std::cout << "Game::processPackets 3" << std::endl;

                    m_Players.insert( m_PlayerID, myNewPlayer );
                    std::cout << "Game::processPackets 4" << std::endl;
                    break;
                }
                
                case PacketType::ReceiveMove:
                {
                    Player* player = getPlayer( packet.receiveMove.playerID );
                    if ( player != NULL )
                    {
                        player->setPos( gf::Vector2i( packet.receiveMove.posX, packet.receiveMove.posY ) );
                    }
                    else
                    {
                        gf::Id newPlayerID = packet.receiveMove.playerID;
                        Player *newPlayer = (Player *) malloc( sizeof(Player) );
                        new ((void*)newPlayer) Player();
                        newPlayer->setPos( gf::Vector2i( packet.receiveMove.posX, packet.receiveMove.posY ) );

                        m_Players.insert( newPlayerID, newPlayer );
                    }
                    break;
                }

                case PacketType::PlayerDisconnected:
                {
                    Player* player = getPlayer( packet.playerDisconnected.playerID );
                    if ( player != NULL )
                    {
                        m_Players.erase( packet.playerDisconnected.playerID );
                    }
                    break;
                }
            }
        }
    }

    void Game::movePlayer( MoveDirection dir )
    {
        m_DirMoving = dir;
    }

    Player* Game::getPlayer( gf::Id playerID )
    {
        auto player = m_Players.find( playerID );

        if ( player != m_Players.end() )
        {
            return player->second;
        }

        return NULL;
    }
}