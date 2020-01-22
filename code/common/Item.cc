#include "Item.h"

namespace redsquare
{
    Item::Item(ItemType type)
    : m_SlotMask(0)
    , m_Type(type)
    {
        defaultCompatibleSlot();
        defaultCharacteristic();
    }

    Item::Item(ItemType type, uint8_t slotMask)
    : m_SlotMask(slotMask)
    , m_Type(type)
    {
        defaultCharacteristic();
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

    void Item::defaultCharacteristic()
    {
        switch (m_Type)
        {
            case ItemType::Sword:
            {
                
                break;
            }

            default:
            {
                m_PassiveLifePoint = 0;
                m_PassiveAttackPoint = 0;
                m_PassiveDefensePoint = 0;

                m_GiveLifePoint = 0;
                m_GiveAttackPoint = 0;
                m_GiveDefensePoint = 0;
                break;
            }
        }
    }
}