#ifndef REDSQUARE_REDSQUARE_INSTANCE_H
#define REDSQUARE_REDSQUARE_INSTANCE_H

#include <gf/Random.h>

#include "GameInstance.h"
#include "World.h"
#include "Player.h"
#include "Monster.h"
#include "Prop.h"
#include "ItemHolder.h"

namespace redsquare
{
    class RedsquareInstance : public GameInstance
    {
    public:
        RedsquareInstance();

        void start() override;

        bool isFinished() override;

        void update(ServerPlayer& player, ProtocolBytes& bytes) override;

        Player* getPlayer( gf::Id playerID );
        Player* getPlayer( gf::Vector2i pos ); // getter of player pos

        Monster* getMonster( gf::Id monsterID );
        Monster* getMonster( gf::Vector2i pos ); // getter of monster pos

        Prop* getProp( gf::Id propID );
        Prop* getProp( gf::Vector2i pos ); // getter of prop pos

        ItemHolder* getItemHolder( gf::Id itemHolderID );
        ItemHolder* getItemHolder( gf::Vector2i pos ); // getter of itemHolder pos

    public:
        World m_World;

        std::map<gf::Id, Player> m_Players;
        std::map<gf::Id, Monster> m_Monsters;
        std::map<gf::Id, Prop> m_Props;
        std::map<gf::Id, ItemHolder> m_ItemHolders;

    private:
        void generateWorld(bool boss = false);

        void addNewMonsters(int nbMonster);
        void placeProps();

        bool canAttack(Monster &monster, gf::Vector2i targetPos);

        void goNextTurn();

        void fillChest();

        void doRemovePlayer(ServerPlayer& player) override;

    private:
        gf::Random m_Random;
        int32_t m_CountPlayers;
        uint m_PlayerIndexTurn;
        uint m_Floor;
    };
}

#endif // REDSQUARE_REDSQUARE_INSTANCE_H
