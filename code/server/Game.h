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

        Game();

        World m_World;

        void addNewPlayer(SocketTcp socket);
        void addNewMonsters(int nbMonster);

        void processPackets( Packet &packet );

        Player* getPlayer( gf::Id playerID );
        Player* getPlayer( gf::Vector2i pos );

        Monster* getMonster( gf::Vector2i pos );

        Prop* getProp( gf::Vector2i pos );

        void sendPacketToAllPlayers( Packet &packet );
        void sendPacketToVisiblePlayers( Packet &packet, gf::Vector2i pos );
        bool canAttack(Monster &monster, gf::Vector2i targetPos);

    private:
        gf::Id generateId() const;

        gf::Queue<Packet> m_ComQueue;
    };
}

#endif //REDSQUARE_SERVER_GAME_H