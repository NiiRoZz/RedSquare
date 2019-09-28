#include "Player.h"
#include "../common/World.h"
#include <iostream>

namespace redsquare
{
    Player::Player(SocketTcp socket, gf::Id playerID)
    : m_Socket(std::move(socket))
    , m_PlayerID(playerID)
    , m_Pos(0,0)
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

    bool Player::applyMove( MoveDirection dir )
    {
        switch ( dir )
        {
            case MoveDirection::Right:
            {
                int newPosX = m_Pos[0] + 1;
                if ( newPosX >= 0 && newPosX < World::MapSize )
                {
                    m_Pos[0] = newPosX;
                    return true;
                }
                else
                {
                    return false;
                }

                break;
            }

            case MoveDirection::Left:
            {
                int newPosX = m_Pos[0] - 1;
                if ( newPosX >= 0 && newPosX < World::MapSize )
                {
                    m_Pos[0] = newPosX;
                    return true;
                }
                else
                {
                    return false;
                }

                break;
            }

            case MoveDirection::Up:
            {
                int newPosY = m_Pos[1] - 1;
                if ( newPosY >= 0 && newPosY < World::MapSize )
                {
                    m_Pos[1] = newPosY;
                    return true;
                }
                else
                {
                    return false;
                }

                break;
            }

            case MoveDirection::Down:
            {
                int newPosY = m_Pos[1] + 1;
                if ( newPosY >= 0 && newPosY < World::MapSize )
                {
                    m_Pos[1] = newPosY;
                    return true;
                }
                else
                {
                    return false;
                }
                
                break;
            }
        }

        return false;
    }

    bool Player::playerDisconnected() const
    {
        return !(m_Socket.getState() == SocketState::Disconnected);
    }
}