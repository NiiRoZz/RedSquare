#ifndef REDSQUARE_SERVER_SQUARE_H
#define REDSQUARE_SERVER_SQUARE_H

#include "../common/Sockets.h"
#include "../common/ThreadCom.h"

#include <gf/Queue.h>
#include <gf/Id.h>

namespace redsquare
{
    class Square
    {
    public:
        Square(SocketTcp socket, gf::Queue<Packet> &queue, gf::Id playerID);

        gf::Id getID() const;

        void sendPacket(Packet &packet);
        void receivePacket(Packet &packet);

        void initialize();

        void setPos( gf::Vector2i pos );
        gf::Vector2i getPos();

        bool applyMove( MoveDirection dir );

        bool playerDisconnected();

    private:
        ThreadCom m_Com;
        gf::Id m_PlayerID;
        gf::Vector2i m_Pos;
    };
}

#endif