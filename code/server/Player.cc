#include "Player.h"
#include "World.h"
#include <iostream>

namespace redsquare
{
    Player::Player(SocketTcp socket, gf::Id playerID)
    : m_Socket(std::move(socket))
    , m_PlayerID(playerID)
    , m_Pos(1,1)
    {
    }

    void Player::sendPacket(Packet &packet)
    {
        m_Socket.send(packet);
    }

    void Player::receivePacket(Packet &packet)
    {
        m_Socket.receive(packet);
    }

    void Player::sendPacket( NewPlayer &packet )
    {
        m_Socket.send(packet);
    }

    bool Player::applyMove(int dirX, int dirY, gf::SquareMap &map )
    {
        //MouseClic
        if(dirX !=0 && dirY !=0)
        {
            printf("actual position x: %d\nactual position y: %d\n",m_Pos[0] ,m_Pos[1]);
            int newPosX =  dirX ;
            int newPosY =  dirY;
            
            
            if ( newPosY >= 0 && newPosY < World::MapSize-1 && newPosX >= 0 && newPosX < World::MapSize-1/*&& map.isWalkable( {m_Pos[0], newPosY} )*/ )
            {
                m_Pos[1] = newPosY;
                m_Pos[0] = newPosX;
                return true;
            }
            else
            {
                return false;
            }
        }
        //
        else
        {
            
            int newPosY = m_Pos[1] + dirY;
            int newPosX = m_Pos[0] + dirX;
            if ( newPosY >= 0 && newPosY < World::MapSize-1 && newPosX >= 0 && newPosX < World::MapSize-1/*&& map.isWalkable( {m_Pos[0], newPosY} )*/ )
            {
                m_Pos[1] = newPosY;
                m_Pos[0] = newPosX;
                return true;
            }
            else
            {
                return false;
            }
        }

        return false;
    }

    bool Player::playerDisconnected() const
    {
        return (m_Socket.getState() == SocketState::Disconnected);
    }
}
