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
        boost::ptr_map<gf::Id, Square>::iterator it = m_Squares.begin();
 
        // Iterate over the map using Iterator till end.
        while (it != m_Squares.end())
        {
            it->second->render( target, states );

            ++it;
        }

        m_World.render( target, states );
    }

    void Game::update(gf::Time time)
    {
        //Checking if we have received a message from server
        processPackets();

        //We update only the player we are controlling
        Square* player = getPlayer( m_SquareID );

        if ( player != NULL )
        {
            player->update( time );

            //Player want to move
            if ( m_DirMoving != MoveDirection::Nothing )
            {
                Packet packet;
                packet.type = PacketType::RequestMove;
                packet.requestMove.playerID = m_SquareID;
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
                    m_SquareID = packet.newPlayer.playerID;
                    Square *myNewPlayer = (Square *) malloc( sizeof(Square) );
                    new ((void*)myNewPlayer) Square();

                    std::cout << "Game::processPackets 3" << std::endl;

                    m_Squares.insert( m_SquareID, myNewPlayer );
                    std::cout << "Game::processPackets 4" << std::endl;
                    break;
                }
                
                case PacketType::ReceiveMove:
                {
                    Square* player = getPlayer( packet.receiveMove.playerID );
                    if ( player != NULL )
                    {
                        player->setPos( gf::Vector2i( packet.receiveMove.posX, packet.receiveMove.posY ) );
                    }
                    else
                    {
                        gf::Id newPlayerID = packet.receiveMove.playerID;
                        Square *newPlayer = (Square *) malloc( sizeof(Square) );
                        new ((void*)newPlayer) Square();
                        newPlayer->setPos( gf::Vector2i( packet.receiveMove.posX, packet.receiveMove.posY ) );

                        m_Squares.insert( newPlayerID, newPlayer );
                    }
                    break;
                }

                case PacketType::PlayerDisconnected:
                {
                    Square* player = getPlayer( packet.playerDisconnected.playerID );
                    if ( player != NULL )
                    {
                        m_Squares.erase( packet.playerDisconnected.playerID );
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

    Square* Game::getPlayer( gf::Id playerID )
    {
        auto square = m_Squares.find( playerID );

        if ( square != m_Squares.end() )
        {
            return square->second;
        }

        return NULL;
    }
}