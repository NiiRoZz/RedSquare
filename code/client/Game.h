#ifndef REDSQUARE_CLIENT_GAME_H
#define REDSQUARE_CLIENT_GAME_H

#include <vector>
#include <map>
#include <queue>

#include <gf/Entity.h>
#include <gf/Time.h>
#include <gf/Id.h>
#include <gf/Views.h>
#include <gf/Map.h>

#include "Monster.h"
#include "Player.h"
#include "ThreadCom.h"
#include "../common/Packet.h"
#include "World.h"

namespace redsquare
{
    class Game: public gf::Entity
    {
    public:
        //ID of current player
        gf::Id m_PlayerID;

        //All players
        std::map<gf::Id, Player> m_Players;
        //All monsters
        std::map<gf::Id, Monster> m_Monsters;

        //Value if it's his turn and he can play
        bool m_CanPlay;

        World m_World;
        
        Game( char* hostname, char *port, gf::ExtendView &view );

        Player* getPlayer( gf::Id playerID );
        Player* getPlayer( gf::Vector2i pos );

        Monster* getMonster( gf::Id monsterID );
        Monster* getMonster( gf::Vector2i pos );

        bool monsterNear();

        void startThreadCom();

        void receiveWorld();

        void movePlayer( int dirX, int dirY );

        void attackPos( int posX, int posY );

        void processPackets();

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
        //Thread for communication
        ThreadCom m_ThreadCom;
        //Queue for message
        gf::Queue<Packet> m_ComQueue;

        std::queue<gf::Vector2i> m_TempMove;

        //Move of player
        //MoveDirection m_DirMoving;
        int m_dirX;
        int m_dirY;

        int m_AttackX;
        int m_AttackY;

        //View of the game
        gf::ExtendView* m_View;

        void doAction();
    };
}


#endif
