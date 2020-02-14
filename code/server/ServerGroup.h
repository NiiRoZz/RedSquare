#ifndef REDSQUARE_SERVER_GROUP_H
#define REDSQUARE_SERVER_GROUP_H

#include <cstdint>
#include <vector>

#include <gf/Ref.h>

#include "../common/ProtocolBytes.h"
#include "../common/ProtocolData.h"

#include "ServerPlayer.h"

namespace redsquare 
{

  class ServerGroup
  {
    public:
        virtual ~ServerGroup();

        void addPlayer(ServerPlayer& player);
        void removePlayer(ServerPlayer& player);
        ServerPlayer& getServerPlayer(gf::Id id);
        std::vector<PlayerData> getPlayers();

        bool isEmpty() const
        {
            return m_players.empty();
        }

        int32_t getPlayersCount()
        {
            return static_cast<int32_t>(m_players.size());
        }

        bool areAllPlayersReady();

        virtual void update(ServerPlayer& player, ProtocolBytes& bytes) = 0;

        template<typename T>
        void send(gf::Id id, const T& data)
        {
            ProtocolBytes bytes;
            bytes.is(data);

            for (ServerPlayer& player : m_players)
            {
                if (player.id == id)
                {
                    player.socket.sendPacket(bytes.packet);
                    return;
                }
            }
        }

        template<typename T>
        void broadcast(const T& data)
        {
            ProtocolBytes bytes;
            bytes.is(data);

            for (ServerPlayer& player : m_players)
            {
                player.socket.sendPacket(bytes.packet);
            }
        }

        void cloneTo(ServerGroup& group);

    private:
        virtual void doAddPlayer(ServerPlayer& player);
        virtual void doRemovePlayer(ServerPlayer& player);

    private:
        std::vector<gf::Ref<ServerPlayer>> m_players;
  };

}

#endif // REDSQUARE_SERVER_GROUP_H
