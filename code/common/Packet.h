#ifndef REDSQUARE_COMMON_PACKET_H
#define REDSQUARE_COMMON_PACKET_H

#include <cstdint>
#include <iostream>
#include <gf/Id.h>
#include <gf/Vector.h>
#include <gf/Array2D.h>
#include <string>

#define MAX_SPELL_PER_PLAYER 8
#define MAX_SIZE_MESSAGE_CHAT 1024
#define MAX_SIZE_FROM_CHAT 30

namespace redsquare
{
    enum class EntityType: uint8_t
    {
        Player,
        Monster,
        Prop,
        ItemHolder,
    };

    enum class EntitySubType: uint8_t
    {
        Unknow,
        /* CLASSES */
        Magus,
        Warrior,
        Rogue,
        Ranger,
        Healer,
        // Paladin,
        // Healer,
        /* CLASSES */

        /* MONSTER */
        Bat,
        Slime,
        SkeletonKnife,
        SkeletonMagus,
        Spirit,
        /* MONSTER */

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
        RangeUp, // gain 1 range for x turns ---------- MAGUS / SHOOTER
        DoubleStrike, // IDK for now
        Heal, // heal a foes ------- PRIEST
        Assasinate, // can one shot a foes  with high miss rate ------- ROGUE
        DamageUp, // gain a bonus of attack for x turns ------- WARRIOR/ROGUE
        Protection, // protect an ally
        Revenge, // deal damage to foes based on missing health of source
        Lacerate, // deal damage to foes based on missing health of targer
        Incinerate, // deal damage to foes and can burn ennemy
        Massacre, // bleed dmg
        LightningStrike, // AOE dmg
        Scorch, // dmg + bleed
        Berserk, // gain attack, loose defence
        Torpedo, // 
        Shoot, // long attack range
        Backstab, // deal damage to foes : x2 if behind foes
        Reaper, // dmg in front of you
    };

    enum class InventorySlotType: uint8_t
    {
        Helmet = 1,
        ChestPlate = 2,
        Legging = 4,
        Boot = 8,
        Weapon = 16,
        Shield = 32,
        Cargo = 64,
    };

    enum class ItemType: uint16_t
    {
        //Should be the first one
        Unknow,
        
        Sword1,
        Sword2,
        Sword3,
        Sword4,
        Sword5,
        Sword6,
        Sword7,
        Sword8,
        Sword9,
        Sword10,
        Sword11,
        Sword12,
        Sword13,
        Sword14,
        Sword15,

        Staff1,
        Staff2,
        Staff3,
        Staff4,
        Staff5,
        Staff6,
        Staff7,
        Staff8,
        Staff9,
        Staff10,
        Staff11,
        Staff12,
        Staff13,
        Staff14,
        Staff15,

        Bow1,
        Bow2,
        Bow3,
        Bow4,
        Bow5,
        Bow6,
        Bow7,
        Bow8,
        Bow9,
        Bow10,
        Bow11,
        Bow12,
        Bow13,
        Bow14,
        Bow15,

        SpellBook1,
        SpellBook2,
        SpellBook3,
        SpellBook4,
        SpellBook5,
        SpellBook6,
        SpellBook7,
        SpellBook8,
        SpellBook9,
        SpellBook10,
        SpellBook11,
        SpellBook12,
        SpellBook13,
        SpellBook14,
        SpellBook15,

        Shield1,
        Shield2,
        Shield3,
        Shield4,
        Shield5,
        Shield6,
        Shield7,
        Shield8,
        Shield9,
        Shield10,
        Shield11,
        Shield12,
        Shield13,
        Shield14,
        Shield15,

        Helmet1,
        Helmet2,
        Helmet3,
        Helmet4,
        Helmet5,
        Helmet6,
        Helmet7,
        Helmet8,
        Helmet9,
        Helmet10,

        Chesplate1,
        Chesplate2,
        Chesplate3,
        Chesplate4,
        Chesplate5,
        Chesplate6,
        Chesplate7,
        Chesplate8,
        Chesplate9,
        Chesplate10,
        Chesplate11,
        Chesplate12,
        Chesplate13,
        Chesplate14,
        Chesplate15,


        Legging,
        Boots,
        // ARMOR

        // CONSUMABLES
        ManaPot1,
        ManaPot2,
        ManaPot3,

        HealthPot1,
        HealthPot2,
        HealthPot3,

        EnergyPot1,
        EnergyPot2,
        EnergyPot3,
        
        BoostAttack1,
        BoostAttack2,
        BoostAttack3,
        
        BoostDefense1,
        BoostDefense2,
        BoostDefense3,
        
        BoostHP1,
        BoostHP2,
        BoostHP3,
        
        BoostMana1,
        BoostMana2,
        BoostMana3,
        
        BoostXP1,
        BoostXP2,
        BoostXP3,

        Fish,
        Candy,
        // CONSUMABLES
    };

    enum class PacketType : uint16_t
    {
        PlayerInfoConnection,
        RequestMove,
        ReceiveMove,
        RequestAttack,
        RequestUse,
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
        gf::Id playerID;
        ItemType type;
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
                ar | packet.requestUse.playerID;
                ar | packet.requestUse.type;
                ar | packet.requestUse.pos;
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
        return ar;
    }

}

#endif
