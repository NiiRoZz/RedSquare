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
        
        Player(SocketTcp socket, gf::Id playerID, const EntitySubType type);

        void sendPacket(Packet &packet);
        void receivePacket(Packet &packet);

        void sendPacket(NewPlayer &packet);

        bool applyMove( int dirX, int dirY,World &world );

        bool playerDisconnected() const;
        void spawn();

        void attack(SpellType spellType, ServerEntity *target);
        
        void levelUp();

        void playerSpawn(World &world, int playerSpawned);

        void sendUpdateOfSpells();

        Inventory& getInventory();

        //Send update of the inventory to client.  pos is used only if slotType == Cargo
        void sendUpdateItem(InventorySlotType slotType, bool remove, uint pos = 0u);

    private:
        SocketTcp m_Socket;
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
        void Impact(ServerEntity *target,gf::SquareMap m_SquareWorld);
        void LightningStrike(ServerEntity *target, int zone);
        void Scorch(ServerEntity *target); // DONE
        void Berserk(); // DONE
        void Cleanse();
        void Mirror();
        void Stun(ServerEntity *target);
        void Warp(gf::Vector2i dash);
        void Shoot(ServerEntity *target); // DONE
        void Backstab(ServerEntity *target); // DONE
        void Energize();
        void Torpedo(ServerEntity *target); // DONE
        void SoulLink(ServerEntity *target);
        void Reaper(ServerEntity *target);
        int Variance(int range); // DONE

    };
}

#endif
