#ifndef REDSQUARE_PROTOCOL_DATA_H
#define REDSQUARE_PROTOCOL_DATA_H

#include <string>
#include <vector>
#include <assert.h>

#include <gf/Id.h>
#include <gf/Vector.h>

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
        /* CLASSES */

        /* MONSTER */
        Bat,
        Slime,
        SkeletonKnife,
        SkeletonMagus,
        Spirit,
        Demon,
        Goblin,
        Imp,
        LilGob,
        LilZombie,
        Lizard,
        Mask,
        Mud,
        Orc,
        Shaman,
        Swamp,
        Zombie,
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

    inline std::string entitySubTypeString(EntitySubType type)
    {
        switch (type)
        {
            case EntitySubType::Magus:
            return "Magus";

            case EntitySubType::Warrior:
            return "Warrior";

            case EntitySubType::Rogue:
            return "Rogue";

            case EntitySubType::Ranger:
            return "Ranger";

            case EntitySubType::Healer:
            return "Healer";

            default:
            return "";
        }

        assert(false);
        return "";
    }

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
        Helmet11,
        Helmet12,
        Helmet13,
        Helmet14,
        Helmet15,

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

        Legging1,
        Legging2,
        Legging3,
        Legging4,
        Legging5,
        Legging6,
        Legging7,
        Legging8,
        Legging9,
        Legging10,
        Legging11,
        Legging12,
        Legging13,
        Legging14,
        Legging15,

        Boot1,
        Boot2,
        Boot3,
        Boot4,
        Boot5,
        Boot6,
        Boot7,
        Boot8,
        Boot9,
        Boot10,
        Boot11,
        Boot12,
        Boot13,
        Boot14,
        Boot15,

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
    
    struct PlayerData
    {
        gf::Id id;
        std::string name;
        EntitySubType subType = EntitySubType::Magus;
        bool ready = false;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, PlayerData& data)
    {
        return ar | data.id | data.name | data.subType | data.ready;
    }

    struct RoomData
    {
        gf::Id id;
        std::string name;
        int32_t players;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RoomData& data)
    {
        return ar | data.id | data.name | data.players;
    }

    struct MessageData
    {
        gf::Id origin;
        std::string author;
        std::string content;
        gf::Id recipient = gf::InvalidId;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, MessageData& data)
    {
        return ar | data.origin | data.author | data.content | data.recipient;
    }

    struct EntityCharacteristicData
    {
        gf::Id id;
        EntityType entityType;

        int m_LifePoint;
        int m_MaxLifePoint;

        int m_ManaPoint = 0;
        int m_MaxManaPoint = 0;

        int m_AttackPoint;

        int m_DefensePoint;

        int m_Range;

        int m_XP = 0;
        int m_MaxXP = 0;

        int m_Level;

        std::vector<SpellType> m_Spells;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, EntityCharacteristicData& data)
    {
        return ar | data.id | data.entityType | data.m_LifePoint | data.m_MaxLifePoint | data.m_ManaPoint | data.m_MaxManaPoint | data.m_AttackPoint | data.m_DefensePoint | data.m_Range | data.m_XP | data.m_MaxXP | data.m_Level | data.m_Spells;
    }

    struct EntityData
    {
        gf::Id id = gf::InvalidId;
        EntityType entityType;
        EntitySubType entitySubType;
        gf::Vector2i pos;
        ItemType holdingItem;
        EntityCharacteristicData characteristics;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, EntityData& data)
    {
        return ar | data.id | data.entityType | data.entitySubType | data.pos | data.holdingItem | data.characteristics;
    }

    struct EntityMove
    {
        gf::Id id = gf::InvalidId;
        EntityType entityType;
        gf::Vector2i pos;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, EntityMove& data)
    {
        return ar | data.id | data.entityType | data.pos;
    }
}

#endif // REDSQUARE_PROTOCOL_DATA_H
