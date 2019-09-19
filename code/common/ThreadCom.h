#ifndef REDSQUARE_COMMON_THREADCOM_H
#define REDSQUARE_COMMON_THREADCOM_H

#include <functional>
#include <gf/Id.h>
#include <gf/Queue.h>

#include "Packet.h"
#include "Sockets.h"

namespace redsquare
{
  class ThreadCom
  {
  public:
    ThreadCom(SocketTcp socket, gf::Queue<Packet> &queue);
    ThreadCom(char* hostname, char *port, gf::Queue<Packet> &queue);

    void setQueue(gf::Queue<Packet>* queue);

    void start();
    bool sendPacket(Packet &packet);
    bool receivePacket(Packet &packet);
    void receivePackets();

    bool socketWorking();

  private:
    SocketTcp m_socket;
    gf::Queue<Packet>* m_queue;
  };
}

#endif // REDSQUARE_COMMON_THREADCOM_H