#include "Item.h"

namespace redsquare
{
    Item::Item(ItemType type)
    : m_SlotMask(0)
    , m_Type(type)
    {
        defaultCompatibleSlot();
        defaultData();
    }

    Item::Item(ItemType type, uint8_t slotMask)
    : m_SlotMask(slotMask)
    , m_Type(type)
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
            case ItemType::Sword:
            {
                addCompatibleSlot(InventorySlotType::Weapon);
                break;
            }
        }

        addCompatibleSlot(InventorySlotType::Cargo);
    }

    void Item::defaultData()
    {
        switch (m_Type)
        {
            case ItemType::Sword:
            {
                
                break;
            }

            default:
            {
                m_GiveLifePoint = 0;
                m_GiveAttackPoint = 0;
                m_GiveDefensePoint = 0;
                m_Useable = false;
                break;
            }
        }
    }

    bool Item::isUseable() const
    {
        return m_Useable;
    }
}