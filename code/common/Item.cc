#include "Item.h"

#include <iostream>

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
            case ItemType::Staff2:
            case ItemType::Sword2: 
            case ItemType::Bow2: 
            case ItemType::SpellBook2: 
            case ItemType::Staff3:
            case ItemType::Sword3: 
            case ItemType::Bow3: 
            case ItemType::SpellBook3:  
            case ItemType::Staff4:
            case ItemType::Sword4: 
            case ItemType::Bow4: 
            case ItemType::SpellBook4:  
            case ItemType::Staff5:
            case ItemType::Sword5: 
            case ItemType::Bow5: 
            case ItemType::SpellBook5:  
            case ItemType::Staff6:
            case ItemType::Sword6: 
            case ItemType::Bow6: 
            case ItemType::SpellBook6:  
            case ItemType::Staff7:
            case ItemType::Sword7: 
            case ItemType::Bow7: 
            case ItemType::SpellBook7:  
            case ItemType::Staff8:
            case ItemType::Sword8: 
            case ItemType::Bow8: 
            case ItemType::SpellBook8:  
            case ItemType::Staff9:
            case ItemType::Sword9: 
            case ItemType::Bow9: 
            case ItemType::SpellBook9:  
            case ItemType::Staff10:
            case ItemType::Sword10: 
            case ItemType::Bow10: 
            case ItemType::SpellBook10:  
            case ItemType::Staff11:
            case ItemType::Sword11: 
            case ItemType::Bow11: 
            case ItemType::SpellBook11:  
            case ItemType::Staff12:
            case ItemType::Sword12: 
            case ItemType::Bow12: 
            case ItemType::SpellBook12:  
            case ItemType::Staff13:
            case ItemType::Sword13: 
            case ItemType::Bow14: 
            case ItemType::SpellBook14:  
            case ItemType::Staff15:
            case ItemType::Sword15: 
            case ItemType::Bow15: 
            case ItemType::SpellBook15:   
            {
                addCompatibleSlot(InventorySlotType::Weapon);
                break;
            }

            case ItemType::Helmet1:
            case ItemType::Helmet2:
            case ItemType::Helmet3:
            case ItemType::Helmet4:
            case ItemType::Helmet5:
            case ItemType::Helmet6:
            case ItemType::Helmet7:
            case ItemType::Helmet8:
            case ItemType::Helmet9:
            case ItemType::Helmet10:
            case ItemType::Helmet11:
            case ItemType::Helmet12:
            case ItemType::Helmet13:
            case ItemType::Helmet14:
            case ItemType::Helmet15: 
            {
                addCompatibleSlot(InventorySlotType::Helmet);
                break;
            }

            case ItemType::Chesplate1:
            case ItemType::Chesplate2:
            case ItemType::Chesplate3:
            case ItemType::Chesplate4:
            case ItemType::Chesplate5:
            case ItemType::Chesplate6:
            case ItemType::Chesplate7:
            case ItemType::Chesplate8:
            case ItemType::Chesplate9:
            case ItemType::Chesplate10:
            case ItemType::Chesplate11:
            case ItemType::Chesplate12:
            case ItemType::Chesplate13:
            case ItemType::Chesplate14:
            case ItemType::Chesplate15: 
            {
                addCompatibleSlot(InventorySlotType::ChestPlate);
                break;
            }

            case ItemType::Legging1:
            case ItemType::Legging2:
            case ItemType::Legging3:
            case ItemType::Legging4:
            case ItemType::Legging5:
            case ItemType::Legging6:
            case ItemType::Legging7:
            case ItemType::Legging8:
            case ItemType::Legging9:
            case ItemType::Legging10:
            case ItemType::Legging11:
            case ItemType::Legging12:
            case ItemType::Legging13:
            case ItemType::Legging14:
            case ItemType::Legging15: 
            {
                addCompatibleSlot(InventorySlotType::Legging);
                break;
            }
            case ItemType::Boot1:
            case ItemType::Boot2:
            case ItemType::Boot3:
            case ItemType::Boot4:
            case ItemType::Boot5:
            case ItemType::Boot6:
            case ItemType::Boot7:
            case ItemType::Boot8:
            case ItemType::Boot9:
            case ItemType::Boot10:
            case ItemType::Boot11:
            case ItemType::Boot12:
            case ItemType::Boot13:
            case ItemType::Boot14:
            case ItemType::Boot15: 
            {
                addCompatibleSlot(InventorySlotType::Boot);
                break;
            }

            case ItemType::Shield1:
            case ItemType::Shield2:
            case ItemType::Shield3:
            case ItemType::Shield4:
            case ItemType::Shield5:
            case ItemType::Shield6:
            case ItemType::Shield7:
            case ItemType::Shield8:
            case ItemType::Shield9:
            case ItemType::Shield10:
            case ItemType::Shield11:
            case ItemType::Shield12:
            case ItemType::Shield13:
            case ItemType::Shield14:
            case ItemType::Shield15: 
            {
                addCompatibleSlot(InventorySlotType::Shield);
                break;
            }

            default:
                break;
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
        m_GiveManaPoint = 0;
        m_GiveAttackPoint = 0;
        m_GiveDefensePoint = 0;
        m_GiveManaPoint = 0;

        switch (m_Type)
        {
            case ItemType::Sword1: case ItemType::Staff1: case ItemType::SpellBook1: case ItemType::Bow1: case ItemType::Shield1:
                m_GiveAttackPoint = m_BaseFloor;
                if(m_Type == ItemType::Sword1){ m_Name = "Bronze Sword"; }
                if(m_Type == ItemType::Staff1){ m_Name = "Bronze Staff"; }
                if(m_Type == ItemType::SpellBook1){ m_Name = "Book of Suffering"; }
                if(m_Type == ItemType::Bow1){ m_Name = "Bronze Bow"; }
                if(m_Type == ItemType::Shield1){ m_Name = "Bronze Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint);
                break;
            case ItemType::Sword2: case ItemType::Staff2: case ItemType::SpellBook2: case ItemType::Bow2: case ItemType::Shield2:
                m_GiveAttackPoint = 2*m_BaseFloor;
                if(m_Type == ItemType::Sword2){ m_Name = "Silver Sword"; }
                if(m_Type == ItemType::Staff2){ m_Name = "Silver Staff"; }
                if(m_Type == ItemType::SpellBook2){ m_Name = "Book of Conjuration"; }
                if(m_Type == ItemType::Bow2){ m_Name = "Silver Bow"; }
                if(m_Type == ItemType::Shield2){ m_Name = "Holy Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint);
                break;
            case ItemType::Sword3: case ItemType::Staff3: case ItemType::SpellBook3: case ItemType::Bow3: case ItemType::Shield3:
                m_GiveAttackPoint = 3*m_BaseFloor;
                m_GiveManaPoint = 3*m_BaseFloor;
                if(m_Type == ItemType::Sword3){ m_Name = "Iron Sword"; }
                if(m_Type == ItemType::Staff3){ m_Name = "Iron Staff"; }
                if(m_Type == ItemType::SpellBook3){ m_Name = "Book of Destruction"; }
                if(m_Type == ItemType::Bow3){ m_Name = "Iron Bow"; }
                if(m_Type == ItemType::Shield3){ m_Name = "Saphir Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+ "\nMana : +"+std::to_string(m_GiveManaPoint);
                break;
            case ItemType::Sword4: case ItemType::Staff4: case ItemType::SpellBook4: case ItemType::Bow4: case ItemType::Shield4:
                m_GiveAttackPoint = 4*m_BaseFloor;
                m_GiveLifePoint = 2*m_BaseFloor;
                if(m_Type == ItemType::Sword4){ m_Name = "Golden Sword"; }
                if(m_Type == ItemType::Staff4){ m_Name = "Golden Staff"; }
                if(m_Type == ItemType::SpellBook4){ m_Name = "Book of Illusion"; }
                if(m_Type == ItemType::Bow4){ m_Name = "Golden Bow"; }
                if(m_Type == ItemType::Shield4){ m_Name = "Golden Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+ "\nHP : +"+std::to_string(m_GiveLifePoint);
                break;
            case ItemType::Sword5: case ItemType::Staff5: case ItemType::SpellBook5: case ItemType::Bow5: case ItemType::Shield5:
                m_GiveAttackPoint = 5*m_BaseFloor;
                if(m_Type == ItemType::Sword5){ m_Name = "Blood Sword"; }
                if(m_Type == ItemType::Staff5){ m_Name = "Blood Staff"; }
                if(m_Type == ItemType::SpellBook5){ m_Name = "Book of Necromancy"; }
                if(m_Type == ItemType::Bow5){ m_Name = "Blood Bow"; }
                if(m_Type == ItemType::Shield5){ m_Name = "Blood Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint);
                break;
            case ItemType::Sword6: case ItemType::Staff6: case ItemType::SpellBook6: case ItemType::Bow6:  case ItemType::Shield6:
                m_GiveLifePoint = 4*m_BaseFloor;
                m_GiveAttackPoint = 4*m_BaseFloor;
                m_GiveManaPoint = 4*m_BaseFloor;
                if(m_Type == ItemType::Sword6){ m_Name = "Long Bronze Sword"; }
                if(m_Type == ItemType::Staff6){ m_Name = "Strong Bronze Staff"; }
                if(m_Type == ItemType::SpellBook6){ m_Name = "Great Book of Suffering"; }
                if(m_Type == ItemType::Bow6){ m_Name = "Strong Bronze Bow"; }
                if(m_Type == ItemType::Shield6){ m_Name = "Heavy Bronze Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+ "\nHP : +" + std::to_string(m_GiveLifePoint)+ "\nMana : +"+std::to_string(m_GiveManaPoint);
                break;
            case ItemType::Sword7: case ItemType::Staff7: case ItemType::SpellBook7: case ItemType::Bow7:  case ItemType::Shield7:
                m_GiveLifePoint = 6*m_BaseFloor;
                m_GiveAttackPoint = 4*m_BaseFloor;
                if(m_Type == ItemType::Sword7){ m_Name = "Long Silver Sword"; }
                if(m_Type == ItemType::Staff7){ m_Name = "Strong Silver Staff"; }
                if(m_Type == ItemType::SpellBook7){ m_Name = "Great Book of Conjuration"; }
                if(m_Type == ItemType::Bow7){ m_Name = "Strong Silver Bow"; }
                if(m_Type == ItemType::Shield7){ m_Name = "Heavy Holy Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+ "\nHP : +"+std::to_string(m_GiveLifePoint);
                break;
            case ItemType::Sword8: case ItemType::Staff8: case ItemType::SpellBook8: case ItemType::Bow8:  case ItemType::Shield8:
                m_GiveLifePoint = 5*m_BaseFloor;
                m_GiveAttackPoint = 4*m_BaseFloor;
                m_GiveManaPoint = 2*m_BaseFloor;
                if(m_Type == ItemType::Sword8){ m_Name = "Long Iron Sword"; }
                if(m_Type == ItemType::Staff8){ m_Name = "Strong Iron Staff"; }
                if(m_Type == ItemType::SpellBook8){ m_Name = "Great Book of Destruction"; }
                if(m_Type == ItemType::Bow8){ m_Name = "Strong Iron Bow"; }
                if(m_Type == ItemType::Shield8){ m_Name = "Heavy Saphir Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+ "\nHP : +" + std::to_string(m_GiveLifePoint)+ "\nMana : +"+std::to_string(m_GiveManaPoint);
                break; 
            case ItemType::Sword9: case ItemType::Staff9: case ItemType::SpellBook9: case ItemType::Bow9:  case ItemType::Shield9:
                m_GiveLifePoint = 6*m_BaseFloor;
                if(m_Type == ItemType::Sword9){ m_Name = "Long Golden Sword"; }
                if(m_Type == ItemType::Staff9){ m_Name = "Strong Golden Staff"; }
                if(m_Type == ItemType::SpellBook9){ m_Name = "Great Book of Illusion"; }
                if(m_Type == ItemType::Bow9){ m_Name = "Strong Golden Bow"; }
                if(m_Type == ItemType::Shield9){ m_Name = "Heavy Golden Shield"; }
                m_Description = "HP : +" + std::to_string(m_GiveLifePoint);
                break;
            case ItemType::Sword10: case ItemType::Staff10: case ItemType::SpellBook10: case ItemType::Bow10:  case ItemType::Shield10:
                m_GiveLifePoint = 3*m_BaseFloor;
                m_GiveAttackPoint = 3*m_BaseFloor;
                m_GiveDefensePoint = 3*m_BaseFloor;
                m_GiveManaPoint = 3*m_BaseFloor;
                if(m_Type == ItemType::Sword10){ m_Name = "Long Blood Sword"; }
                if(m_Type == ItemType::Staff10){ m_Name = "Strong Blood Staff"; }
                if(m_Type == ItemType::SpellBook10){ m_Name = "Great Book of Necromancy"; }
                if(m_Type == ItemType::Bow10){ m_Name = "Strong Blood Bow"; }
                if(m_Type == ItemType::Shield10){ m_Name = "Heavy Blood Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+ "\nHP : +" + std::to_string(m_GiveLifePoint)+ "\nMana : +"+std::to_string(m_GiveManaPoint)+ "\nDefense : +"+std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Sword11: case ItemType::Staff11: case ItemType::SpellBook11: case ItemType::Bow11:  case ItemType::Shield11:
                m_GiveLifePoint = 4*m_BaseFloor; 
                m_GiveAttackPoint = 4*m_BaseFloor;
                m_GiveDefensePoint = 3*m_BaseFloor;
                if(m_Type == ItemType::Sword11){ m_Name = "Large Bronze Sword"; }
                if(m_Type == ItemType::Staff11){ m_Name = "Antic Blood Staff"; }
                if(m_Type == ItemType::SpellBook11){ m_Name = "Superior Book of Suffering"; }
                if(m_Type == ItemType::Bow11){ m_Name = "Long Blood Bow"; }
                if(m_Type == ItemType::Shield11){ m_Name = "Strong Bronze Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+ "\nHP : +" + std::to_string(m_GiveLifePoint)+ "\nMana : +"+std::to_string(m_GiveManaPoint)+ "\nDefense : +"+std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Sword12: case ItemType::Staff12: case ItemType::SpellBook12: case ItemType::Bow12:  case ItemType::Shield12:
                m_GiveLifePoint = 3*m_BaseFloor;
                m_GiveAttackPoint = 4*m_BaseFloor;
                m_GiveDefensePoint = 4*m_BaseFloor;
                m_GiveManaPoint = 3*m_BaseFloor;
                if(m_Type == ItemType::Sword12){ m_Name = "Large Silver Sword"; }
                if(m_Type == ItemType::Staff12){ m_Name = "Antic Silver Staff"; }
                if(m_Type == ItemType::SpellBook12){ m_Name = "Superior Book of Conjuration"; }
                if(m_Type == ItemType::Bow12){ m_Name = "Long Silver Bow"; }
                if(m_Type == ItemType::Shield12){ m_Name = "Strong Holy Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+ "\nHP : +" + std::to_string(m_GiveLifePoint)+ "\nMana : +"+std::to_string(m_GiveManaPoint)+ "\nDefense : +"+std::to_string(m_GiveDefensePoint);
                break; 
            case ItemType::Sword13: case ItemType::Staff13: case ItemType::SpellBook13: case ItemType::Bow13:  case ItemType::Shield13:
                m_GiveAttackPoint = 5*m_BaseFloor;
                m_GiveDefensePoint = 5*m_BaseFloor;
                if(m_Type == ItemType::Sword13){ m_Name = "Large Iron Sword"; }
                if(m_Type == ItemType::Staff13){ m_Name = "Antic Iron Staff"; }
                if(m_Type == ItemType::SpellBook13){ m_Name = "Superior Book of Destruction"; }
                if(m_Type == ItemType::Bow13){ m_Name = "Long Iron Bow"; }
                if(m_Type == ItemType::Shield13){ m_Name = "Strong Saphir Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint) + "\nDefense : +" + std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Sword14: case ItemType::Staff14: case ItemType::SpellBook14: case ItemType::Bow14:  case ItemType::Shield14:
                m_GiveAttackPoint = 7*m_BaseFloor;
                m_GiveDefensePoint = 4*m_BaseFloor;
                m_GiveManaPoint = 4*m_BaseFloor;
                if(m_Type == ItemType::Sword14){ m_Name = "Large Golden Sword"; }
                if(m_Type == ItemType::Staff14){ m_Name = "Antic Golden Staff"; }
                if(m_Type == ItemType::SpellBook14){ m_Name = "Superior Book of Necromancy"; }
                if(m_Type == ItemType::Bow14){ m_Name = "Long Golden Bow"; }
                if(m_Type == ItemType::Shield14){ m_Name = "Strong Golden Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint) + "\nMana : +" + std::to_string(m_GiveManaPoint) + "\nDefense : +" + std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Sword15: case ItemType::Staff15: case ItemType::SpellBook15:case ItemType::Bow15: case ItemType::Shield15:
                m_GiveAttackPoint = 8*m_BaseFloor;
                m_GiveLifePoint = 8*m_BaseFloor;
                m_GiveManaPoint = 8*m_BaseFloor;
                if(m_Type == ItemType::Sword15){ m_Name = "Large Blood Sword"; }
                if(m_Type == ItemType::Staff15){ m_Name = "Antic Blood Staff"; }
                if(m_Type == ItemType::SpellBook15){ m_Name = "Superior Book of Necromancy"; }
                if(m_Type == ItemType::Bow15){ m_Name = "Long Blood Bow"; }
                if(m_Type == ItemType::Shield15){ m_Name = "Strong Blood Shield"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint) + "\nHP : +" + std::to_string(m_GiveLifePoint) + "\nMana : +" + std::to_string(m_GiveManaPoint);
                break;

            case ItemType::Helmet1: case ItemType::Chesplate1: case ItemType::Legging1: case ItemType::Boot1:
                m_GiveDefensePoint = m_BaseFloor;   
                if(m_Type == ItemType::Helmet1){ m_Name = "Leather Helmet"; }
                if(m_Type == ItemType::Chesplate1){ m_Name = "Leather Chesplate"; }
                if(m_Type == ItemType::Legging1){ m_Name = "Leather Legging"; }
                if(m_Type == ItemType::Boot1){ m_Name = "Leather Boot"; }
                m_Description = "Defense : +" + std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Helmet2: case ItemType::Chesplate2: case ItemType::Legging2: case ItemType::Boot2:
                m_GiveDefensePoint = m_BaseFloor; 
                m_GiveLifePoint = m_BaseFloor;    
                if(m_Type == ItemType::Helmet2){ m_Name = "Bronze Helmet"; }
                if(m_Type == ItemType::Chesplate2){ m_Name = "Bronze Chesplate"; }
                if(m_Type == ItemType::Legging2){ m_Name = "Bronze Legging"; }
                if(m_Type == ItemType::Boot2){ m_Name = "Bronze Boot"; }
                m_Description = "HP : +" + std::to_string(m_GiveLifePoint) + "\nDefense : +" + std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Helmet3: case ItemType::Chesplate3: case ItemType::Legging3: case ItemType::Boot3:
                m_GiveDefensePoint =  2 * m_BaseFloor; 
                m_GiveLifePoint = 2 * m_BaseFloor;   
                if(m_Type == ItemType::Helmet3){ m_Name = "Sky Helmet"; }
                if(m_Type == ItemType::Chesplate3){ m_Name = "Sky Chesplate"; }
                if(m_Type == ItemType::Legging3){ m_Name = "Sky Legging"; }
                if(m_Type == ItemType::Boot3){ m_Name = "Sky Boot"; }
                m_Description = "HP : +" + std::to_string(m_GiveLifePoint) + "\nDefense : +" + std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Helmet4: case ItemType::Chesplate4: case ItemType::Legging4: case ItemType::Boot4:
                m_GiveDefensePoint = 2 * m_BaseFloor; 
                m_GiveLifePoint = 2 * m_BaseFloor;   
                m_GiveManaPoint = 2 * m_BaseFloor;   
                if(m_Type == ItemType::Helmet4){ m_Name = "Duality Helmet"; }
                if(m_Type == ItemType::Chesplate4){ m_Name = "Duality Chesplate"; }
                if(m_Type == ItemType::Legging4){ m_Name = "Duality Legging"; }
                if(m_Type == ItemType::Boot4){ m_Name = "Duality Boot"; } 
                m_Description = "HP : +" + std::to_string(m_GiveLifePoint)+ "\nMana : +" + std::to_string(m_GiveManaPoint) + "\nDefense : +" + std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Helmet5: case ItemType::Chesplate5: case ItemType::Legging5: case ItemType::Boot5:
                m_GiveDefensePoint = 3 * m_BaseFloor; 
                m_GiveLifePoint = 4 * m_BaseFloor;    
                if(m_Type == ItemType::Helmet5){ m_Name = "Lightning Helmet"; }
                if(m_Type == ItemType::Chesplate5){ m_Name = "Lightning Chesplate"; }
                if(m_Type == ItemType::Legging5){ m_Name = "Lightning Legging"; }
                if(m_Type == ItemType::Boot5){ m_Name = "Lightning Boot"; } 
                m_Description = "HP : +" + std::to_string(m_GiveLifePoint) + "\nDefense : +" + std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Helmet6: case ItemType::Chesplate6: case ItemType::Legging6: case ItemType::Boot6:
                m_GiveAttackPoint = 2 * m_BaseFloor;  
                m_GiveDefensePoint = 2 * m_BaseFloor; 
                m_GiveLifePoint = 2 * m_BaseFloor;   
                m_GiveManaPoint = 2 * m_BaseFloor;  
                if(m_Type == ItemType::Helmet6){ m_Name = "Shadow Helmet"; }
                if(m_Type == ItemType::Chesplate6){ m_Name = "Shadow Chesplate"; }
                if(m_Type == ItemType::Legging6){ m_Name = "Shadow Legging"; }
                if(m_Type == ItemType::Boot6){ m_Name = "Shadow Boot"; }    
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+ "\nHP : +" + std::to_string(m_GiveLifePoint)+ "\nMana : +"+std::to_string(m_GiveManaPoint)+ "\nDefense : +"+std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Helmet7: case ItemType::Chesplate7: case ItemType::Legging7: case ItemType::Boot7:
                m_GiveAttackPoint = 3 * m_BaseFloor;  
                m_GiveManaPoint = 3 * m_BaseFloor;  
                if(m_Type == ItemType::Helmet7){ m_Name = "Angel Helmet"; }
                if(m_Type == ItemType::Chesplate7){ m_Name = "Angel Chesplate"; }
                if(m_Type == ItemType::Legging7){ m_Name = "Angel Legging"; }
                if(m_Type == ItemType::Boot7){ m_Name = "Angel Boot"; }   
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+"\nMana : +"+std::to_string(m_GiveManaPoint);
                break;
            case ItemType::Helmet8: case ItemType::Chesplate8: case ItemType::Legging8: case ItemType::Boot8:
                m_GiveAttackPoint = 3 * m_BaseFloor;  
                m_GiveManaPoint = 3 * m_BaseFloor;   
                if(m_Type == ItemType::Helmet8){ m_Name = "Anthracite Helmet"; }
                if(m_Type == ItemType::Chesplate8){ m_Name = "Anthracite Chesplate"; }
                if(m_Type == ItemType::Legging8){ m_Name = "Anthracite Legging"; }
                if(m_Type == ItemType::Boot8){ m_Name = "Anthracite Boot"; } 
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+"\nMana : +"+std::to_string(m_GiveManaPoint);
                break;
            case ItemType::Helmet9: case ItemType::Chesplate9: case ItemType::Legging9: case ItemType::Boot9:
                m_GiveAttackPoint = 2 * m_BaseFloor;  
                m_GiveManaPoint = 4 * m_BaseFloor;  
                if(m_Type == ItemType::Helmet9){ m_Name = "Elf Helmet"; }
                if(m_Type == ItemType::Chesplate9){ m_Name = "Elf Chesplate"; }
                if(m_Type == ItemType::Legging9){ m_Name = "Elf Legging"; }
                if(m_Type == ItemType::Boot9){ m_Name = "Elf Boot"; } 
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+"\nMana : +"+std::to_string(m_GiveManaPoint);
                break;
            case ItemType::Helmet10: case ItemType::Chesplate10: case ItemType::Legging10: case ItemType::Boot10:
                m_GiveDefensePoint = 3 * m_BaseFloor;  
                m_GiveLifePoint = 4 * m_BaseFloor; 
                if(m_Type == ItemType::Helmet10){ m_Name = "Blood Helmet"; }
                if(m_Type == ItemType::Chesplate10){ m_Name = "Blood Chesplate"; }
                if(m_Type == ItemType::Legging10){ m_Name = "Blood Legging"; }
                if(m_Type == ItemType::Boot10){ m_Name = "Blood Boot"; }
                m_Description = "HP : +" + std::to_string(m_GiveLifePoint) + "\nDefense : +" + std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Helmet11: case ItemType::Chesplate11: case ItemType::Legging11: case ItemType::Boot11:
                m_GiveAttackPoint = 3 * m_BaseFloor;
                m_GiveDefensePoint = 3 * m_BaseFloor;  
                m_GiveLifePoint = 3 * m_BaseFloor;    
                if(m_Type == ItemType::Helmet11){ m_Name = "Ranger Helmet"; }
                if(m_Type == ItemType::Chesplate11){ m_Name = "Ranger Chesplate"; }
                if(m_Type == ItemType::Legging11){ m_Name = "Ranger Legging"; }
                if(m_Type == ItemType::Boot11){ m_Name = "Ranger Boot"; } 
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+"\nHP : +" + std::to_string(m_GiveLifePoint) + "\nDefense : +" + std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Helmet12: case ItemType::Chesplate12: case ItemType::Legging12: case ItemType::Boot12:
                m_GiveAttackPoint = 3 * m_BaseFloor;
                m_GiveDefensePoint = 3 * m_BaseFloor;  
                m_GiveLifePoint = 4 * m_BaseFloor;  
                if(m_Type == ItemType::Helmet12){ m_Name = "Solar Helmet"; }
                if(m_Type == ItemType::Chesplate12){ m_Name = "Solar Chesplate"; }
                if(m_Type == ItemType::Legging12){ m_Name = "Solar Legging"; }
                if(m_Type == ItemType::Boot12){ m_Name = "Solar Boot"; } 
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+"\nHP : +" + std::to_string(m_GiveLifePoint) + "\nDefense : +" + std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Helmet13: case ItemType::Chesplate13: case ItemType::Legging13: case ItemType::Boot13:
                m_GiveAttackPoint = 4 * m_BaseFloor;
                m_GiveDefensePoint = 4 * m_BaseFloor;  
                m_GiveLifePoint = 4 * m_BaseFloor;    
                if(m_Type == ItemType::Helmet13){ m_Name = "Saphir Helmet"; }
                if(m_Type == ItemType::Chesplate13){ m_Name = "Saphir Chesplate"; }
                if(m_Type == ItemType::Legging13){ m_Name = "Saphir Legging"; }
                if(m_Type == ItemType::Boot13){ m_Name = "Saphir Boot"; } 
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+"\nHP : +" + std::to_string(m_GiveLifePoint) + "\nDefense : +" + std::to_string(m_GiveDefensePoint);
                break;
            case ItemType::Helmet14: case ItemType::Chesplate14: case ItemType::Legging14: case ItemType::Boot14:
                m_GiveAttackPoint = 5 * m_BaseFloor;
                m_GiveLifePoint = 5 * m_BaseFloor;  
                if(m_Type == ItemType::Helmet14){ m_Name = "Toxic Helmet"; }
                if(m_Type == ItemType::Chesplate14){ m_Name = "Toxic Chesplate"; }
                if(m_Type == ItemType::Legging14){ m_Name = "Toxic Legging"; }
                if(m_Type == ItemType::Boot14){ m_Name = "Toxic Boot"; } 
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+"\nHP : +" + std::to_string(m_GiveLifePoint);
                break;
            case ItemType::Helmet15: case ItemType::Chesplate15: case ItemType::Legging15: case ItemType::Boot15:
                m_GiveAttackPoint = 5 * m_BaseFloor;
                m_GiveManaPoint = 4 * m_BaseFloor;    
                m_GiveDefensePoint = 4 * m_BaseFloor;    
                m_GiveLifePoint = 5 * m_BaseFloor;   
                if(m_Type == ItemType::Helmet15){ m_Name = "God Helmet"; }
                if(m_Type == ItemType::Chesplate15){ m_Name = "God Chesplate"; }
                if(m_Type == ItemType::Legging15){ m_Name = "God Legging"; }
                if(m_Type == ItemType::Boot15){ m_Name = "God Boot"; }
                m_Description = "Attack : +" + std::to_string(m_GiveAttackPoint)+ "\nHP : +" + std::to_string(m_GiveLifePoint)+ "\nMana : +"+std::to_string(m_GiveManaPoint)+ "\nDefense : +"+std::to_string(m_GiveDefensePoint);
                break;

            case ItemType::HealthPot1:
                m_Useable = true;
                m_Name = "Health Potion";
                m_Description = "Basic potion of health, give : 25% of max lifepoint";
                break;
            case ItemType::HealthPot2:
                m_Useable = true;
                m_Name = "Health Potion";
                m_Description = "Medium potion of health, give : 50% of max lifepoint";
                break;
            case ItemType::HealthPot3:
                m_Useable = true;
                m_Name = "Health Potion";
                m_Description = "Strong potion of health, give : 70% of max lifepoint";
                break;
            case ItemType::ManaPot1:
                m_Useable = true;
                m_Name = "Mana Potion";
                m_Description = "Basic potion of mana, give : 25% of max manapoint";
                break;
            case ItemType::ManaPot2:
                m_Useable = true;
                m_Name = "Mana Potion";
                m_Description = "Medium potion of mana, give : 50% of max lifepoint";
                break;
            case ItemType::ManaPot3:
                m_Useable = true;
                m_Name = "Mana Potion";
                m_Description = "Strong potion of mana, give : 70% of max lifepoint";
                break;
            case ItemType::EnergyPot1:
                m_Useable = true;
                m_Name = "Energy Potion";
                m_Description = "Basic potion of Energy, give : 25% of max lifepoint and 25% of max manapoint";
                break;
            case ItemType::EnergyPot2:
                m_Useable = true;
                m_Name = "Energy Potion";
                m_Description = "Medium potion of Energy, give : 50% of max lifepoint and 50% of max manapoint";
                break;
            case ItemType::EnergyPot3:
                m_Useable = true;
                m_Name = "Energy Potion";
                m_Description = "Strong potion of Energy, give : 70% of max lifepoint and 70% of max manapoint";
                break;
            case ItemType::BoostAttack1:
                m_Useable = true;
                m_Name = "Boost attack";
                m_Description = "Basic boost of attack, add 10% to your attackpoint";
                break;
            case ItemType::BoostAttack2:
                m_Useable = true;
                m_Name = "Boost attack";
                m_Description = "Medium boost of attack, add 15% to your attackpoint";
                break;
            case ItemType::BoostAttack3:
                m_Useable = true;
                m_Name = "Boost attack";
                m_Description = "Strong boost of attack, add 20% to your attackpoint";
                break;
            case ItemType::BoostDefense1:
                m_Useable = true;
                m_Name = "Boost defense";
                m_Description = "Basic boost of defense, add 10% to your attackpoint";
                break;
            case ItemType::BoostDefense2:
                m_Useable = true;
                m_Name = "Boost defense";
                m_Description = "Medium boost of defense, add 15% to your attackpoint";
                break;
            case ItemType::BoostDefense3:
                m_Useable = true;
                m_Name = "Boost defense";
                m_Description = "Strong boost of defense, add 20% to your attackpoint";
                break;
            case ItemType::BoostHP1:
                m_Useable = true;
                m_Name = "Boost health";
                m_Description = "Basic boost of health, add 10% to your attackpoint";
                break;
            case ItemType::BoostHP2:
                m_Useable = true;
                m_Name = "Boost health";
                m_Description = "Medium boost of health, add 15% to your attackpoint";
                break;
            case ItemType::BoostHP3:
                m_Useable = true;
                m_Name = "Boost health";
                m_Description = "Strong boost of health, add 20% to your attackpoint";
                break;
            case ItemType::BoostXP1:
                m_Useable = true;
                m_Name = "Boost XP";
                m_Description = "Basic boost of XP, add 10% to your attackpoint";
                break;
            case ItemType::BoostXP2:
                m_Useable = true;
                m_Name = "Boost XP";
                m_Description = "Medium boost of XP, add 15% to your attackpoint";
                break;
            case ItemType::BoostXP3:
                m_Useable = true;
                m_Name = "Boost XP";
                m_Description = "Strong boost of XP, add 20% to your attackpoint";
                break;

            default:
                m_GiveLifePoint = 0;
                m_GiveAttackPoint = 0;
                m_GiveDefensePoint = 0;
                m_GiveManaPoint = 0;
                m_Useable = false;
                m_Name = "";
                m_Description = "";
                break;
        }

        m_GiveLifePoint += Variance(-(m_GiveLifePoint/10));
        m_GiveAttackPoint += Variance(-(m_GiveAttackPoint/10));
        m_GiveDefensePoint += Variance(-(m_GiveDefensePoint/10));
        m_GiveManaPoint += Variance(-(m_GiveManaPoint/10));
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