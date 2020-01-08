#ifndef REDSQUARE_SERVER_GAME_H
#define REDSQUARE_SERVER_GAME_H

#include "../common/Sockets.h"
#include "../common/Packet.h"
#include "Player.h"
#include "Monster.h"
#include "Prop.h"
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

        World m_World;

        uint m_Floor;

        Game();

        void addNewPlayer(SocketTcp socket);
        void addNewMonsters(int nbMonster);

        void processPackets( Packet &packet );

        Player* getPlayer( gf::Id playerID );
        Player* getPlayer( gf::Vector2i pos ); // getter of player pos

        Monster* getMonster( gf::Vector2i pos ); // getter of monster pos

        Prop* getProp( gf::Vector2i pos ); // getter of props pos

        void sendPacketToAllPlayers( Packet &packet );
        void sendPacketToVisiblePlayers( Packet &packet, gf::Vector2i pos );
        bool canAttack(Monster &monster, gf::Vector2i targetPos);
        void placeProps();
        void getSpawnPoint();
        void putStair();

    private:
        gf::Id generateId() const;

        gf::Queue<Packet> m_ComQueue;
        gf::Queue<Message> m_chatQueue;

        int m_PlayerSpawned;
    };
}

#endif //REDSQUARE_SERVER_GAME_H