#ifndef REDSQUARE_COMMON_SOCKETS_H
#define REDSQUARE_COMMON_SOCKETS_H

#include "Packet.h"

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

    void send(Packet &packet);
    void receive(Packet &packet);

  private:
    boost::asio::io_service m_IoService;
    boost::asio::ip::tcp::socket m_Socket;
    SocketState m_State;
  };

  class ListenerTcp
  {
    public:
    ListenerTcp(std::uint16_t port);

    ListenerTcp(const SocketTcp&) = delete;
    ListenerTcp& operator=(const SocketTcp&) = delete;

    ListenerTcp(SocketTcp&& other) = delete;
    ListenerTcp& operator=(SocketTcp&& other) = delete;

    void handleNewConnection(std::function<void(SocketTcp)> handler);

    private:
    boost::asio::io_service m_IoService;
    boost::asio::ip::tcp::acceptor m_Acceptor;
  };
}


#endif // REDSQUARE_COMMON_SOCKETS_H