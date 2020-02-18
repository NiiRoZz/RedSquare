#include "ClientNetwork.h"

#include <cstring>
#include <stdexcept>
#include <thread>

#include <gf/Log.h>
#include <gf/SerializationOps.h>

#include "../common/Protocol.h"

namespace redsquare
{
    ClientNetwork::ClientNetwork()
    : m_Connecting(false)
    {

    }

    bool ClientNetwork::isConnecting()
    {
        return m_Connecting;
    }

    bool ClientNetwork::isConnected()
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        return static_cast<bool>(m_Socket);
    }

    void ClientNetwork::connect(const std::string& hostname)
    {
        m_Connecting = true;

        std::thread thread(&ClientNetwork::run, this, hostname);
        thread.detach();
    }

    void ClientNetwork::disconnect()
    {
        ClientDisconnect data;
        send(data);
        m_Socket = gf::TcpSocket();
        m_Connecting = false;
    }

    void ClientNetwork::run(std::string hostname)
    {
        gf::TcpSocket socket(hostname, Service);

        if (!socket)
        {
            m_Connecting = false;
            return;
        }

        {
            std::lock_guard<std::mutex> guard(m_Mutex);
            m_Socket = std::move(socket);
        }

        for (;;)
        {
            gf::Packet bytes;

            switch (m_Socket.recvPacket(bytes))
            {
                case gf::SocketStatus::Data:
                    queue.push(std::move(bytes));
                    break;
                
                case gf::SocketStatus::Error:
                    gf::Log::error("Error while receiving a packet from server\n");
                    return;
                
                case gf::SocketStatus::Close:
                    gf::Log::info("End of connection to the server\n");
                    return;

                case gf::SocketStatus::Block:
                    assert(false);
                    break;
            }
        }
    }

}
