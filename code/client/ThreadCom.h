#ifndef REDSQUARE_CLIENT_THREADCOM_H
#define REDSQUARE_CLIENT_THREADCOM_H

#include <functional>
#include <gf/Id.h>
#include <gf/Queue.h>
#include <thread>

#include "../common/Packet.h"
#include "../common/Sockets.h"

namespace redsquare
{
    template<typename T>
    class ThreadCom
    {
    public:
        ThreadCom(gf::Queue<T> &queue)
        : m_queue(&queue)
        {

        }

        ThreadCom(SocketTcp socket, gf::Queue<T> &queue)
        : m_socket(std::move(socket))
        , m_queue(&queue)
        {

        }

        ThreadCom(const char* hostname, const char *port, gf::Queue<T> &queue)
        : m_queue(&queue)
        {
            m_socket.connectTo(hostname, port);
        }

        SocketTcp& getSocket()
        {
            return m_socket;
        }

        void start()
        {
            std::thread(&ThreadCom::receivePackets, this).detach();
        }

        template<typename U>
        bool sendPacket(U &packet)
        {
            m_socket.send(packet);

            return !(m_socket.getState() == SocketState::Disconnected);
        }

        template<typename U>
        bool receivePacket(U &packet)
        {
            m_socket.receive(packet);

            return !(m_socket.getState() == SocketState::Disconnected);
        }

        void receivePackets()
        {
            for(;;)
            {
                T packet;

                if (!receivePacket(packet))
                {
                    break;
                }

                m_queue->push(packet);
            }
        }

        bool socketWorking() const
        {
            return !(m_socket.getState() == SocketState::Disconnected);
        }

    private:
        SocketTcp m_socket;
        gf::Queue<T>* m_queue;
    };

    template<> inline void ThreadCom<Packet>::receivePackets()
    {
        for(;;)
        {
            Packet packet;

            if (!receivePacket(packet))
            {
                break;
            }

            m_queue->push(packet);
            
            if (packet.type == PacketType::NewMap)
            {
                break;
            }
        }
    }
}

#endif // REDSQUARE_COMMON_THREADCOM_H
