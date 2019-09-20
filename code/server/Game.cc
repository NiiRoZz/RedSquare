#include "Game.h"

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

        // Create a new player
        Square *newSquare = (Square *) malloc( sizeof(Square) );
        new ((void*)newSquare) Square(std::move(socket), m_ComQueue, id);
        newSquare->initialize();
        m_Squares.insert( id, newSquare );

        // Send to the client his ID
        Packet packet;
        packet.type = PacketType::NewPlayer;
        packet.newPlayer.playerID = id;
        newSquare->sendPacket(packet);

        //HACKY, too, sending fake move to all other players INCLUDE HIMSELF!!! Should be reworked
        packet.type = PacketType::ReceiveMove;
        packet.receiveMove.playerID = id;
        packet.receiveMove.posX = newSquare->getPos()[0];
        packet.receiveMove.posY = newSquare->getPos()[1];
        sendPacketToAllPlayers( packet );

        //HACKY, find best way, fake a move of all players inside the game to make them apparear in the new client
        boost::ptr_map<gf::Id, Square>::iterator it = m_Squares.begin();
        // Iterate over the map using Iterator till end.
        while (it != m_Squares.end())
        {
            if ( it->first != id )
            {
                packet.type = PacketType::ReceiveMove;
                packet.receiveMove.playerID = it->first;
                packet.receiveMove.posX = it->second->getPos()[0];
                packet.receiveMove.posY = it->second->getPos()[1];
                newSquare->sendPacket( packet );
            }

            ++it;
        }
    }

    gf::Id Game::generateId() const
    {
        // Get the ID
        gf::Random random;

        uint64_t min = std::numeric_limits<uint64_t>::min();
        uint64_t max = std::numeric_limits<uint64_t>::max();
        uint64_t number = random.computeUniformInteger(min, max);

        return number;
    }

    void Game::doUpdate( gf::Time time )
    {
        processPackets();

        detectDisonnection();
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
                    Square *player = getPlayer( packet.requestMove.playerID );
                    if ( player != NULL )
                    {
                        bool moved = player->applyMove( packet.requestMove.dir );

                        if ( moved )
                        {
                            Packet sendPacket;
                            sendPacket.type = PacketType::ReceiveMove;
                            sendPacket.receiveMove.playerID = packet.requestMove.playerID;
                            sendPacket.receiveMove.posX = player->getPos()[0];
                            sendPacket.receiveMove.posY = player->getPos()[1];

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
        boost::ptr_map<gf::Id, Square>::iterator it = m_Squares.begin();
 
        // Iterate over the map using Iterator till end.
        while ( it != m_Squares.end() )
        {
            it->second->sendPacket( packet );

            ++it;
        }
    }

    void Game::detectDisonnection()
    {
        boost::ptr_map<gf::Id, Square>::iterator it = m_Squares.begin();
 
        // Iterate over the map using Iterator till end.
        while ( it != m_Squares.end() )
        {
            if ( it->second->playerDisconnected() )
            {
                gf::Id disconnectID = it->first;
                //Should use free ?
                //free( it->second );

                m_Squares.erase( disconnectID );

                Packet sendPacket;
                sendPacket.type = PacketType::PlayerDisconnected;
                sendPacket.receiveMove.playerID = disconnectID;

                sendPacketToAllPlayers( sendPacket );
            }

            ++it;
        }
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