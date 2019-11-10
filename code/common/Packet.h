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
        Prop,
    };

    enum class EntityClass: uint8_t
    {
        /* CLASSES */
        Magus,
        Warrior,
        // Paladin,
        // Archer,
        // Rogue,
        // Healer,
        /* CLASSES */


        //Class count for random
        EntityClassCount,

        /* PROP */
        Box,
        Pot,
        /* PROP */
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

    enum class SpellType: uint8_t
    {
        BasicAttack, // basic attack ------- ALL
        FireBall, // randed attack that can burn foes ------- MAGUS
        ArmorUp, // gain a bonus of armor for x turns ------- WARRIOR
        DoubleStrike, // IDK for now
        Heal, // heal a foes ------- PRIEST
        Assasinate, // can one shot a foes  with high miss rate ------- ROGUE
        DamageUp, // gain a bonus of attack for x turns ------- WARRIOR/ROGUE
        Protection, // protect an ally
        Revenge, // deal damage to foes based on missing health of source
        Lacerate, // deal damage to foes based on missing health of targer
        Incinerate, // deal damage to foes and can burn ennemy
        Devastate, // AOE dmg
        Massacre, // bleed dmg
        Impact, // dash attack ?
        LightningStrike, // AOE dmg
        Scorch, // dmg + bleed
        Chaos, // AOE dmg
        Berserk, // gain attack, loose defence
        Cleanse, // purge all status effect
        Mirror, // deal a percentage of damage receice to attacker
        Torpedo, // 
        Stun, // stun ennemy for x turns
        Warp, // dash 
        Reincarnate, // 
        Shoot, // long attack range
        Backstab, // deal damage to foes : x2 if behind foes
        Energize, // next attack deal more damage
        SoulLink, // take damage that ally can receive
    };

    enum class PacketType : uint16_t
    {
        PlayerInfoConnection,
        RequestMove,
        ReceiveMove,
        RequestAttack,
        PassTurn,
        EntityDisconnected,
        EntityCar,
        PlayerTurn,
        SpawnEntity,
        PlayerDead,
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

    struct PlayerInfoConnection
    {
        EntityClass entityClass;
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
        int posX;
        int posY;
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
        EntityClass typeOfEntity;
        int posX;
        int posY;
    };

    struct PassTurn
    {
        gf::Id playerID;
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
            PassTurn passTurn;
            EntityDisconnected entityDisconnected;
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
            case PacketType::PlayerInfoConnection:
            {
                ar | packet.playerInfoConnection.entityClass;
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
                ar | packet.requestAttack.posX;
                ar | packet.requestAttack.posY;
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
