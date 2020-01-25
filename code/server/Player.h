#ifndef REDSQUARE_SERVER_PLAYER_H
#define REDSQUARE_SERVER_PLAYER_H

#include "../common/Sockets.h"
#include "ServerEntity.h"
#include "World.h"
#include "Monster.h"
#include "../common/Packet.h"
#include "Inventory.h"

#include <gf/Queue.h>
#include <gf/Id.h>
#include <gf/Map.h>

namespace redsquare
{
    // forward class
    class Game;

    class Player: public ServerEntity
    {
    public:
        /* Characteristic */
        int m_ManaPoint;
        int m_MaxManaPoint;

        int m_XP;
        int m_MaxXP;
        /* Characteristic*/
        EntitySubType m_Class;
        SpellType m_CurrentSpell;
        std::vector<SpellType> m_SpellTab;

        uint8_t m_PointInRound;
        bool m_MovedInRound;

        virtual void createCarPacket(Packet &packet) override;
        
        Player(SocketTcp socket, gf::Id playerID, const EntitySubType entitySubType);

        void sendPacket(Packet &packet);
        void receivePacket(Packet &packet);

        void sendPacket(NewPlayer &packet);

        bool applyMove( int dirX, int dirY,World &world );

        bool playerDisconnected() const;
        void spawn();

        void attack(SpellType spellType, ServerEntity *target);
        std::vector<Monster*> attack(SpellType spellType, ServerEntity *target, std::map<gf::Id, Monster> &monsters);
        
        void levelUp();

        void playerSpawn(World &world, int playerSpawned);

        void sendUpdateOfSpells();

        virtual void defaultInventoryStuff() override;
        void UseItem(ItemType type);

    private:
        void BasicAttack(ServerEntity *target); // DONE
        void Fireball(ServerEntity *target); // DONE
        void RangeUp(); // DONE
        void ArmorUp(); // DONE
        void DoubleStrike(ServerEntity *target); // DONE
        void Heal(ServerEntity *target); // DONE
        void Assasinate(ServerEntity *target); // DONE
        void DamageUp(); // DONE
        void Protection(ServerEntity *target); // DONE
        void Revenge(ServerEntity *target); // DONE
        void Lacerate(ServerEntity *target); // DONE
        void Incinerate(ServerEntity *target); // DONE
        void Devastate(ServerEntity *target, int zone);
        void Massacre(ServerEntity *target); // DONE
        void Scorch(ServerEntity *target); // DONE
        void Berserk(); // DONE
        void Shoot(ServerEntity *target); // DONE
        void Backstab(ServerEntity *target); // DONE
        void Torpedo(ServerEntity *target); // DONE
        std::vector<Monster*> Reaper(ServerEntity *target,std::map<gf::Id, Monster> &monsters); // DONE
        std::vector<Monster*> LightningStrike(ServerEntity *target,std::map<gf::Id, Monster> &monsters); // need to fix smthg
        int Variance(int range); // DONE
        void ManaPot(int ratio);
        void HealthPot(int ratio);
        void EnergyPot(int ratio);
        void BoostAttack(int ratio);
        void BoostDefense(int ratio);
        void BoostXP(int ratio);

        SocketTcp m_Socket;
    };
}

#endif
