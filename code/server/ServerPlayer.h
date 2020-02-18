#ifndef REDSQUARE_SERVER_SERVER_PLAYER_H
#define REDSQUARE_SERVER_SERVER_PLAYER_H

#include <cstdint>
#include <string>

#include <gf/TcpSocket.h>
#include <gf/Packet.h>

#include "../common/RedsquareProtocol.h"

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
        EntitySubType subType = EntitySubType::Magus;

        template<typename T>
        void send(const T& data)
        {
            gf::Packet bytes;
            bytes.is(data);
            socket.sendPacket(bytes);
        }
    };
}

#endif // REDSQUARE_SERVER_SERVER_PLAYER_H
