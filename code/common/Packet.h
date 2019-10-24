#ifndef REDSQUARE_COMMON_PACKET_H
#define REDSQUARE_COMMON_PACKET_H

#include <cstdint>
#include <iostream>
#include <gf/Id.h>
#include <gf/Vector.h>
#include <gf/Array2D.h>

namespace redsquare
{
   /* enum class MoveDirection : uint16_t
    {
        Right,
        Left,
        Up,
        Down,
        Nothing,
    };*/

    enum class EntityType: uint8_t
    {
        Player,
        Monster,
    };

    enum class Tile: uint8_t
    {
        Void,
        Wall,
        Room,
        Corridor,
        Stair,
        Grid,
        Test,
    };

    enum class PacketType : uint16_t
    {
        RequestMove,
        ReceiveMove,
        RequestAttack,
        PlayerDisconnected,
        EntityCar,
        PlayerTurn,
        SpawnEntity,
    };

    struct NewPlayer
    {
        gf::Id playerID;
        gf::Array2D<Tile> world;

        NewPlayer( gf::Array2D<Tile> &tile, gf::Id id )
        : world( tile )
        , playerID( id )
        {
        }

        NewPlayer()
        {
        }
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

        int m_MovePoint;
        int m_Range;

        int m_XP;

        int m_MaxXP;
        int m_Level;
    };

    struct RequestMove
    {
        gf::Id playerID;
        int dirX;
        int dirY;
    };

    struct ReceiveMove
    {
        gf::Id playerID;
        int posX;
        int posY;
    };

    struct RequestAttack
    {
        gf::Id playerID;
        int posX;
        int posY;
    };

    struct PlayerDisconnected
    {
        gf::Id playerID;
    };

    struct PlayerTurn
    {
        bool playerTurn;
    };

    struct SpawnEntity
    {
        gf::Id entityID;
        EntityType typeEntity;
        uint8_t typeOfEntity;
        int posX;
        int posY;
    };

    struct Packet
    {
        PacketType type;

        union
        {
            RequestMove requestMove;
            ReceiveMove receiveMove;
            RequestAttack requestAttack;
            PlayerDisconnected playerDisconnected;
            PlayerTurn playerTurn;
            SpawnEntity spawnEntity;
            EntityCar entityCar;
        };
    };

    template<class Archive>
    Archive& operator|(Archive& ar, Packet& packet)
    {
        ar | packet.type;

        switch (packet.type)
        {
            case PacketType::RequestMove:
            {
                ar | packet.requestMove.playerID;
                ar | packet.requestMove.dirX;
                ar | packet.requestMove.dirY;
                break;
            }

            case PacketType::ReceiveMove:
            {
                ar | packet.receiveMove.playerID;
                ar | packet.receiveMove.posX;
                ar | packet.receiveMove.posY;
                break;
            }
                
            case PacketType::PlayerDisconnected:
            {
                ar | packet.playerDisconnected.playerID;
                break;
            }

            case PacketType::RequestAttack:
            {
                ar | packet.requestAttack.playerID;
                ar | packet.requestAttack.posX;
                ar | packet.requestAttack.posY;
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

                ar | packet.entityCar.m_MovePoint;
                ar | packet.entityCar.m_Range;

                ar | packet.entityCar.m_XP;
                ar | packet.entityCar.m_MaxXP;

                ar | packet.entityCar.m_Level;

                break;
            }
        }

        return ar;
    }

    template<class Archive>
    Archive& operator|(Archive& ar, NewPlayer& packet)
    {
        ar | packet.playerID;
        ar | packet.world;

        return ar;
    }
}

#endif
