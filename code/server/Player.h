#ifndef REDSQUARE_SERVER_PLAYER_H
#define REDSQUARE_SERVER_PLAYER_H

#include "../common/Sockets.h"
#include "../common/Entity.h"
#include "World.h"

#include <gf/Queue.h>
#include <gf/Id.h>
#include <gf/Map.h>

namespace redsquare
{
    class Player: public Entity
    {
    public:
        /* Characteristic */
        int m_ManaPoint;
        int m_MaxManaPoint;

        int m_XP;
        int m_MaxXP;
        /* Characteristic*/

        void createCarPacket(Packet &packet);

        const uint8_t m_TypeOfPlayer;
        
        Player(SocketTcp socket, gf::Id playerID);

        void sendPacket(Packet &packet);
        void receivePacket(Packet &packet);

        void sendPacket(NewPlayer &packet);

        bool applyMove( int dirX, int dirY, gf::SquareMap &map );

        bool playerDisconnected() const;
        void spawn();

        bool canAttack(gf::Vector2i m_TargetPos);
        void attack();
        
        void levelUp();

        void playerSpawn(std::map<gf::Id,Player> &m_Players,World &world);

    private:
        SocketTcp m_Socket;
        gf::Id m_PlayerID;
    };
}

#endif
