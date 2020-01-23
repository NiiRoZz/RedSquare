#include "Item.h"

namespace redsquare
{
    Item::Item(ItemType type)
    : m_SlotMask(0)
    , m_Type(type)
    , m_Useable(false)
    , m_Name("")
    , m_Description("")
    {
        defaultCompatibleSlot();
        defaultData(1); // todo floor
    }

    Item::Item(ItemType type, uint8_t slotMask)
    : m_SlotMask(slotMask)
    , m_Type(type)
    , m_Useable(false)
    , m_Name("")
    , m_Description("")
    {
        defaultData(1); // todo floor
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

    void Item::defaultData(int floor)
    {
        switch (m_Type)
        {
            case ItemType::Sword1: case ItemType::Staff1: case ItemType::SpellBook1: case ItemType::Bow1:
                m_GiveAttackPoint = 2*floor;
                break;
            case ItemType::Sword2: case ItemType::Staff2: case ItemType::SpellBook2: case ItemType::Bow2:
                m_GiveAttackPoint = 3*floor;
                break;
            case ItemType::Sword3: case ItemType::Staff3: case ItemType::SpellBook3: case ItemType::Bow3:
                m_GiveAttackPoint = 4*floor;
                break;
            case ItemType::Sword4: case ItemType::Staff4: case ItemType::SpellBook4: case ItemType::Bow4:
                m_GiveAttackPoint = 5*floor;
                m_GiveLifePoint = 3*floor;
                break;
            case ItemType::Sword5: case ItemType::Staff5: case ItemType::SpellBook5: case ItemType::Bow5:
                m_GiveAttackPoint = 6*floor;
                break;
            case ItemType::Sword6: case ItemType::Staff6: case ItemType::SpellBook6: case ItemType::Bow6:
                m_GiveLifePoint = 5*floor;
                m_GiveAttackPoint = 5*floor;
                break;
            case ItemType::Sword7: case ItemType::Staff7: case ItemType::SpellBook7: case ItemType::Bow7:
                m_GiveLifePoint = 8*floor;
                m_GiveAttackPoint = 4*floor;
                break;
            case ItemType::Sword8: case ItemType::Staff8: case ItemType::SpellBook8: case ItemType::Bow8:
                m_GiveLifePoint = 7*floor;
                m_GiveAttackPoint = 5*floor;
                break; 
            case ItemType::Sword9: case ItemType::Staff9: case ItemType::SpellBook9: case ItemType::Bow9:
                m_GiveLifePoint = 8*floor;
                break;
            case ItemType::Sword10: case ItemType::Staff10: case ItemType::SpellBook10: case ItemType::Bow10:
                m_GiveLifePoint = 3*floor;
                m_GiveAttackPoint = 3*floor;
                m_GiveDefensePoint = 3*floor;
                break;
            case ItemType::Sword11: case ItemType::Staff11: case ItemType::SpellBook11: case ItemType::Bow11:
                m_GiveLifePoint = 4*floor; 
                m_GiveAttackPoint = 4*floor;
                m_GiveDefensePoint = 3*floor;
                break;
            case ItemType::Sword12: case ItemType::Staff12: case ItemType::SpellBook12: case ItemType::Bow12:
                m_GiveLifePoint = 3*floor;
                m_GiveAttackPoint = 4*floor;
                m_GiveDefensePoint = 4*floor;
                break; 
            case ItemType::Sword13: case ItemType::Staff13: case ItemType::SpellBook13: case ItemType::Bow13:
                m_GiveAttackPoint = 8*floor;
                m_GiveDefensePoint = 8*floor;
                break;
            case ItemType::Sword14: case ItemType::Staff14: case ItemType::SpellBook14: case ItemType::Bow14:
                m_GiveAttackPoint = 10*floor;
                m_GiveDefensePoint = 5*floor;
                break;
            case ItemType::Sword15: case ItemType::Staff15: case ItemType::SpellBook15:case ItemType::Bow15:
                m_GiveAttackPoint = 10*floor;
                m_GiveLifePoint = 10*floor;
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
}