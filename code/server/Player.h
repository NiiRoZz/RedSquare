#ifndef REDSQUARE_SERVER_PLAYER_H
#define REDSQUARE_SERVER_PLAYER_H

#include "../common/Sockets.h"
#include "../common/ThreadCom.h"

#include <gf/Queue.h>
#include <gf/Id.h>

namespace redsquare
{
    class Player
    {
    public:
        gf::Vector2i m_Pos;
        
        Player(SocketTcp socket, gf::Queue<Packet> &queue, gf::Id playerID);

        gf::Id getID() const;

        void sendPacket(Packet &packet);
        void receivePacket(Packet &packet);

        void initialize();

        bool applyMove( MoveDirection dir );

        bool playerDisconnected();

    private:
        ThreadCom m_Com;
        gf::Id m_PlayerID;
    };
}

#endif