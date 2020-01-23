#ifndef REDSQUARE_SERVER_INVENTORY_H
#define REDSQUARE_SERVER_INVENTORY_H

#include "ServerItem.h"

#include <sys/types.h>

namespace redsquare
{
    class Inventory
    {
    public:
        Inventory();

        //Add the item at the first slot possible if it's == cargo, return >= 0 if succes or -1 if not
        ssize_t addItem(InventorySlotType slotType, ServerItem &&item);
        bool addItem(InventorySlotType slotType, ServerItem &&item, uint pos);

        //pos is used only if slotType == Cargo
        bool removeItem(InventorySlotType slotType, uint pos = 0u);

        //pos is used only if slotType == Cargo, nullptr if Item doesn't exist
        ServerItem* getItem(InventorySlotType slotType, uint pos = 0u);

        bool moveItem(MoveItem moveItem);

    private:
        const uint RowCargoSlotNmb = 6;
        const uint ColumnCargoSlotNmb = 10;
        std::map<uint, ServerItem> m_CargoItems;

        std::map<InventorySlotType, ServerItem> m_SpecialItems;
    };
}

#endif