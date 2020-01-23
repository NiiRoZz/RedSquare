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
}