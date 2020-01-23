#include "Sockets.h"

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <stdexcept>
#include <thread>

#include <gf/Log.h>
#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

#include <iostream>

namespace redsquare
{
    SocketTcp::SocketTcp()
    : m_Socket(m_IoService)
    , m_State(SocketState::Disconnected)
    {
    }

    SocketTcp::SocketTcp(boost::asio::ip::tcp::socket socket)
    : m_Socket(std::move(socket))
    , m_State(SocketState::Connected)
    {

    }

    SocketTcp::SocketTcp(SocketTcp&& other)
    : m_Socket(std::move(other.m_Socket))
    , m_State(other.m_State)
    {

    }

    SocketTcp& SocketTcp::operator=(SocketTcp&& other)
    {
        m_Socket = std::move(other.m_Socket);
        m_State = other.m_State;

        return *this;
    }

    SocketState SocketTcp::getState() const
    {
        return m_State;
    }

    void SocketTcp::connectTo(const char* server, const char* port)
    {
        tcp::resolver resolver(m_IoService);
        boost::asio::connect(m_Socket, resolver.resolve({ server, port }));
        m_State = SocketState::Connected;
    }

    void SocketTcp::send(Packet &packet)
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

    void SocketTcp::receive(Packet &packet)
    {
        if (m_State == SocketState::Disconnected)
        {
            return;
        }

        constexpr int MaxLength = 1024;
        uint8_t data[MaxLength];

        boost::system::error_code error;
        //size_t length = m_Socket.read_some(boost::asio::buffer(data), error);
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

    void SocketTcp::send(NewPlayer &packet)
    {
        if (m_State == SocketState::Disconnected)
        {
            return;
        }

        constexpr int MaxLength = 1024 * 1024;
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

    void SocketTcp::receive(NewPlayer &packet)
    {
        if (m_State == SocketState::Disconnected)
        {
            return;
        }

        constexpr int MaxLength = 1024 * 1024;
        uint8_t data[MaxLength];

        boost::system::error_code error;
        //m_Socket.read_some(boost::asio::buffer(data), error);
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


}