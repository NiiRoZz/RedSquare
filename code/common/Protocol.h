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

    enum class PacketType : uint16_t
    {
        PlayerInfoConnection,
        RequestMove,
        ReceiveMove,
        RequestAttack,
        RequestUse,
        RequestDrop,
        PassTurn,
        EntityDisconnected,
        EntityCar,
        PlayerTurn,
        SpawnEntity,
        PlayerDead,
        NewMap,
        UpdateSpells,
        UpdateItem,
        MoveItem,
        Message,
    };

    struct NewPlayer
    {
        gf::Id playerID;
        uint floor;
        gf::Array2D<Tile> world;

        NewPlayer( gf::Array2D<Tile> &tile, gf::Id id, uint currFloor )
        : world( tile )
        , playerID( id )
        , floor( currFloor )
        {
        }

        NewPlayer()
        {
        }
    };

    struct Message
    {
        char from[MAX_SIZE_FROM_CHAT];
        char message[MAX_SIZE_MESSAGE_CHAT];
        char to[MAX_SIZE_FROM_CHAT];
    };

    struct SendName
    {
        char from[MAX_SIZE_FROM_CHAT];
    };


    struct PlayerInfoConnection
    {
        EntitySubType entitySubType;
        char name[20];
    };

    struct EntityCar
    {
        gf::Id entityID;
        EntityType entityType;

        int m_LifePoint;
        int m_ManaPoint;

        int m_MaxLifePoint;
        int m_MaxManaPoint;

        int m_AttackPoint;
        int m_DefensePoint;

        int m_MaxAttackPoint;
        int m_MaxDefensePoint;

        int m_Range;

        int m_XP;

        int m_MaxXP;
        int m_Level;
        gf::Vector2i m_Pos;
    };

    struct RequestMove
    {
        gf::Id playerID;
        int dirX;
        int dirY;
    };

    struct ReceiveMove
    {
        gf::Id entityID;
        EntityType typeEntity;
        int posX;
        int posY;
    };

    struct RequestAttack
    {
        gf::Id playerID;
        SpellType spellType;
        int posX;
        int posY;
    };
    
    struct RequestUse
    {
        gf::Id entityID;
        EntityType entityType;
        gf::Id playerID;
        InventorySlotType slotType;
        uint pos;
    };

    struct RequestDrop
    {
        gf::Id entityID;
        EntityType entityType;
        gf::Id playerID;
        InventorySlotType slotType;
        uint pos;
    };

    struct EntityDisconnected
    {
        gf::Id entityID;
        EntityType typeEntity;
    };

    struct PlayerTurn
    {
        bool playerTurn;
    };

    struct SpawnEntity
    {
        gf::Id entityID;
        EntityType typeEntity;
        EntitySubType typeOfEntity;
        int posX;
        int posY;
        ItemType holdingItem;
    };

    struct PassTurn
    {
        gf::Id playerID;
    };

    struct UpdateSpells
    {
        SpellType spells[MAX_SPELL_PER_PLAYER];
    };

    struct UpdateItem
    {
        gf::Id entityID;
        EntityType entityType;
        InventorySlotType slotType;
        uint pos;
        ItemType typeItem;
        uint baseFloorItem;
        bool removeItem;
        uint8_t slotMask;
    };

    struct MoveItem
    {
        gf::Id oldEntityID;
        EntityType oldEntityType;
        InventorySlotType oldSlotType;
        uint oldPos;
        gf::Id newEntityID;
        EntityType newEntityType;
        InventorySlotType newSlotType;
        uint newPos;
    };

    struct Packet
    {
        PacketType type;

        union
        {
            PlayerInfoConnection playerInfoConnection;
            RequestMove requestMove;
            ReceiveMove receiveMove;
            RequestAttack requestAttack;
            RequestUse requestUse;
            RequestDrop requestDrop;
            PassTurn passTurn;
            EntityDisconnected entityDisconnected;
            PlayerTurn playerTurn;
            SpawnEntity spawnEntity;
            EntityCar entityCar;
            UpdateSpells updateSpells;
            UpdateItem updateItem;
            MoveItem moveItem;
        };
    };

    template<class Archive>
    Archive& operator|(Archive& ar, Packet& packet)
    {
        ar | packet.type;

        switch (packet.type)
        {
            case PacketType::PlayerInfoConnection:
            {
                ar | packet.playerInfoConnection.entitySubType;
                ar | packet.playerInfoConnection.name;
                break;
            }

            case PacketType::RequestMove:
            {
                ar | packet.requestMove.playerID;
                ar | packet.requestMove.dirX;
                ar | packet.requestMove.dirY;
                break;
            }

            case PacketType::ReceiveMove:
            {
                ar | packet.receiveMove.entityID;
                ar | packet.receiveMove.typeEntity;
                ar | packet.receiveMove.posX;
                ar | packet.receiveMove.posY;
                break;
            }

            case PacketType::RequestAttack:
            {
                ar | packet.requestAttack.playerID;
                ar | packet.requestAttack.spellType;
                ar | packet.requestAttack.posX;
                ar | packet.requestAttack.posY;
                break;
            }

            case PacketType::RequestUse:
            {
                ar | packet.requestUse.entityID;
                ar | packet.requestUse.entityType;
                ar | packet.requestUse.playerID;
                ar | packet.requestUse.slotType;
                ar | packet.requestUse.pos;
                break;
            }

            case PacketType::RequestDrop:
            {
                ar | packet.requestDrop.entityID;
                ar | packet.requestDrop.entityType;
                ar | packet.requestDrop.playerID;
                ar | packet.requestDrop.slotType;
                ar | packet.requestDrop.pos;
                break;
            }

            case PacketType::PassTurn:
            {
                ar | packet.passTurn.playerID;
                break;
            }

            case PacketType::EntityDisconnected:
            {
                ar | packet.entityDisconnected.entityID;
                ar | packet.entityDisconnected.typeEntity;
                break;
            }

            case PacketType::PlayerTurn:
            {
                ar | packet.playerTurn.playerTurn;
                break;
            }

            case PacketType::SpawnEntity:
            {
                ar | packet.spawnEntity.entityID;
                ar | packet.spawnEntity.typeEntity;
                ar | packet.spawnEntity.typeOfEntity;
                ar | packet.spawnEntity.posX;
                ar | packet.spawnEntity.posY;
                ar | packet.spawnEntity.holdingItem;
                break;
            }
            case PacketType::EntityCar:
            {
                ar | packet.entityCar.entityID;
                ar | packet.entityCar.entityType;

                ar | packet.entityCar.m_LifePoint;
                ar | packet.entityCar.m_ManaPoint;

                ar | packet.entityCar.m_MaxLifePoint;
                ar | packet.entityCar.m_MaxManaPoint;

                ar | packet.entityCar.m_AttackPoint;
                ar | packet.entityCar.m_DefensePoint;

                ar | packet.entityCar.m_Range;

                ar | packet.entityCar.m_XP;
                ar | packet.entityCar.m_MaxXP;

                ar | packet.entityCar.m_Level;

                break;
            }

            case PacketType::UpdateSpells:
            {
                ar | packet.updateSpells.spells;
                break;
            }

            case PacketType::UpdateItem:
            {
                ar | packet.updateItem.entityID;
                ar | packet.updateItem.entityType;
                ar | packet.updateItem.slotType;
                ar | packet.updateItem.pos;
                ar | packet.updateItem.typeItem;
                ar | packet.updateItem.baseFloorItem;
                ar | packet.updateItem.removeItem;
                ar | packet.updateItem.slotMask;
                break;
            }

            case PacketType::MoveItem:
            {
                ar | packet.moveItem.oldEntityID;
                ar | packet.moveItem.oldEntityType;
                ar | packet.moveItem.oldSlotType;
                ar | packet.moveItem.oldPos;
                ar | packet.moveItem.newEntityID;
                ar | packet.moveItem.newEntityType;
                ar | packet.moveItem.newSlotType;
                ar | packet.moveItem.newPos;
                break;
            }
        }

        return ar;
    }

    template<class Archive>
    Archive& operator|(Archive& ar, NewPlayer& packet)
    {
        ar | packet.playerID;
        ar | packet.floor;
        ar | packet.world;
        return ar;
    }

    template<class Archive>
    Archive& operator|(Archive& ar, Message& packet)
    {
        ar | packet.from;
        ar | packet.message;
        ar | packet.to;
        return ar;
    }


    template<class Archive>
    Archive& operator|(Archive& ar, SendName& packet)
    {
        ar | packet.from;
        return ar;
    }

    /*struct PlayerData
    {
        gf::Id id;
        char name[MAX_SIZE_FROM_CHAT];
    };

    struct PlayerReady
    {
        gf::Id id;
        bool ready;
    };

    enum class LobbyPacketType : uint16_t
    {
        PlayerData,
        PlayerReady,
    };

    struct LobbyPacket
    {
        LobbyPacketType type;

        union
        {
            PlayerData playerData;
            PlayerReady playerReady;
        };
    };

    template<class Archive>
    Archive& operator|(Archive& ar, LobbyPacket& packet)
    {
        ar | packet.type;

        switch (packet.type)
        {
            case LobbyPacketType::PlayerData:
            {
                ar | packet.playerData.id;
                ar | packet.playerData.name;
                ar | packet.playerData.ready;
                break;
            }

            case LobbyPacketType::PlayerReady:
            {
                ar | packet.playerReady.id;
                ar | packet.playerReady.ready;
                break;
            }
        }

        return ar;
    }
    */
}

#endif
