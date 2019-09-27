#ifndef REDSQUARE_CLIENT_GAME_H
#define REDSQUARE_CLIENT_GAME_H

#include <vector>
#include <map>

#include <gf/Entity.h>
#include <gf/Time.h>
#include <gf/Id.h>
#include <gf/Views.h>

#include "Player.h"
#include "../common/ThreadCom.h"
#include "../common/Packet.h"

namespace redsquare
{
    class Game: public gf::Entity
    {
        public:
        Game( char* hostname, char *port, gf::ExtendView &view );

        Player* getPlayer( gf::Id playerID );

        void startThreadCom();

        void movePlayer( MoveDirection dir );

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
        MoveDirection m_DirMoving;

        gf::ExtendView* m_View;
    };
}


#endif