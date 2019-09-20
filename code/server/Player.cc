#include "Player.h"
#include "../common/World.h"

namespace redsquare
{
    Player::Player(SocketTcp socket, gf::Queue<Packet> &queue, gf::Id playerID)
    : m_Com(std::move(socket), queue)
    , m_PlayerID(playerID)
    , m_Pos(0,0)
    {
    }

    void Player::sendPacket(Packet &packet)
    {
        bool ok = m_Com.sendPacket(packet);
        assert(ok);
        if (!ok)
        throw std::runtime_error("!ok");
    }

    void Player::receivePacket(Packet &packet)
    {
        bool ok = m_Com.receivePacket(packet);
        assert(ok);
    }

    void Player::initialize()
    {
        std::thread([this]()
        {
            m_Com.start();
        }).detach();
    }

    void Player::setPos( gf::Vector2i pos )
    {
        m_Pos = pos;
    }

    gf::Vector2i Player::getPos()
    {
        return m_Pos;
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
                }
                break;
            }

            case MoveDirection::Left:
            {
                int newPosX = m_Pos[0] - 1;
                if ( newPosX >= 0 && newPosX < World::MapSize )
                {
                    m_Pos[0] = newPosX;
                }
                break;
            }

            case MoveDirection::Up:
            {
                int newPosY = m_Pos[1] - 1;
                if ( newPosY >= 0 && newPosY < World::MapSize )
                {
                    m_Pos[1] = newPosY;
                }
                break;
            }

            case MoveDirection::Down:
            {
                int newPosY = m_Pos[1] + 1;
                if ( newPosY >= 0 && newPosY < World::MapSize )
                {
                    m_Pos[1] = newPosY;
                }
                break;
            }
        }
    }

    bool Player::playerDisconnected()
    {
        return !m_Com.socketWorking();
    }
}