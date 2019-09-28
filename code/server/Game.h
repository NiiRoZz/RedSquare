#ifndef REDSQUARE_SERVER_GAME_H
#define REDSQUARE_SERVER_GAME_H

#include "../common/Sockets.h"
#include "../common/Packet.h"
#include "Player.h"

#include <gf/Queue.h>

namespace redsquare
{
    class Game
    {
        public:
        std::map<gf::Id, Player> m_Players;

        Game( std::uint16_t port );

        void addNewPlayer(SocketTcp socket);

        bool processPackets( Packet &packet );

        Player* getPlayer( gf::Id playerID );

        void sendPacketToAllPlayers( Packet &packet );
        void sendPacketToVisiblePlayers( Packet &packet, gf::Vector2i pos );

        private:
        gf::Id generateId() const;

        ListenerTcp m_Listener;

        gf::Queue<Packet> m_ComQueue;
    };
}

#endif //REDSQUARE_SERVER_GAME_H