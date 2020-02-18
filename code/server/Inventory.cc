#include "Inventory.h"
#include "Player.h"

namespace redsquare
{
    Inventory::Inventory()
    : m_Owner(nullptr)
    {
    }

    void Inventory::setOwner(Player *owner)
    {
        m_Owner = owner;
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
                if (m_Owner != nullptr)
                {
                    m_Owner->onMovedItem(item, false);
                }
                m_SpecialItems.insert(std::make_pair(slotType, std::move(item)));
                return 0;
            }
        }
        return -1;
    }

    ssize_t Inventory::addItemRandom(InventorySlotType slotType, ServerItem &&item)
    {
        if (slotType == InventorySlotType::Cargo && item.canBeInSlot(slotType))
        {
            std::cout << static_cast<uint16_t>(item.getType()) <<std::endl;
            int nbTry = 20;
            uint row;
            uint column;
            do{ 
                nbTry--;
                row = rand() % RowCargoSlotNmb;
                column = rand() % ColumnCargoSlotNmb;
                if(nbTry == 0){
                    return -1;
                }
            }while(getItem(slotType,row * ColumnCargoSlotNmb + column) != nullptr);

            m_CargoItems.insert(std::make_pair(row * ColumnCargoSlotNmb + column, std::move(item)));
            return (row * ColumnCargoSlotNmb + column);
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
                if (m_Owner != nullptr)
                {
                    m_Owner->onMovedItem(item, false);
                }
                m_SpecialItems.insert(std::make_pair(slotType, std::move(item)));
                return true;
            }
        }

        return false;
    }

    ServerItem Inventory::removeItem(InventorySlotType slotType, uint pos)
    {
        if (slotType == InventorySlotType::Cargo)
        {
            auto it = m_CargoItems.find(pos);
            if (it != m_CargoItems.end())
            {
                ServerItem item = std::move(it->second);
                m_CargoItems.erase(it);
                return item;
            }
        }
        else
        {
            auto it = m_SpecialItems.find(slotType);
            if (it != m_SpecialItems.end())
            {
                ServerItem item = std::move(it->second);
                if (m_Owner != nullptr)
                {
                    m_Owner->onMovedItem(item, true);
                }
                m_SpecialItems.erase(it);
                return item;
            }
        }

        return ServerItem();
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
}