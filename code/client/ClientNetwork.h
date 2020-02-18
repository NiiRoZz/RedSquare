#ifndef REDSQUARE_CLIENT_NETWORK_H
#define REDSQUARE_CLIENT_NETWORK_H

#include <atomic>
#include <mutex>
#include <string>

#include <gf/Queue.h>
#include <gf/TcpSocket.h>
#include <gf/Packet.h>

namespace redsquare
{
    class ClientNetwork
    {
    public:
        ClientNetwork();

        bool isConnecting();
        bool isConnected();
        void connect(const std::string& hostname);
        void disconnect();

        template<typename T>
        void send(const T& data)
        {
            gf::Packet bytes;
            bytes.is(data);
            m_Socket.sendPacket(bytes);
        }

    public:
        gf::Queue<gf::Packet> queue;

    private:
        void run(std::string hostname);

    private:
        gf::TcpSocket m_Socket;
        std::mutex m_Mutex;
        std::atomic_bool m_Connecting;
    };
}

#endif // PEM_CLIENT_NETWORK_H
