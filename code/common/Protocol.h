#ifndef REDSQUARE_COMMON_PACKET_H
#define REDSQUARE_COMMON_PACKET_H

#include <cstdint>
#include <iostream>
#include <gf/Id.h>
#include <gf/Vector.h>
#include <gf/Array2D.h>
#include <gf/SerializationOps.h>
#include <string>

#include "Constants.h"
#include "ProtocolData.h"

using namespace gf::literals;

namespace redsquare
{
    /*
        Server ===> Client
    */
    struct ServerHello
    {
        static constexpr gf::Id type = "ServerHello"_id;
        gf::Id playerId;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerHello& data)
    {
        return ar | data.playerId;
    }

    struct ServerDisconnect
    {
        static constexpr gf::Id type = "ServerDisconnect"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerDisconnect&)
    {
        return ar;
    }

    struct ServerAnswerRoom
    {
        static constexpr gf::Id type = "ServerAnswerRoom"_id;
        gf::Id room;
        std::string name;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerAnswerRoom& data)
    {
        return ar | data.room | data.name;
    }

    struct ServerJoinRoom
    {
        static constexpr gf::Id type = "ServerJoinRoom"_id;
        gf::Id room;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerJoinRoom& data)
    {
        return ar | data.room;
    }

    struct ServerLeaveRoom
    {
        static constexpr gf::Id type = "ServerLeaveRoom"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerLeaveRoom&)
    {
        return ar;
    }

    struct ServerReady
    {
        static constexpr gf::Id type = "ServerReady"_id;
        bool ready;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerReady& data)
    {
        return ar | data.ready;
    }

    struct ServerChatMessage
    {
        static constexpr gf::Id type = "ServerChatMessage"_id;
        MessageData message;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerChatMessage& data)
    {
        return ar | data.message;
    }

    struct ServerListPlayers
    {
        static constexpr gf::Id type = "ServerListPlayers"_id;
        std::vector<PlayerData> players;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerListPlayers& data)
    {
        return ar | data.players;
    }

    struct ServerListRoomPlayers
    {
        static constexpr gf::Id type = "ServerListRoomPlayers"_id;
        std::vector<PlayerData> players;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerListRoomPlayers& data)
    {
        return ar | data.players;
    }

    struct ServerListRooms
    {
        static constexpr gf::Id type = "ServerListRooms"_id;
        std::vector<RoomData> rooms;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerListRooms& data)
    {
        return ar | data.rooms;
    }

    struct ServerStartGame
    {
        static constexpr gf::Id type = "ServerStartGame"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerStartGame&)
    {
        return ar;
    }

    struct ServerStopGame
    {
        static constexpr gf::Id type = "ServerStopGame"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerStopGame&)
    {
        return ar;
    }

    struct ServerError
    {
        static constexpr gf::Id type = "ServerError"_id;

        enum Type : uint16_t
        {
            PlayerAlreadyInRoom,
            PlayerAlreadyReady,
            PlayerNotInRoom,
            PlayerNotInTeam,
            UnknownRoom,
            FullRoom,
            UnknownTeam,
            FullTeam,
            GameAlreadyStarted,
        };

        Type reason;
    };

    inline std::string serverErrorString(ServerError::Type type)
    {
        switch (type)
        {
            case ServerError::Type::PlayerAlreadyInRoom:
            return "You are already in room.";

            case ServerError::Type::PlayerAlreadyReady:
            return "You are already ready.";

            case ServerError::Type::PlayerNotInRoom:
            return "You are not in room.";

            case ServerError::Type::PlayerNotInTeam:
            return "You are not in team.";

            case ServerError::Type::UnknownRoom:
            return "The room is unknown.";

            case ServerError::Type::FullRoom:
            return "The room is full.";

            case ServerError::Type::UnknownTeam:
            return "The team is unknown.";

            case ServerError::Type::FullTeam:
            return "The team is full.";

            case ServerError::Type::GameAlreadyStarted:
            return "The game is already started.";
        }

        assert(false);
        return "";
    }

    template<typename Archive>
    Archive operator|(Archive& ar, ServerError& data)
    {
        return ar | data.reason;
    }

    /*
        Client ===> Server
    */
    struct ClientHello
    {
        static constexpr gf::Id type = "ClientHello"_id;
        std::string name;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ClientHello& data)
    {
        return ar | data.name;
    }

    struct ClientDisconnect
    {
        static constexpr gf::Id type = "ClientDisconnect"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ClientDisconnect&)
    {
        return ar;
    }

    struct ClientCreateRoom
    {
        static constexpr gf::Id type = "ClientCreateRoom"_id;
        std::string name;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ClientCreateRoom& data)
    {
        return ar | data.name;
    }

    struct ClientQueryRoom
    {
        static constexpr gf::Id type = "ClientQueryRoom"_id;
        gf::Id room;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ClientQueryRoom& data)
    {
        return ar | data.room;
    }

    struct ClientJoinRoom
    {
        static constexpr gf::Id type = "ClientJoinRoom"_id;
        gf::Id room;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ClientJoinRoom& data)
    {
        return ar | data.room;
    }

    struct ClientLeaveRoom
    {
        static constexpr gf::Id type = "ClientLeaveRoom"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ClientLeaveRoom&)
    {
        return ar;
    }

    struct ClientReady
    {
        static constexpr gf::Id type = "ClientReady"_id;
        bool ready;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ClientReady& data)
    {
        return ar | data.ready;
    }

    struct ClientSubType
    {
        static constexpr gf::Id type = "ClientSubType"_id;
        EntitySubType entitySubType;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ClientSubType& data)
    {
        return ar | data.entitySubType;
    }

    struct ClientChatMessage
    {
        static constexpr gf::Id type = "ClientChatMessage"_id;
        std::string content;
        gf::Id recipient = gf::InvalidId;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ClientChatMessage& data)
    {
        return ar | data.content | data.recipient;
    }
}

#endif
