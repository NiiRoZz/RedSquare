#ifndef REDSQUARE_SERVER_ROOM_H
#define REDSQUARE_SERVER_ROOM_H

#include "GameFactory.h"
#include "GameInstance.h"
#include "ServerGroup.h"

namespace redsquare
{
    class ServerRoom : public ServerGroup
    {
    public:
        ServerRoom(GameFactory& factory);

        void update(ServerPlayer& player, ProtocolBytes& bytes) override;

        gf::Id id;
        std::string name;

        void broadcastPlayers();

        bool isGameStarted();

    private:
        void startGameIfReady();

    private:
        void doAddPlayer(ServerPlayer& player) override;
        void doRemovePlayer(ServerPlayer& player) override;

    private:
        gf::Ref<GameFactory> m_Factory;
        std::unique_ptr<GameInstance> m_Instance;
    };

}


#endif // REDSQUARE_SERVER_ROOM_H
