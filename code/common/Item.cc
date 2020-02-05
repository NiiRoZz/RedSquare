#include "Item.h"

namespace redsquare
{
    Item::Item(ItemType type, uint baseFloor)
    : m_SlotMask(0)
    , m_Type(type)
    , m_Useable(false)
    , m_Name("")
    , m_Description("")
    , m_BaseFloor(baseFloor)
    {
        defaultCompatibleSlot();
        defaultData();
    }

    Item::Item(ItemType type, uint8_t slotMask, uint baseFloor)
    : m_SlotMask(slotMask)
    , m_Type(type)
    , m_Useable(false)
    , m_Name("")
    , m_Description("")
    , m_BaseFloor(baseFloor)
    {
        defaultData();
    }

    ItemType Item::getType() const
    {
        return m_Type;
    }

    void Item::addCompatibleSlot(InventorySlotType slotType)
    {
        m_SlotMask |= static_cast<uint8_t>(slotType);
    }

    void Item::removeCompatibleSlot(InventorySlotType slotType)
    {
        m_SlotMask &= ~static_cast<uint8_t>(slotType);
    }

    bool Item::canBeInSlot(InventorySlotType slotType)
    {
        return (m_SlotMask & static_cast<uint8_t>(slotType));
    }

    void Item::defaultCompatibleSlot()
    {
        switch (m_Type)
        {
            case ItemType::Staff1:
            case ItemType::Sword1: 
            case ItemType::Bow1: 
            case ItemType::SpellBook1: 
            {
                addCompatibleSlot(InventorySlotType::Weapon);
                break;
            }
        }

        addCompatibleSlot(InventorySlotType::Cargo);
    }

    int Item::Variance(int range){ // adding some rng to the damage of a spell [range;+range] added to the base damage of the spell
        if(range == 0){
            return 0;
        }
        int randomNum = rand() % (range*2) + (range);
        std::cout << "RANDOM :" << randomNum << std::endl;
        return randomNum;
    }

    void Item::defaultData()
    {
        m_GiveLifePoint = 0;
        m_GiveAttackPoint = 0;
        m_GiveDefensePoint = 0;

        switch (m_Type)
        {
            case ItemType::Sword1: case ItemType::Staff1: case ItemType::SpellBook1: case ItemType::Bow1:
                m_GiveAttackPoint = 2*m_BaseFloor;
                m_Name = "Basic Weapon";
                m_Description = "Basic weapon of the class " + std::to_string(m_GiveAttackPoint);
                break;
            case ItemType::Sword2: case ItemType::Staff2: case ItemType::SpellBook2: case ItemType::Bow2:
                m_GiveAttackPoint = 3*m_BaseFloor;
                break;
            case ItemType::Sword3: case ItemType::Staff3: case ItemType::SpellBook3: case ItemType::Bow3:
                m_GiveAttackPoint = 4*m_BaseFloor;
                break;
            case ItemType::Sword4: case ItemType::Staff4: case ItemType::SpellBook4: case ItemType::Bow4:
                m_GiveAttackPoint = 5*m_BaseFloor;
                m_GiveLifePoint = 3*m_BaseFloor;
                break;
            case ItemType::Sword5: case ItemType::Staff5: case ItemType::SpellBook5: case ItemType::Bow5:
                m_GiveAttackPoint = 6*m_BaseFloor;
                break;
            case ItemType::Sword6: case ItemType::Staff6: case ItemType::SpellBook6: case ItemType::Bow6:
                m_GiveLifePoint = 5*m_BaseFloor;
                m_GiveAttackPoint = 5*m_BaseFloor;
                break;
            case ItemType::Sword7: case ItemType::Staff7: case ItemType::SpellBook7: case ItemType::Bow7:
                m_GiveLifePoint = 8*m_BaseFloor;
                m_GiveAttackPoint = 4*m_BaseFloor;
                break;
            case ItemType::Sword8: case ItemType::Staff8: case ItemType::SpellBook8: case ItemType::Bow8:
                m_GiveLifePoint = 7*m_BaseFloor;
                m_GiveAttackPoint = 5*m_BaseFloor;
                break; 
            case ItemType::Sword9: case ItemType::Staff9: case ItemType::SpellBook9: case ItemType::Bow9:
                m_GiveLifePoint = 8*m_BaseFloor;
                break;
            case ItemType::Sword10: case ItemType::Staff10: case ItemType::SpellBook10: case ItemType::Bow10:
                m_GiveLifePoint = 3*m_BaseFloor;
                m_GiveAttackPoint = 3*m_BaseFloor;
                m_GiveDefensePoint = 3*m_BaseFloor;
                break;
            case ItemType::Sword11: case ItemType::Staff11: case ItemType::SpellBook11: case ItemType::Bow11:
                m_GiveLifePoint = 4*m_BaseFloor; 
                m_GiveAttackPoint = 4*m_BaseFloor;
                m_GiveDefensePoint = 3*m_BaseFloor;
                break;
            case ItemType::Sword12: case ItemType::Staff12: case ItemType::SpellBook12: case ItemType::Bow12:
                m_GiveLifePoint = 3*m_BaseFloor;
                m_GiveAttackPoint = 4*m_BaseFloor;
                m_GiveDefensePoint = 4*m_BaseFloor;
                break; 
            case ItemType::Sword13: case ItemType::Staff13: case ItemType::SpellBook13: case ItemType::Bow13:
                m_GiveAttackPoint = 8*m_BaseFloor;
                m_GiveDefensePoint = 8*m_BaseFloor;
                break;
            case ItemType::Sword14: case ItemType::Staff14: case ItemType::SpellBook14: case ItemType::Bow14:
                m_GiveAttackPoint = 10*m_BaseFloor;
                m_GiveDefensePoint = 5*m_BaseFloor;
                break;
            case ItemType::Sword15: case ItemType::Staff15: case ItemType::SpellBook15:case ItemType::Bow15:
                m_GiveAttackPoint = 10*m_BaseFloor;
                m_GiveLifePoint = 10*m_BaseFloor;
                break;

            case ItemType::HealthPot1:
                m_Useable = true;
                m_Description = "Basic potion of health, give : 25% of max lifepoint";
                break;
            case ItemType::HealthPot2:
                m_Useable = true;
                m_Description = "Medium potion of health, give : 50% of max lifepoint";
                break;
            case ItemType::HealthPot3:
                m_Useable = true;
                m_Description = "Strong potion of health, give : 70% of max lifepoint";
                break;
            case ItemType::ManaPot1:
                m_Useable = true;
                m_Description = "Basic potion of mana, give : 25% of max manapoint";
                break;
            case ItemType::ManaPot2:
                m_Useable = true;
                m_Description = "Medium potion of mana, give : 50% of max lifepoint";
                break;
            case ItemType::ManaPot3:
                m_Useable = true;
                m_Description = "Strong potion of mana, give : 70% of max lifepoint";
                break;
            case ItemType::BoostAttack1:
                m_Useable = true;
                m_Description = "Basic boost of attack, add 10% to your attackpoint";
                break;
            case ItemType::BoostAttack2:
                m_Useable = true;
                m_Description = "Medium boost of attack, add 15% to your attackpoint";
                break;
            case ItemType::BoostAttack3:
                m_Useable = true;
                m_Description = "Strong boost of attack, add 20% to your attackpoint";
                break;
            case ItemType::BoostDefense1:
                m_Useable = true;
                m_Description = "Basic boost of defense, add 10% to your attackpoint";
                break;
            case ItemType::BoostDefense2:
                m_Useable = true;
                m_Description = "Medium boost of defense, add 15% to your attackpoint";
                break;
            case ItemType::BoostDefense3:
                m_Useable = true;
                m_Description = "Strong boost of defense, add 20% to your attackpoint";
                break;
            case ItemType::BoostHP1:
                m_Useable = true;
                m_Description = "Basic boost of health, add 10% to your attackpoint";
                break;
            case ItemType::BoostHP2:
                m_Useable = true;
                m_Description = "Medium boost of health, add 15% to your attackpoint";
                break;
            case ItemType::BoostHP3:
                m_Useable = true;
                m_Description = "Strong boost of health, add 20% to your attackpoint";
                break;
            case ItemType::BoostXP1:
                m_Useable = true;
                m_Description = "Basic boost of XP, add 10% to your attackpoint";
                break;
            case ItemType::BoostXP2:
                m_Useable = true;
                m_Description = "Medium boost of XP, add 15% to your attackpoint";
                break;
            case ItemType::BoostXP3:
                m_Useable = true;
                m_Description = "Strong boost of XP, add 20% to your attackpoint";
                break;


            default:
                m_GiveLifePoint = 0;
                m_GiveAttackPoint = 0;
                m_GiveDefensePoint = 0;
                m_Useable = false;
                break;
        }

        m_GiveLifePoint += Variance(-(m_GiveLifePoint/10));
        m_GiveAttackPoint += Variance(-(m_GiveAttackPoint/10));
        m_GiveDefensePoint += Variance(-(m_GiveDefensePoint/10));
    }

    bool Item::isUseable() const
    {
        return m_Useable;
    }

    const std::string& Item::getName() const
    {
        return m_Name;
    }

    const std::string& Item::getDescription() const
    {
        return m_Description;
    }

    uint Item::getBaseFloor() const
    {
        return m_BaseFloor;
    }
}