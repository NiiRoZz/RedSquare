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
        PlayerDisconnected,
        PlayerCar,
        PlayerTurn,
        SpawnPlayer,
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

    struct PlayerCar
    {
        gf::Id Id;
        int m_LifePoint;
        int m_ManaPoint;

        int m_MaxLifePoint;
        int m_MaxManaPoint;


        int m_AttackPoint;
        int m_DefensePoint;

        int m_MovePoint;
        int m_Range;

        int m_XP;
        int m_MaxXP;
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

    struct PlayerDisconnected
    {
        gf::Id playerID;
    };

    struct PlayerTurn
    {
        bool playerTurn;
    };

    struct SpawnPlayer
    {
        gf::Id playerID;
        uint8_t typePlayer;
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
            PlayerDisconnected playerDisconnected;
            PlayerTurn playerTurn;
            SpawnPlayer spawnPlayer;
            PlayerCar playerCar;
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

            case PacketType::PlayerTurn:
            {
                ar | packet.playerTurn.playerTurn;
                break;
            }

            case PacketType::SpawnPlayer:
            {
                ar | packet.spawnPlayer.playerID;
                ar | packet.spawnPlayer.typePlayer;
                ar | packet.spawnPlayer.posX;
                ar | packet.spawnPlayer.posY;
                break;
            }
            case PacketType::PlayerCar:
            {
                ar | packet.playerCar.Id;

                ar | packet.playerCar.m_LifePoint;
                ar | packet.playerCar.m_ManaPoint;

                ar | packet.playerCar.m_MaxLifePoint;
                ar | packet.playerCar.m_MaxManaPoint;

                ar | packet.playerCar.m_AttackPoint;
                ar | packet.playerCar.m_DefensePoint;

                ar | packet.playerCar.m_MovePoint;
                ar | packet.playerCar.m_Range;


                ar | packet.playerCar.m_XP;
                ar | packet.playerCar.m_MaxXP;

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
