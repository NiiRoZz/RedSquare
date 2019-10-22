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
        const uint8_t m_TypeOfPlayer;
        
        Player(SocketTcp socket, gf::Id playerID, gf::Vector2i pos );

        gf::Id getID() const;

        void sendPacket(Packet &packet);
        void receivePacket(Packet &packet);

        void sendPacket(NewPlayer &packet);

        bool applyMove( int dirX, int dirY, gf::SquareMap &map );

        bool playerDisconnected() const;
        void spawn();

        int getLife() const;
        int getMana() const;

        bool canAttack(gf::Vector2i m_TargetPos);
        void attack();

    private:
        SocketTcp m_Socket;
        gf::Id m_PlayerID;

        int m_Life;
        int m_Mana;
    };
}

#endif
