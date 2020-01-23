#ifndef REDSQUARE_COMMON_ITEM_H
#define REDSQUARE_COMMON_ITEM_H

#include "Packet.h"

namespace redsquare
{
    class Item
    {
    public:
        //Per default, item will only be able to be placed to cargo slot
        Item(ItemType type);
        Item(ItemType type, uint8_t slotMask);

        ItemType getType() const;

        void addCompatibleSlot(InventorySlotType slotType);
        void removeCompatibleSlot(InventorySlotType slotType);
        bool canBeInSlot(InventorySlotType slotType);

        int m_GiveLifePoint;
        int m_GiveAttackPoint;
        int m_GiveDefensePoint;

        bool isUseable() const;

        uint8_t m_SlotMask;

    protected:
        

    private:

        void defaultCompatibleSlot();
        void defaultData();

        ItemType m_Type;

        bool m_Useable;

        std::string m_Name;
        std::string m_Description;
    };
}

#endif