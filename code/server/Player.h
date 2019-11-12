#ifndef REDSQUARE_SERVER_PLAYER_H
#define REDSQUARE_SERVER_PLAYER_H

#include "../common/Sockets.h"
#include "ServerEntity.h"
#include "World.h"
#include "Monster.h"
#include "../common/Packet.h"

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
        EntityClass m_Class;
        std::vector<SpellType> m_SpellTab;

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

        void playerSpawn(World &world, int playerSpawned);

    private:
        SocketTcp m_Socket;
        void BasicAttack(ServerEntity *target);
        void Fireball(ServerEntity *target);
        void ArmorUp();
        void DoubleStrike(ServerEntity *target);
        void Heal();
        void Assasinate(ServerEntity *target);
        void DamageUp();
        void Protection(ServerEntity *target);
        void Revenge(ServerEntity *target);
        void Lacerate(ServerEntity *target);
        void Incinerate(ServerEntity *target);
        void Devastate(ServerEntity *target, int zone);
        void Massacre(ServerEntity *target);
        void Impact(ServerEntity *target,gf::SquareMap m_SquareWorld);
        void LightningStrike(ServerEntity *target, int zone);
        void Scorch(ServerEntity *target);
        void Berserk();
        void Cleanse();
        void Mirror();
        void Stun(ServerEntity *target);
        void Warp(gf::Vector2i dash);
        void Reincarnate(ServerEntity *target);
        void Shoot(ServerEntity *target);
        void Backstab(ServerEntity *target);
        void Energize();
        void Torpedo(ServerEntity *target);
        void SoulLink(ServerEntity *target);

    };
}

#endif
