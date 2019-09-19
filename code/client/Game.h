#ifndef REDSQUARE_CLIENT_GAME_H
#define REDSQUARE_CLIENT_GAME_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Time.h>
#include <gf/Id.h>

#include "Square.h"
#include "../common/ThreadCom.h"
#include "../common/Packet.h"
#include <boost/ptr_container/ptr_map.hpp>

namespace redsquare
{
    class Game: public gf::Entity
    {
        public:
        Game( char* hostname, char *port );

        Square* getPlayer( gf::Id playerID );

        void startThreadCom();

        void movePlayer( MoveDirection dir );

        void processPackets();

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        private:
        World m_World;

        //ID of current player
        gf::Id m_SquareID;
        //All near players
        boost::ptr_map<gf::Id, Square> m_Squares;

        //Thread for communication
        ThreadCom m_ThreadCom;
        //Queue for message
        gf::Queue<Packet> m_ComQueue;

        //Move of player
        MoveDirection m_DirMoving;
    };
}


#endif