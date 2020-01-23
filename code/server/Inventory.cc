#include "Inventory.h"

namespace redsquare
{
    Inventory::Inventory()
    {

    }

    ssize_t Inventory::addItem(InventorySlotType slotType, ServerItem &&item)
    {
        if (slotType == InventorySlotType::Cargo && item.canBeInSlot(slotType))
        {
            for(uint i = 0; i < RowCargoSlotNmb; ++i )
            {
                for(uint j = 0; j < ColumnCargoSlotNmb; ++j )
                {
                    if (m_CargoItems.find(i * ColumnCargoSlotNmb + j) == m_CargoItems.end())
                    {
                        m_CargoItems.insert(std::make_pair(i * ColumnCargoSlotNmb + j, std::move(item)));
                        return (i * ColumnCargoSlotNmb + j);
                    }
                }
            }
        }
        else
        {
            if (m_SpecialItems.find(slotType) == m_SpecialItems.end() && item.canBeInSlot(slotType))
            {
                m_SpecialItems.insert(std::make_pair(slotType, std::move(item)));
                return 0;
            }
        }
        
        return -1;
    }

    bool Inventory::addItem(InventorySlotType slotType, ServerItem &&item, uint pos)
    {
        if (slotType == InventorySlotType::Cargo && item.canBeInSlot(slotType))
        {
            auto it = m_CargoItems.find(pos);
            if (it == m_CargoItems.end())
            {
                m_CargoItems.insert(std::make_pair(pos, std::move(item)));
                return true;
            }
        }
        else
        {
            auto it = m_SpecialItems.find(slotType);
            if (it == m_SpecialItems.end() && item.canBeInSlot(slotType))
            {
                m_SpecialItems.insert(std::make_pair(slotType, std::move(item)));
                return true;
            }
        }

        return false;
    }

    bool Inventory::removeItem(InventorySlotType slotType, uint pos)
    {
        if (slotType == InventorySlotType::Cargo)
        {
            auto it = m_CargoItems.find(pos);
            if (it != m_CargoItems.end())
            {
                m_CargoItems.erase(it);
                return true;
            }
        }
        else
        {
            auto it = m_SpecialItems.find(slotType);
            if (it != m_SpecialItems.end())
            {
                m_SpecialItems.erase(it);
                return true;
            }
        }

        return false;
    }

    ServerItem* Inventory::getItem(InventorySlotType slotType, uint pos)
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

    bool Inventory::moveItem(MoveItem moveItem)
    {
        if (moveItem.oldSlotType == InventorySlotType::Cargo)
        {
            auto oldIt = m_CargoItems.find(moveItem.oldPos);
            if (oldIt != m_CargoItems.end())
            {
                ServerItem item = std::move(oldIt->second);
                m_CargoItems.erase(oldIt);

                if ( addItem(moveItem.newSlotType, std::move(item), moveItem.newPos) )
                {
                    return true;
                }
            }
        }
        else
        {
            auto oldIt = m_SpecialItems.find(moveItem.oldSlotType);
            if (oldIt != m_SpecialItems.end())
            {
                ServerItem item = std::move(oldIt->second);
                m_SpecialItems.erase(oldIt);

                if ( addItem(moveItem.newSlotType, std::move(item), moveItem.newPos) )
                {
                    return true;
                }
            }
        }

        return false;
    }
}