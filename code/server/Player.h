#ifndef REDSQUARE_SERVER_PLAYER_H
#define REDSQUARE_SERVER_PLAYER_H

#include "../common/Sockets.h"
#include "World.h"

#include <gf/Queue.h>
#include <gf/Id.h>
#include <gf/Map.h>
namespace redsquare
{
    class Player
    {
    public:
        gf::Vector2i m_Pos;
        
        Player(SocketTcp socket, gf::Id playerID);

        gf::Id getID() const;

        void sendPacket(Packet &packet);
        void receivePacket(Packet &packet);

        void sendPacket(NewPlayer &packet);

        bool applyMove( int dirX, int dirY, gf::SquareMap &map );

        bool playerDisconnected() const;

    private:
        SocketTcp m_Socket;
        gf::Id m_PlayerID;
    };
}

#endif
