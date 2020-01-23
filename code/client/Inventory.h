

#ifndef REDSQUARE_CLIENT_INVENTORY_H
#define REDSQUARE_CLIENT_INVENTORY_H

#include "ClientItem.h"

namespace redsquare
{
    //Forward class
    class ClientEntity;

    class Inventory
    {
    public:
        Inventory(gf::Id ownerID);

        bool addItem(InventorySlotType slotType, ClientItem &&item, uint pos = 0);
        bool removeItem(InventorySlotType slotType, uint pos = 0);
        bool moveItem(MoveItem moveItem);
        
        //pos is used only if slotType == Cargo, nullptr if Item doesn't exist
        ClientItem* getItem(InventorySlotType slotType, uint pos = 0u);

    private:
        gf::Id m_OwnerID;

        std::map<uint, ClientItem> m_CargoItems;
        std::map<InventorySlotType, ClientItem> m_SpecialItems;
    };
}

#endif