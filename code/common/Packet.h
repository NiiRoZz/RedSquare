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
        PlayerTurn,
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

    struct Packet
    {
        PacketType type;

        union
        {
            RequestMove requestMove;
            ReceiveMove receiveMove;
            PlayerDisconnected playerDisconnected;
            PlayerTurn playerTurn;
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
