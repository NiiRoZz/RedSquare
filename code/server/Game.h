#ifndef REDSQUARE_SERVER_GAME_H
#define REDSQUARE_SERVER_GAME_H

#include "../common/Sockets.h"
#include "../common/Packet.h"
#include "Player.h"

#include <gf/Queue.h>
#include <boost/ptr_container/ptr_map.hpp>

namespace redsquare
{
    class Game
    {
        public:
        Game( std::uint16_t port );

        void addNewPlayer(SocketTcp socket);

        void doUpdate(gf::Time time);
        void processPackets();

        Player* getPlayer( gf::Id playerID );

        void sendPacketToAllPlayers( Packet &packet );
        void sendPacketToVisiblePlayers( Packet &packet, gf::Vector2i pos );

        void detectDisonnection();

        private:
        gf::Id generateId() const;

        private:
        ListenerTcp m_Listener;

        gf::Queue<Packet> m_ComQueue;

        boost::ptr_map<gf::Id, Player> m_Players;
    };
}

#endif //REDSQUARE_SERVER_GAME_H