#ifndef REDSQUARE_SERVER_PLAYER_H
#define REDSQUARE_SERVER_PLAYER_H

#include "../common/Sockets.h"
#include "ServerEntity.h"
#include "World.h"
#include "Monster.h"

#include <gf/Queue.h>
#include <gf/Id.h>
#include <gf/Map.h>

namespace redsquare
{
    class Player: public ServerEntity
    {
    public:
        /* Characteristic */
        int m_ManaPoint;
        int m_MaxManaPoint;

        int m_XP;
        int m_MaxXP;
        /* Characteristic*/

        uint8_t m_PointInRound;
        bool m_MovedInRound;

        virtual void createCarPacket(Packet &packet) override;
        
        Player(SocketTcp socket, gf::Id playerID, const EntityClass type);

        void sendPacket(Packet &packet);
        void receivePacket(Packet &packet);

        void sendPacket(NewPlayer &packet);

        bool applyMove( int dirX, int dirY,World &world );

        bool playerDisconnected() const;
        void spawn();

        bool canAttack(gf::Vector2i m_TargetPos);
        void attack(ServerEntity *target);
        
        void levelUp();

        void playerSpawn(std::map<gf::Id,Player> &m_Players,World &world);

    private:
        SocketTcp m_Socket;
    };
}

#endif
