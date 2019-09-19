#include "ThreadCom.h"

namespace redsquare 
{

    ThreadCom::ThreadCom(SocketTcp socket, gf::Queue<Packet> &queue)
    : m_socket(std::move(socket))
    , m_queue(&queue)
    {

    }

    ThreadCom::ThreadCom(char* hostname, char *port, gf::Queue<Packet> &queue)
    : m_queue(&queue)
    {
        m_socket.connectTo(hostname, port);
    }

    void ThreadCom::setQueue(gf::Queue<Packet>* queue)
    {
        m_queue = queue;
    }

    void ThreadCom::start()
    {
        std::thread(&ThreadCom::receivePackets, this).detach();
    }

    bool ThreadCom::sendPacket(Packet &packet)
    {
        m_socket.send(packet);

        return !(m_socket.getState() == SocketState::Disconnected);
    }

    bool ThreadCom::receivePacket(Packet &packet)
    {
        m_socket.receive(packet);

        return !(m_socket.getState() == SocketState::Disconnected);
    }

    bool ThreadCom::socketWorking()
    {
        return !(m_socket.getState() == SocketState::Disconnected);
    }

    void ThreadCom::receivePackets()
    {
        for(;;) {
            Packet packet;

            if (!receivePacket(packet))
            {
                break;
            }

            m_queue->push(packet);
        }
    }
}
