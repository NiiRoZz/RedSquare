#ifndef REDSQUARE_COMMON_SOCKETS_H
#define REDSQUARE_COMMON_SOCKETS_H

#include "Packet.h"

#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace redsquare {
  enum class SocketState {
    Disconnected,
    Connected,
  };

  class SocketTcp {
    public:
        SocketTcp();
        SocketTcp(boost::asio::ip::tcp::socket socket);

        SocketTcp(const SocketTcp&) = delete;
        SocketTcp& operator=(const SocketTcp&) = delete;

        SocketTcp(SocketTcp&& other);
        SocketTcp& operator=(SocketTcp&& other);

        SocketState getState() const;

        void connectTo(const char* server, const char* port);
        void disconnect();

        template<typename T>
        void send(T &packet)
        {
            if (m_State == SocketState::Disconnected)
            {
                return;
            }

            constexpr int MaxLength = 1024;
            uint8_t data[MaxLength];

            gf::MemoryOutputStream stream(data);
            gf::Serializer ar(stream);

            ar | packet;

            boost::system::error_code error;
            boost::asio::write(m_Socket, boost::asio::buffer(data, MaxLength), error);

            if (error == boost::asio::error::eof || error == boost::asio::error::broken_pipe)
            {
                m_State = SocketState::Disconnected;
                return; // Connection closed by peer
            } else if (error)
            {
                throw boost::system::system_error(error);
            }
        }

        template<typename T>
        void receive(T &packet)
        {
            if (m_State == SocketState::Disconnected)
            {
                return;
            }

            constexpr int MaxLength = 1024;
            uint8_t data[MaxLength];

            boost::system::error_code error;
            size_t length = boost::asio::read(m_Socket, boost::asio::buffer(data), error);

            if (error == boost::asio::error::eof)
            {
                m_State = SocketState::Disconnected;
                return; // Connection closed cleanly by peer
            } else if (error)
            {
                throw boost::system::system_error(error);
            }

            gf::MemoryInputStream stream(gf::ArrayRef<uint8_t>(data, length));
            gf::Deserializer ar(stream);

            ar | packet;
        }

    private:
        boost::asio::io_service m_IoService;
        boost::asio::ip::tcp::socket m_Socket;
        SocketState m_State;
    };

    template<>
    void SocketTcp::send(NewPlayer &packet);

    template<>
    void SocketTcp::receive<NewPlayer>(NewPlayer &packet);
}


#endif // REDSQUARE_COMMON_SOCKETS_H