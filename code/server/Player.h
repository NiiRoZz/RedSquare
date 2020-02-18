#ifndef REDSQUARE_SERVER_PLAYER_H
#define REDSQUARE_SERVER_PLAYER_H

#include "ServerEntity.h"
#include "Monster.h"
#include "../common/ProtocolData.h"
#include "Inventory.h"

#include <gf/Queue.h>
#include <gf/Id.h>
#include <gf/Map.h>
#include <string>

namespace redsquare
{
    // forward class
    class Game;
    class RedsquareInstance;
    class World;

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

        bool m_PlayerTurn;
        
        Player(gf::Id playerID, const EntitySubType entitySubType, const std::string name, RedsquareInstance &instance);

        bool applyMove( gf::Vector2i dir, World &world );

        void spawn();

        void attack(SpellType spellType, ServerEntity *target);
        std::vector<Monster*> attack(SpellType spellType, ServerEntity *target, std::map<gf::Id, Monster> &monsters);
        
        void levelUp();

        void playerSpawn(World &world, int playerSpawned);

        void sendUpdateOfSpells();

        virtual void defaultInventoryStuff() override;
        void UseItem(ItemType type);

        void onMovedItem(ServerItem &item, bool remove);

        //void sendMessageToChat(std::string str);

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
        void ManaPot(float ratio);
        void HealthPot(float ratio);
        void EnergyPot(float ratio);
        void BoostAttack(float ratio);
        void BoostDefense(float ratio);
        void BoostMana(float ratio);
        void BoostHealth(float ratio);
        void BoostXP(float ratio);
        void createSystemMessage(std::string message, std::string to,std::string name);

    private:
        RedsquareInstance &m_RedsquareInstance;
        std::string m_Name;
    };
}

#endif
