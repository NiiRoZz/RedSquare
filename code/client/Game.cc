#include "Game.h"

#include <iostream>
#include <utility>

namespace redsquare
{
    Game::Game( char* hostname, char *port, gf::ExtendView &view )
    : m_ThreadCom(hostname, port, m_ComQueue)
    , m_View(&view)
    , m_CanPlay( false )
    , m_DirMoving( MoveDirection::Nothing )
    {
    }

    void Game::startThreadCom()
    {
        m_ThreadCom.start();
    }

    void Game::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        m_World.render( target, states );
        
        auto it = m_Players.begin();
 
        // Iterate over the map using Iterator till end.
        while (it != m_Players.end())
        {
            it->second.render( target, states );

            ++it;
        }
    }

    void Game::update(gf::Time time)
    {
        //Checking if we have received a message from server
        processPackets();

        //We update only the player we are controlling
        Player* player = getPlayer( m_PlayerID );

        m_World.update( time );

        if ( player != nullptr )
        {
            player->update( time );

            m_View->setCenter( { player->m_Pos[0] * World::TileSize, player->m_Pos[1] * World::TileSize } );

            //Do all actions stuff here
            doAction();
        }
    }

    void Game::doAction()
    {
        if ( !m_CanPlay )
        {
            return;
        }

        //Player want to move
        if ( m_DirMoving != MoveDirection::Nothing )
        {
            Packet packet;
            packet.type = PacketType::RequestMove;
            packet.requestMove.playerID = m_PlayerID;
            packet.requestMove.dir = m_DirMoving;

            m_ThreadCom.sendPacket( packet );

            m_DirMoving = MoveDirection::Nothing;

            //don't forget to call m_CanPlay false when sent action
            m_CanPlay = false;
        }
        //else if ( )
    }

    void Game::processPackets()
    {
        Packet packet;
        while ( m_ComQueue.poll(packet) )
        {
            switch (packet.type)
            {
                case PacketType::NewPlayer:
                {
                    m_PlayerID = packet.newPlayer.playerID;

                    m_Players.insert( std::make_pair( m_PlayerID, Player() ) );
                    break;
                }
                
                case PacketType::ReceiveMove:
                {
                    Player* player = getPlayer( packet.receiveMove.playerID );
                    if ( player != nullptr )
                    {
                        player->m_Pos[0] = packet.receiveMove.posX;
                        player->m_Pos[1] = packet.receiveMove.posY;
                    }
                    else
                    {
                        gf::Id newPlayerID = packet.receiveMove.playerID;

                        auto it = m_Players.insert( std::make_pair( newPlayerID, Player( gf::Vector2i(packet.receiveMove.posX, packet.receiveMove.posY) ) ) );
                        assert( it.second );
                    }
                    break;
                }

                case PacketType::PlayerDisconnected:
                {
                    Player* player = getPlayer( packet.playerDisconnected.playerID );
                    if ( player != nullptr )
                    {
                        m_Players.erase( packet.playerDisconnected.playerID );
                    }
                    break;
                }

                case PacketType::PlayerTurn:
                {
                    m_CanPlay = packet.playerTurn.playerTurn;
                    std::cout << "It's your turn!!!" << std::endl;
                    break;
                }
            }
        }
    }

    void Game::movePlayer( MoveDirection dir )
    {
        if ( !m_CanPlay )
        {
            return;
        }
        
        m_DirMoving = dir;
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