#ifndef REDSQUARE_CLIENT_GAME_H
#define REDSQUARE_CLIENT_GAME_H

#include <vector>
#include <map>

#include <gf/Entity.h>
#include <gf/Time.h>
#include <gf/Id.h>
#include <gf/Views.h>

#include "Player.h"
#include "ThreadCom.h"
#include "../common/Packet.h"
#include "World.h"

namespace redsquare
{
    class Game: public gf::Entity
    {
        public:
        Game( char* hostname, char *port, gf::ExtendView &view );

        Player* getPlayer( gf::Id playerID );
        Player* getPlayer( gf::Vector2i pos );

        void startThreadCom();

        void receiveWorld();

        void movePlayer( int dirX, int dirY);

        void processPackets();

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        private:
        World m_World;

        //ID of current player
        gf::Id m_PlayerID;
        //All near players
        std::map<gf::Id, Player> m_Players;

        //Thread for communication
        ThreadCom m_ThreadCom;
        //Queue for message
        gf::Queue<Packet> m_ComQueue;

        //Move of player
        //MoveDirection m_DirMoving;
        int m_dirX;
        int m_dirY;

        //View of the game
        gf::ExtendView* m_View;

        //Value if it's his turn and he can play
        bool m_CanPlay;

        void doAction();
    };
}


#endif
