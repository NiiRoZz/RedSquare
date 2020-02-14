#include "Inventory.h"
#include "ClientEntity.h"
#include "Message.h"
#include "../common/Singletons.h"

namespace redsquare
{
    Inventory::Inventory(gf::Id ownerID)
    : m_OwnerID(ownerID)
    {

    }

    bool Inventory::addItem(InventorySlotType slotType, ClientItem &&item, uint pos)
    {
        ItemUpdateUIMessage message;
        message.entityID = m_OwnerID;
        message.itemRemoved = false;
        message.slotType = slotType;
        message.slotPos = pos;

        if (slotType == InventorySlotType::Cargo)
        {
            auto it = m_CargoItems.find(pos);

            //If item exist already, we replace it by the new one
            if ( it != m_CargoItems.end() )
            {
                m_CargoItems.erase(it);
            }
            
            auto it2 = m_CargoItems.emplace(pos, std::move(item));
            assert(it2.second);
            message.item = &(it2.first->second);

            gMessageManager().sendMessage(&message);

            return true;
        }
        else
        {
            auto it = m_SpecialItems.find(slotType);

            //If item exist already, we replace it by the new one
            if (it != m_SpecialItems.end())
            {
                m_SpecialItems.erase(it);
            }

            auto it2 = m_SpecialItems.emplace(slotType, std::move(item));
            assert(it2.second);

            message.item = &(it2.first->second);

            gMessageManager().sendMessage(&message);

            return true;
        }

        return false;
    }

    ClientItem* Inventory::getItem(InventorySlotType slotType, uint pos)
    {
        if (slotType == InventorySlotType::Cargo)
        {
            auto it = m_CargoItems.find(pos);
            if (it != m_CargoItems.end())
            {
                return &(it->second);
            }
        }
        else
        {
            auto it = m_SpecialItems.find(slotType);
            if (it != m_SpecialItems.end())
            {
                return &(it->second);
            }
        }

        return nullptr;
    }

    ClientItem Inventory::removeItem(InventorySlotType slotType, uint pos)
    {
        ItemUpdateUIMessage message;
        message.entityID = m_OwnerID;
        message.itemRemoved = true;
        message.slotType = slotType;
        message.slotPos = pos;

        if (slotType == InventorySlotType::Cargo)
        {
            auto it = m_CargoItems.find(pos);
            if (it != m_CargoItems.end())
            {
                ClientItem item = std::move(it->second);
                m_CargoItems.erase(it);

                gMessageManager().sendMessage(&message);

                return item;
            }
        }
        else
        {
            auto it = m_SpecialItems.find(slotType);
            if (it != m_SpecialItems.end())
            {
                ClientItem item = std::move(it->second);
                m_SpecialItems.erase(it);

                gMessageManager().sendMessage(&message);

                return item;
            }
        }

        return ClientItem();
    }
}