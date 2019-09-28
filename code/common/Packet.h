#ifndef REDSQUARE_COMMON_PACKET_H
#define REDSQUARE_COMMON_PACKET_H

#include <cstdint>
#include <gf/Id.h>
#include <gf/Vector.h>
#include <gf/Array2D.h>

namespace redsquare
{
    enum class MoveDirection : uint16_t
    {
        Right,
        Left,
        Up,
        Down,
        Nothing,
    };

    enum class Tile: uint8_t
    {
        Wall,
        Ground,
    };

    enum class PacketType : uint16_t
    {
        NewPlayer,
        RequestMove,
        ReceiveMove,
        PlayerDisconnected,
        PlayerTurn,
    };

    struct NewPlayer
    {
        gf::Id playerID;
        //Should add gf::Array2D<Tile> here but not working for now until solution
        int world;
    };

    struct RequestMove
    {
        gf::Id playerID;
        MoveDirection dir;
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
            NewPlayer newPlayer;
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

        switch (packet.type) {
            case PacketType::NewPlayer:
            {
                ar | packet.newPlayer.playerID;
                ar | packet.newPlayer.world;
                break;
            }

            case PacketType::RequestMove:
            {
                ar | packet.requestMove.playerID;
                ar | packet.requestMove.dir;
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
}

#endif