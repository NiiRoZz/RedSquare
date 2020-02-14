#ifndef REDSQUARE_SERVER_SERVER_PLAYER_H
#define REDSQUARE_SERVER_SERVER_PLAYER_H

#include <cstdint>
#include <string>

#include <gf/TcpSocket.h>

#include "../common/ProtocolBytes.h"

namespace redsquare
{
    class ServerRoom;

    struct ServerPlayer
    {
        gf::Id id = gf::InvalidId;
        std::string name;
        gf::TcpSocket socket;
        ServerRoom *room = nullptr;
        bool ready = false;

        template<typename T>
        void send(const T& data)
        {
            ProtocolBytes bytes;
            bytes.is(data);
            socket.sendPacket(bytes.packet);
        }
    };
}

#endif // REDSQUARE_SERVER_SERVER_PLAYER_H
