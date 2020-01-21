#ifndef REDSQUARE_COMMON_PACKET_H
#define REDSQUARE_COMMON_PACKET_H

#include <cstdint>
#include <iostream>
#include <gf/Id.h>
#include <gf/Vector.h>
#include <gf/Array2D.h>
#include <string>

#define MAX_SPELL_PER_PLAYER 8

namespace redsquare
{
    enum class EntityType: uint8_t
    {
        Player,
        Monster,
        Prop,
    };

    enum class EntitySubType: uint8_t
    {
        /* CLASSES */
        Magus,
        Warrior,
        Rogue,
        // Paladin,
        // Archer,
        // Healer,
        /* CLASSES */


        //Class count for random
        EntityClassCount,

        /* SMALL PROP */
        BrokenPots,
        Pot,
        LitllePots,
        Torch,
        Chest,
        OpenedChest,
        Stool,
        PileWood,
        GreyBrokenPillar,
        BrownBrokenPillar,
        ExtinctTorch,
        Rock,
        BrownRock,
        /* SMALL PROP */


        /* BIG PROPS */
        BlankShelf,
        BookShelf,
        BrokenShelf,
        BrownDualRock,
        BrownPillar1,
        BrownPillar2,
        BrownQuadRock,
        Box1,
        Box2,
        DualBox,
        EmptyStand,
        GreyDualRock,
        GreyPillar1,
        GreyPillar2,
        GreyQuadRock,
        LeftChain,
        RightChain,
        PotShelf,
        Table,
        Throne,
        WeaponShelf1,
        WeaponShelf2,
        /* BIG PROPS */
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
        Unknow,
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
        NewMap,
        UpdateSpells,
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
        char from[30];
        char message[1024];
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
    };

    struct PassTurn
    {
        gf::Id playerID;
    };

    struct UpdateSpells
    {
        SpellType spells[MAX_SPELL_PER_PLAYER];
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
            Message receiveMessage;
            UpdateSpells updateSpells;
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

            case PacketType::Message:
            {
                ar | packet.receiveMessage.from;
                ar | packet.receiveMessage.message;
                break;
            }

            case PacketType::UpdateSpells:
            {
                ar | packet.updateSpells.spells;
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

}

#endif
