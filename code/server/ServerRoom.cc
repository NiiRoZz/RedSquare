#include "ServerRoom.h"

#include <cassert>
#include <cinttypes>
#include <algorithm>

#include <gf/Log.h>

#include "../common/Protocol.h"

namespace redsquare
{
    ServerRoom::ServerRoom(GameFactory& factory)
    : m_Factory(factory)
    , m_Instance(nullptr)
    {
    }

    void ServerRoom::update(ServerPlayer& player, gf::Packet& bytes)
    {
        assert(player.room == this);

        switch (bytes.getType())
        {
            case ClientReady::type:
            {
                gf::Log::info("(ROOM) {%" PRIX64 "} Ready.\n", player.id);
                if (isGameStarted())
                {
                    gf::Log::warning("(ROOM) {%" PRIX64 "} Game already started\n", player.id);
                    ServerError error;
                    error.reason = ServerError::GameAlreadyStarted;
                    player.send(error);
                    break;
                }

                auto data = bytes.as<ClientReady>();
                player.ready = data.ready;
                // send an acknowledgement to the player
                ServerReady ready;
                ready.ready = data.ready;
                player.send(ready);
                // broadcast new state
                broadcastPlayers();
                // Check if everyone is ready
                startGameIfReady();
                break;
            }

            case ClientSubType::type:
            {
                gf::Log::info("(ROOM) {%" PRIX64 "} ClientSubType::type.\n", player.id);

                auto data = bytes.as<ClientSubType>();
                player.subType = data.entitySubType;

                // broadcast new state
                broadcastPlayers();
                break;
            }

            case ClientChatMessage::type:
            {
                gf::Log::info("(ROOM) {%" PRIX64 "} Chat message.\n", player.id);
                if (isGameStarted())
                {
                    break;
                }

                // deserialize
                auto in = bytes.as<ClientChatMessage>();
                // broadcast the message
                ServerChatMessage out;
                out.message.origin = player.id;
                out.message.author = player.name;
                out.message.content = std::move(in.content);
                broadcast(out);
                break;
            }
        }

        if (isGameStarted())
        {
            if (m_Instance->isFinished())
            {
                gf::Log::info("(ROOM) {%" PRIX64 "} Stop game in @%" PRIX64 ".\n", player.id, player.room->id);
                ServerStopGame data;
                broadcast(data);
                m_Instance = nullptr;
            }
            else
            {
                gf::Log::info("(ROOM) {%" PRIX64 "} Forwarding to game in @%" PRIX64 ".\n", player.id, player.room->id);
                m_Instance->update(player, bytes);
            }
        }
    }

    void ServerRoom::broadcastPlayers()
    {
        ServerListRoomPlayers data;
        data.players = getPlayers();
        broadcast(data);
    }

    bool ServerRoom::isGameStarted()
    {
        return m_Instance != nullptr;
    }

    void ServerRoom::startGameIfReady()
    {
        if (getPlayersCount() < MIN_PLAYERS || !areAllPlayersReady())
        {
            return;
        }
        ServerStartGame data;
        broadcast(data);

        m_Instance = m_Factory.get().createInstance();
        cloneTo(*m_Instance);
        m_Instance->start();
    }

    void ServerRoom::doAddPlayer(ServerPlayer& player)
    {
        gf::Log::info("(ROOM) {%" PRIX64 "} Adding player.\n", player.id);
        player.room = this;
        player.ready = false;
        broadcastPlayers();
    }

    void ServerRoom::doRemovePlayer(ServerPlayer& player)
    {
        gf::Log::info("(ROOM) {%" PRIX64 "} Removing player.\n", player.id);
        player.room = nullptr;
        player.ready = false;
        broadcastPlayers();

        if (isGameStarted())
        {
            m_Instance->removePlayer(player);
        }
    }
}
