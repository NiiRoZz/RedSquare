#ifndef REDSQUARE_CLIENT_GAME_H
#define REDSQUARE_CLIENT_GAME_H

#include <vector>
#include <map>

#include <gf/Entity.h>
#include <gf/Time.h>
#include <gf/Id.h>
#include <gf/Views.h>
#include <gf/Map.h>

#include "Monster.h"
#include "Player.h"
#include "Prop.h"
#include "ThreadCom.h"
#include "../common/Packet.h"
#include "World.h"
#include "AnimatedEntity.h"

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

        //All animatedEntity
        std::map<gf::Id, AnimatedEntity> m_AnimatedEntities;
        //All props
        std::map<gf::Id, Prop> m_Props;

        //Value if it's his turn and he can play
        bool m_CanPlay;

        World m_World;

        std::vector<gf::Vector2i> m_TempMove;
        
        Game( char* hostname, char *port, gf::ExtendView &view );

        Player* getPlayer( gf::Id playerID );
        Player* getPlayer( gf::Vector2i pos );

        Monster* getMonster( gf::Id monsterID );
        Monster* getMonster( gf::Vector2i pos );

        bool playerNear();
        bool monsterNear();

        void startThreadCom();

        void sendInfoConnection(EntityClass type, char *name);

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

        gf::Texture nextPosTexture;

        //Move of player
        int m_dirX;
        int m_dirY;

        //Attack of player
        int m_AttackX;
        int m_AttackY;

        //View of the game
        gf::ExtendView &m_View;

        void doAction();
    };
}


#endif
