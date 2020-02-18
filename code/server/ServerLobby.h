#ifndef REDSQUARE_SERVER_LOBBY_H
#define REDSQUARE_SERVER_LOBBY_H

#include <gf/Random.h>
#include <gf/Ref.h>

#include "../common/ProtocolData.h"

#include "GameFactory.h"
#include "ServerGroup.h"
#include "ServerRoom.h"

namespace redsquare 
{
    class ServerLobby : public ServerGroup
    {
    public:
        ServerLobby(gf::Random& random, GameFactory& factory);

        void update(ServerPlayer& player, gf::Packet& bytes) override;

        std::vector<RoomData> getRooms();

        void broadcastRooms();
        void broadcastPlayers();

    private:
        void doAddPlayer(ServerPlayer& player) override;
        void doRemovePlayer(ServerPlayer& player) override;

        void checkEmptyRoom(ServerRoom& room);

    private:
        gf::Ref<gf::Random> m_Random;
        gf::Ref<GameFactory> m_Factory;
        std::map<gf::Id, ServerRoom> m_Rooms;
    };
}

#endif // REDSQUARE_SERVER_LOBBY_H
