#include "Square.h"
#include "../common/World.h"

namespace redsquare
{
    Square::Square(SocketTcp socket, gf::Queue<Packet> &queue, gf::Id playerID)
    : m_Com(std::move(socket), queue)
    , m_PlayerID(playerID)
    , m_Pos(0,0)
    {
    }

    void Square::sendPacket(Packet &packet)
    {
        bool ok = m_Com.sendPacket(packet);
        assert(ok);
        if (!ok)
        throw std::runtime_error("!ok");
    }

    void Square::receivePacket(Packet &packet)
    {
        bool ok = m_Com.receivePacket(packet);
        assert(ok);
    }

    void Square::initialize()
    {
        std::thread([this]()
        {
            m_Com.start();
        }).detach();
    }

    void Square::setPos( gf::Vector2i pos )
    {
        m_Pos = pos;
    }

    gf::Vector2i Square::getPos()
    {
        return m_Pos;
    }

    bool Square::applyMove( MoveDirection dir )
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

    bool Square::playerDisconnected()
    {
        return !m_Com.socketWorking();
    }
}