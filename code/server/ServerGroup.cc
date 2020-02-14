#include "ServerGroup.h"

#include <algorithm>

#include <gf/Unused.h>

namespace redsquare
{
    ServerGroup::~ServerGroup() = default;

    void ServerGroup::addPlayer(ServerPlayer& player)
    {
        m_players.push_back(player);
        doAddPlayer(player);
    }

    void ServerGroup::removePlayer(ServerPlayer& player)
    {
        m_players.erase(std::remove(m_players.begin(), m_players.end(), player), m_players.end());
        doRemovePlayer(player);
    }

    ServerPlayer& ServerGroup::getServerPlayer(gf::Id id)
    {
        for(auto &player: m_players)
        {
            if (player.get().id == id)
            {
                return player.get();
            }
        }

        assert(false);
    }

    std::vector<PlayerData> ServerGroup::getPlayers()
    {
        std::vector<PlayerData> list;

        for (const ServerPlayer& player : m_players)
        {
            if (player.name.empty())
            {
                continue;
            }

            PlayerData data;
            data.id = player.id;
            data.name = player.name;
            data.ready = player.ready;
            data.subType = player.subType;
            list.push_back(std::move(data));
        }

        return list;
    }

    bool ServerGroup::areAllPlayersReady()
    {
        return std::all_of(m_players.begin(), m_players.end(), [](ServerPlayer& player) { return player.ready; });
    }


    void ServerGroup::cloneTo(ServerGroup& group)
    {
        group.m_players.clear();

        for (auto& player : m_players)
        {
            group.addPlayer(player);
        }
    }

    void ServerGroup::doAddPlayer(ServerPlayer& player)
    {
        gf::unused(player);
        // nothing by default
    }

    void ServerGroup::doRemovePlayer(ServerPlayer& player)
    {
        gf::unused(player);
        // nothing by default
    }
}

