#include "ServerLobby.h"

#include <cassert>
#include <cinttypes>

#include <gf/Log.h>
#include <gf/Math.h>

#include "../common/Protocol.h"

namespace redsquare
{
    ServerLobby::ServerLobby(gf::Random& random, GameFactory& factory)
    : m_Random(random)
    , m_Factory(factory)
    {
    }

    void ServerLobby::update(ServerPlayer& player, gf::Packet& bytes) {
        switch (bytes.getType())
        {
            case ClientHello::type:
            {
                gf::Log::info("(LOBBY) {%" PRIX64 "} Hello.\n", player.id);
                auto data = bytes.as<ClientHello>();
                player.name = data.name;
                // send an acknowledgement to the player
                ServerHello hello;
                hello.playerId = player.id;
                player.send(hello);
                // send list of rooms
                ServerListRooms list;
                list.rooms = getRooms();
                player.send(list);
                // broadcast new information
                broadcastPlayers();
                break;
            }

            case ClientCreateRoom::type:
            {
                auto id = m_Random.get().computeId();
                gf::Log::info("(LOBBY) {%" PRIX64 "} Create room @%" PRIX64 "\n", player.id, id);
                // deserialize
                auto data = bytes.as<ClientCreateRoom>();
                // create new room
                ServerRoom roomInstance(m_Factory);
                roomInstance.id = id;
                roomInstance.name = std::move(data.name);
                // add it to the lobby
                auto res = m_Rooms.emplace(id, std::move(roomInstance));
                assert(res.second);
                auto& room = res.first->second;
                // send an acknowledgement to the player
                ServerJoinRoom join;
                join.room = id;
                player.send(join);
                // move player to the room
                removePlayer(player);
                room.addPlayer(player);
                // broadcast new information
                broadcastRooms();
                break;
            }

            case ClientJoinRoom::type:
            {
                gf::Log::info("(LOBBY) {%" PRIX64 "} Join room.\n", player.id);
                // check if player is in a room
                if (player.room != nullptr)
                {
                    gf::Log::warning("(LOBBY) {%" PRIX64 "} Already in room @%" PRIX64 "\n", player.id, player.room->id);
                    ServerError error;
                    error.reason = ServerError::PlayerAlreadyInRoom;
                    player.send(error);
                    break;
                }
                // deserialize
                auto data = bytes.as<ClientJoinRoom>();
                // find the room
                auto it = m_Rooms.find(data.room);
                if (it == m_Rooms.end())
                {
                    gf::Log::warning("(LOBBY) {%" PRIX64 "} Unknown room @%" PRIX64 "\n", player.id, data.room);
                    ServerError error;
                    error.reason = ServerError::UnknownRoom;
                    player.send(error);
                    break;
                }
                auto& room = it->second;
                // check if the room can accept more players
                if (room.isGameStarted())
                {
                    gf::Log::warning("(LOBBY) {%" PRIX64 "} Game started @%" PRIu64 "\n", player.id, data.room);
                    ServerError error;
                    error.reason = ServerError::GameAlreadyStarted;
                    player.send(error);
                    break;
                }
                if (room.getPlayersCount() == MAX_PLAYERS)
                {
                    gf::Log::warning("(LOBBY) {%" PRIX64 "} Full room @%" PRIu64 "\n", player.id, data.room);
                    ServerError error;
                    error.reason = ServerError::FullRoom;
                    player.send(error);
                    break;
                }
                // send the answer
                ServerJoinRoom join;
                join.room = room.id;
                player.send(join);
                // move player to the room
                removePlayer(player);
                room.addPlayer(player);
                // broadcast new information
                broadcastRooms();
                break;
            }

            case ClientLeaveRoom::type:
            {
                gf::Log::info("(LOBBY) {%" PRIX64 "} Leave room.\n", player.id);
                if (player.room == nullptr)
                {
                    gf::Log::warning("(LOBBY) {%" PRIX64 "} Player not in a room\n", player.id);
                    ServerError error;
                    error.reason = ServerError::PlayerNotInRoom;
                    player.send(error);
                    break;
                }

                auto& room = *player.room;
                room.removePlayer(player);
                checkEmptyRoom(room);
                addPlayer(player);
                // send an acknowledgement to the player
                ServerLeaveRoom leave;
                player.send(leave);
                // send player the list of rooms
                ServerListRooms list;
                list.rooms = getRooms();
                player.send(list);
                // broadcast new information
                broadcastPlayers();
                broadcastRooms();
                break;
            }

            case ClientChatMessage::type:
            {
                gf::Log::info("(LOBBY) {%" PRIX64 "} Chat message.\n", player.id);
                if (player.room != nullptr)
                {
                    // the message needs to be broadcast to the room, not the lobby
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

        if (player.room != nullptr)
        {
            gf::Log::info("(LOBBY) {%" PRIX64 "} Forwarding to room @%" PRIX64 ".\n", player.id, player.room->id);
            player.room->update(player, bytes);
        }
    }

    std::vector<RoomData> ServerLobby::getRooms()
    {
        std::vector<RoomData> list;

        for (auto& kv : m_Rooms)
        {
            auto& room  = kv.second;
            RoomData data;
            data.id = room.id;
            data.name = room.name;
            data.players = room.getPlayersCount();
            list.push_back(std::move(data));
        }

        return list;
    }

    void ServerLobby::broadcastRooms()
    {
        ServerListRooms data;
        data.rooms = getRooms();
        broadcast(data);
    }

    void ServerLobby::broadcastPlayers()
    {
        ServerListPlayers data;
        data.players = getPlayers();
        broadcast(data);
    }

    void ServerLobby::doAddPlayer(ServerPlayer& player)
    {
        gf::Log::info("(LOBBY) {%" PRIX64 "} Adding player.\n", player.id);
    }

    void ServerLobby::doRemovePlayer(ServerPlayer& player)
    {
        gf::Log::info("(LOBBY) {%" PRIX64 "} Removing player.\n", player.id);

        if (player.room != nullptr)
        {
            auto& room = *player.room;
            room.removePlayer(player);
            checkEmptyRoom(room);
            broadcastRooms();
        }

        broadcastPlayers();
    }

    void ServerLobby::checkEmptyRoom(ServerRoom& room)
    {
        if (!room.isEmpty())
        {
            return;
        }

        auto count = m_Rooms.erase(room.id);
        assert(count == 1);

        broadcastRooms();
    }
}
