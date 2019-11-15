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

        //All props
        std::map<gf::Id, Prop> m_Props;

        //Value if it's his turn and he can play
        bool m_CanPlay;

        //Value to say we can play the game
        bool m_PlayerDead;

        World m_World;

        uint m_Floor;

        std::vector<gf::Vector2i> m_TempMove;

        Game( char* hostname, char *port, gf::ExtendView &view , const char * name);

        Player* getMyPlayer();
        Player* getPlayer( gf::Id playerID );
        Player* getPlayer( gf::Vector2i pos );

        Monster* getMonster( gf::Id monsterID );
        Monster* getMonster( gf::Vector2i pos );

        Prop* getProp( gf::Id propID );

        bool monsterInRange();

        void startThreadCom();

        void sendInfoConnection(EntitySubType type, char *name);

        void receiveWorld();

        void movePlayer( int dirX, int dirY, bool mouseClic = false );

        void sendMessage(std::string message );

        void receiveMessage(std::string message);

        void attackPos( int posX, int posY );

        void passTurn();

        void processPackets();

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
        const char *m_Name;
        //Thread for communication
        ThreadCom m_ThreadCom;

       // ThreadCom m_ChatCom;
        //Queue for message
        gf::Queue<Packet> m_ComQueue;

        gf::Queue<Packet> m_ChatQueue;


        gf::Texture &m_NextPosTexture;

        //Move of player
        std::pair<gf::Vector2i, bool> m_MovePlayer;
        gf::Vector2i m_TempMoveTarget;

        //Attack of player
        int m_AttackX;
        int m_AttackY;

        //Pass turn
        bool m_PassTurn;

        //View of the game
        gf::ExtendView &m_View;

        void doAction();
    };
}


#endif
