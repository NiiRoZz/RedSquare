#ifndef REDSQUARE_SERVER_GAME_H
#define REDSQUARE_SERVER_GAME_H

#include "../common/Sockets.h"
#include "../common/Packet.h"
#include "Player.h"
#include "Monster.h"
#include "Prop.h"
#include "ItemHolder.h"
#include "World.h"

#include <gf/Queue.h>

namespace redsquare
{
    class Game
    {
    public:
        std::map<gf::Id, Player> m_Players;
        std::map<gf::Id, Monster> m_Monsters;
        std::map<gf::Id, Prop> m_Props;
        std::map<gf::Id, ItemHolder> m_ItemHolders;

        World m_World;

        uint m_Floor;

        Game();

        gf::Id addNewPlayer(SocketTcp socket);
        void addNewMonsters(int nbMonster);

        void processPackets( Packet &packet );

        Player* getPlayer( gf::Id playerID );
        Player* getPlayer( gf::Vector2i pos ); // getter of player pos

        Monster* getMonster( gf::Id monsterID );
        Monster* getMonster( gf::Vector2i pos ); // getter of monster pos

        Prop* getProp( gf::Id propID );
        Prop* getProp( gf::Vector2i pos ); // getter of prop pos

        ItemHolder* getItemHolder( gf::Id itemHolderID );
        ItemHolder* getItemHolder( gf::Vector2i pos ); // getter of itemHolder pos

        void sendPacketToAllPlayers( Packet &packet );
        void sendPacketToVisiblePlayers( Packet &packet, gf::Vector2i pos );
        bool canAttack(Monster &monster, gf::Vector2i targetPos);
        void placeProps();
        void getSpawnPoint();
        void putStair();

    private:
        gf::Id generateId() const;

        gf::Queue<Packet> m_ComQueue;

        int m_PlayerSpawned;
    };
}

#endif //REDSQUARE_SERVER_GAME_H