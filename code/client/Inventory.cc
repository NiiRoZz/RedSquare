#include "Inventory.h"
#include "../common/Singletons.h"
#include "Game.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Vector.h>
#include <gf/WidgetContainer.h>
#include <gf/VectorOps.h>


namespace redsquare
{
    Inventory::Inventory(gf::Font &font, Game &game)
    : m_ShowInventory(false)
    , m_Game(game)
    , m_UI(font)
    , m_PlayerWidget(gResourceManager().getTexture("img/goblin.png"))
    , m_OldSlot(nullptr)
    , m_CurrMovingWidget(nullptr)
    , m_CurrMovingItem(nullptr)
    {
        gMessageManager().registerHandler<InventoryUpdateMessage>(&Inventory::onInventoryUpdate, this);
        gMessageManager().registerHandler<ItemUpdateMessage>(&Inventory::onItemUpdate, this);
        gMessageManager().registerHandler<ItemMoveMessage>(&Inventory::onItemMove, this);
        gMessageManager().registerHandler<MyPlayerReceivedTypeMessage>(&Inventory::onMyPlayerReceived, this);

        for(uint i = 0; i < RowCargoSlotNmb; ++i )
        {
            for(uint j = 0; j < ColumnCargoSlotNmb; ++j )
            {
                m_CargoSlots.insert(std::make_pair(i * ColumnCargoSlotNmb + j, std::move(InventorySlot(InventorySlotType::Cargo, i * ColumnCargoSlotNmb + j))));
            }
        }

        m_SpecialSlots.insert(std::make_pair(InventorySlotType::Helmet, std::move(InventorySlot(InventorySlotType::Helmet))));
        m_SpecialSlots.insert(std::make_pair(InventorySlotType::ChestPlate, std::move(InventorySlot(InventorySlotType::ChestPlate))));
        m_SpecialSlots.insert(std::make_pair(InventorySlotType::Legging, std::move(InventorySlot(InventorySlotType::Legging))));
        m_SpecialSlots.insert(std::make_pair(InventorySlotType::Boot, std::move(InventorySlot(InventorySlotType::Boot))));
        m_SpecialSlots.insert(std::make_pair(InventorySlotType::Weapon, std::move(InventorySlot(InventorySlotType::Weapon))));
        m_SpecialSlots.insert(std::make_pair(InventorySlotType::Shield, std::move(InventorySlot(InventorySlotType::Shield))));
    }

    void Inventory::update(gf::Time time)
    {
        for( auto &x: m_CargoSlots )
        {
            x.second.update(time);
        }

        for( auto &x: m_SpecialSlots )
        {
            x.second.update(time);
        }
    }

    void Inventory::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        if(m_ShowInventory)
        {
            m_UI.setCharacterSize(18);
            gf::Coordinates coordinates(target);

            gf::Vector2f InventoryWindowPoint = coordinates.getRelativePoint({ 0.35f, 0.1f });
            gf::Vector2f InventoryWindowSize = coordinates.getRelativeSize({ 0.50f, 0.8889f });
        
            if( m_UI.begin("Inventory", gf::RectF::fromPositionSize( InventoryWindowPoint, InventoryWindowSize ), gf::UIWindow::Title | gf::UIWindow::NoScrollbar))
            {
                //Image of player
                gf::Vector2f startPlayerWidgetPos({InventoryWindowPoint[0], InventoryWindowPoint[1] + InventoryWindowSize[1] * StartPercentagePosPlayerWidget});
                gf::Vector2f endPlayerWidgetPos({InventoryWindowPoint[0], InventoryWindowPoint[1] + InventoryWindowSize[1] * EndPercentagePosPlayerWidget});
                m_PlayerWidget.setPosition(startPlayerWidgetPos);
                m_PlayerWidget.setScale({(InventoryWindowSize[0] * WidthPlayerWidget) / PlayerTextureSize, (endPlayerWidgetPos[1] - startPlayerWidgetPos[1]) / PlayerTextureSize});

                //Cargo slot rendering
                gf::Vector2f firstSlotPos({InventoryWindowPoint[0], InventoryWindowPoint[1] + InventoryWindowSize[1] * StartPercentagePos});
                float sizeWidthItem = (InventoryWindowSize[0] - (ColumnCargoSlotNmb - 1) * SpaceBetweenSlots) / ColumnCargoSlotNmb;
                float sizeHeightItem = (InventoryWindowSize[1] * (1 - StartPercentagePos) - (RowCargoSlotNmb - 1) * SpaceBetweenSlots) / RowCargoSlotNmb;

                //Update pos of CargoSlot
                for(uint i = 0; i < RowCargoSlotNmb; ++i )
                {
                    for(uint j = 0; j < ColumnCargoSlotNmb; ++j )
                    {
                        gf::Vector2f pos = firstSlotPos + gf::Vector2f({sizeWidthItem * j + SpaceBetweenSlots * j, sizeHeightItem * i + SpaceBetweenSlots * i});
                        gf::Vector2f scale(sizeWidthItem / IconSize, sizeHeightItem / IconSize);

                        m_CargoSlots.at(i * ColumnCargoSlotNmb + j).update(pos, scale);
                    }
                }

                //Update pos of SpecialSlots
                gf::Vector2f startGearPos({InventoryWindowPoint[0] + (InventoryWindowSize[0] * WidthPositionGearStuff), InventoryWindowPoint[1] + InventoryWindowSize[1] * StartPercentageGearStuff});
                float sizeItem = ((endPlayerWidgetPos[1] - startGearPos[1]) - 3 * SpaceBetweenSlots) / 4;
                gf::Vector2f scale(sizeItem / IconSize, sizeItem / IconSize);

                for( auto &x : m_SpecialSlots )
                {
                    InventorySlotType currSlotType = x.first;

                    switch (currSlotType)
                    {
                        case InventorySlotType::Helmet:
                        case InventorySlotType::ChestPlate:
                        case InventorySlotType::Legging:
                        case InventorySlotType::Boot:
                        {
                            int pos = -1;

                            if (currSlotType == InventorySlotType::Helmet)
                            {
                                pos = 0;
                            }
                            else if (currSlotType == InventorySlotType::ChestPlate)
                            {
                                pos = 1;
                            }
                            else if (currSlotType == InventorySlotType::Legging)
                            {
                                pos = 2;
                            }
                            else if (currSlotType == InventorySlotType::Boot)
                            {
                                pos = 3;
                            }

                            if (pos == -1)
                            {
                                continue;
                            }

                            gf::Vector2f posVector = startGearPos + gf::Vector2f({0, sizeItem * pos + SpaceBetweenSlots * pos});

                            x.second.update(posVector, scale);
                            break;
                        }

                        case InventorySlotType::Weapon:
                        case InventorySlotType::Shield:
                        {
                            int pos = -1;

                            if (currSlotType == InventorySlotType::Weapon)
                            {
                                pos = 1;
                            }
                            else if (currSlotType == InventorySlotType::Shield)
                            {
                                pos = 2;
                            }

                            if (pos == -1)
                            {
                                continue;
                            }

                            gf::Vector2f posVector = startGearPos + gf::Vector2f({InventoryWindowSize[0] * 0.2f, sizeItem * pos + SpaceBetweenSlots * pos});
                            x.second.update(posVector, scale);
                            break;
                        }
                    }
                }
                 
                m_UI.end();

                target.draw( m_UI , states );
                target.draw( m_PlayerWidget, states );

                for( auto &x: m_CargoSlots )
                {
                    x.second.render( target, states );
                }

                for( auto &x: m_SpecialSlots )
                {
                    x.second.render( target, states );
                }

                //Draw the current moving widget, so he can be over any slot case
                if (m_CurrMovingWidget != nullptr)
                {
                    target.draw( *m_CurrMovingWidget, states );
                }
            }
        }
    }

    void Inventory::processEvent(const gf::Event &event)
    {
        m_UI.processEvent(event);

        if (m_ShowInventory)
        {
            switch (event.type)
            {
                case gf::EventType::MouseButtonPressed:
                {
                    switch (event.mouseButton.button)
                    {
                        case gf::MouseButton::Left:
                        {
                            bool found = false;

                            for( auto &x: m_SpecialSlots)
                            {
                                InventoryWidget *itemWidget = x.second.getItemWidget(event.mouseButton.coords);
                                if ( itemWidget != nullptr )
                                {
                                    itemWidget->currDragging = true;
                                    m_OldSlot = &(x.second);
                                    m_CurrMovingWidget = itemWidget;
                                    m_CurrMovingItem = x.second.getItem();
                                    m_OffsetDrag = event.mouseButton.coords - itemWidget->getPosition();
                                    found = true;
                                    break;
                                }
                            }

                            if (!found)
                            {
                                for( auto &x: m_CargoSlots)
                                {
                                    InventoryWidget *itemWidget = x.second.getItemWidget(event.mouseButton.coords);
                                    if ( itemWidget != nullptr )
                                    {
                                        itemWidget->currDragging = true;
                                        m_OldSlot = &(x.second);
                                        m_CurrMovingWidget = itemWidget;
                                        m_CurrMovingItem = x.second.getItem();
                                        m_OffsetDrag = event.mouseButton.coords - itemWidget->getPosition();
                                        break;
                                    }
                                }
                            }
                            break;
                        }

                        case gf::MouseButton::Right:
                        {
                            break;
                        }
                    }

                    break;
                }

                case gf::EventType::MouseButtonReleased:
                {
                    if (m_CurrMovingWidget != nullptr && m_CurrMovingItem != nullptr)
                    {
                        bool sendedPacket = false;

                        for( auto &x: m_SpecialSlots)
                        {
                            if (x.second.contains(event.mouseButton.coords) && !(x.second.haveItem()) && m_CurrMovingItem->canBeInSlot(x.second.getSlotType()))
                            {
                                //Request move item here
                                Packet packet;
                                packet.type = PacketType::MoveItem;
                                packet.moveItem.playerID = m_Game.m_PlayerID;
                                packet.moveItem.oldSlotType = m_OldSlot->getSlotType();
                                packet.moveItem.oldPos = m_OldSlot->getSlotPos();
                                packet.moveItem.newSlotType = x.second.getSlotType();
                                packet.moveItem.newPos = x.second.getSlotPos();

                                m_Game.sendPacket(packet);

                                sendedPacket = true;
                                break;
                            }
                        }

                        if (!sendedPacket)
                        {
                            for( auto &x: m_CargoSlots)
                            {
                                if (x.second.contains(event.mouseButton.coords) && !(x.second.haveItem()) && m_CurrMovingItem->canBeInSlot(x.second.getSlotType()))
                                {
                                    //Request move item here
                                    Packet packet;
                                    packet.type = PacketType::MoveItem;
                                    packet.moveItem.playerID = m_Game.m_PlayerID;
                                    packet.moveItem.oldSlotType = m_OldSlot->getSlotType();
                                    packet.moveItem.oldPos = m_OldSlot->getSlotPos();
                                    packet.moveItem.newSlotType = x.second.getSlotType();
                                    packet.moveItem.newPos = x.second.getSlotPos();

                                    m_Game.sendPacket(packet);

                                    sendedPacket = true;
                                    break;
                                }
                            }
                        }

                        if (sendedPacket)
                        {
                            m_OldSlot->setMoveItemRequest();
                        }
                        
                        m_CurrMovingWidget->currDragging = false;
                        m_OldSlot = nullptr;
                        m_CurrMovingWidget = nullptr;
                        m_CurrMovingItem = nullptr;
                        m_OffsetDrag = {0,0};
                    }
                }

                case gf::EventType::MouseMoved:
                {
                    if (m_CurrMovingWidget != nullptr)
                    {
                        m_CurrMovingWidget->setPosition(event.mouseCursor.coords - m_OffsetDrag);
                    }
                    break;
                }
            }
        }
    }

    void Inventory::addItem(InventorySlotType slotType, ClientItem &&item, uint pos)
    {
        if (slotType == InventorySlotType::Cargo)
        {
            auto it = m_CargoItems.find(pos);
            auto it2 = m_CargoSlots.find(pos);
            if ( it2 != m_CargoSlots.end() )
            {
                //If item exist already, we replace it by the new one
                if ( it != m_CargoItems.end() )
                {
                    it2->second.setItem(nullptr);
                    m_CargoItems.erase(it);
                }

                auto it3 = m_CargoItems.emplace(pos, std::move(item));
                assert(it3.second);
                
                it2->second.setItem(&(it3.first->second));
            }
        }
        else
        {
            auto it = m_SpecialItems.find(slotType);
            auto it2 = m_SpecialSlots.find(slotType);
            if ( it2 != m_SpecialSlots.end() )
            {
                //If item exist already, we replace it by the new one
                if (it != m_SpecialItems.end())
                {
                    it2->second.setItem(nullptr);
                    m_SpecialItems.erase(it);
                }

                auto it3 = m_SpecialItems.emplace(slotType, std::move(item));
                assert(it3.second);

                it2->second.setItem(&(it3.first->second));
            }
        }
    }

    gf::MessageStatus Inventory::onInventoryUpdate(gf::Id id, gf::Message *msg)
    {
        assert(id == InventoryUpdateMessage::type);
        
        m_ShowInventory = !m_ShowInventory;
        
        return gf::MessageStatus::Keep;
    }

    gf::MessageStatus Inventory::onItemUpdate(gf::Id id, gf::Message *msg)
    {
        assert(id == ItemUpdateMessage::type);
        
        ItemUpdateMessage *message = static_cast<ItemUpdateMessage*>(msg);

        if (message)
        {
            if (message->itemMessage.slotType == InventorySlotType::Cargo)
            {
                if (message->itemMessage.removeItem)
                {
                    auto it = m_CargoItems.find(message->itemMessage.pos);
                    auto it2 = m_CargoSlots.find(message->itemMessage.pos);
                    if (it != m_CargoItems.end() && it2 != m_CargoSlots.end())
                    {
                        it2->second.setItem(nullptr);
                        m_CargoItems.erase(it);
                    }
                }
                else
                {
                    ClientItem newItem(message->itemMessage.typeItem, message->itemMessage.slotMask);
                    addItem(message->itemMessage.slotType, std::move(newItem), message->itemMessage.pos);
                }
            }
            else
            {
                if (message->itemMessage.removeItem)
                {
                    auto it = m_SpecialItems.find(message->itemMessage.slotType);
                    auto it2 = m_SpecialSlots.find(message->itemMessage.slotType);
                    if (it != m_SpecialItems.end() && it2 != m_SpecialSlots.end())
                    {
                        it2->second.setItem(nullptr);
                        m_SpecialItems.erase(it);
                    }
                }
                else
                {
                    ClientItem newItem(message->itemMessage.typeItem, message->itemMessage.slotMask);
                    addItem(message->itemMessage.slotType, std::move(newItem));
                }
            }
        }
        
        return gf::MessageStatus::Keep;
    }

    gf::MessageStatus Inventory::onItemMove(gf::Id id, gf::Message *msg)
    {
        assert(id == ItemMoveMessage::type);
        
        ItemMoveMessage *message = static_cast<ItemMoveMessage*>(msg);

        if (message)
        {
            if (message->itemMessage.oldSlotType == InventorySlotType::Cargo)
            {
                auto oldIt = m_CargoItems.find(message->itemMessage.oldPos);
                auto oldIt2 = m_CargoSlots.find(message->itemMessage.oldPos);
                if (oldIt != m_CargoItems.end() && oldIt2 != m_CargoSlots.end())
                {
                    oldIt2->second.setItem(nullptr);
                    ClientItem item = std::move(oldIt->second);
                    m_CargoItems.erase(oldIt);

                    addItem(message->itemMessage.newSlotType, std::move(item), message->itemMessage.newPos);
                }
            }
            else
            {
                auto oldIt = m_SpecialItems.find(message->itemMessage.oldSlotType);
                auto oldIt2 = m_SpecialSlots.find(message->itemMessage.oldSlotType);
                if (oldIt != m_SpecialItems.end() && oldIt2 != m_SpecialSlots.end())
                {
                    oldIt2->second.setItem(nullptr);
                    ClientItem item = std::move(oldIt->second);
                    m_SpecialItems.erase(oldIt);

                    addItem(message->itemMessage.newSlotType, std::move(item), message->itemMessage.newPos);
                }
            }
        }
        
        return gf::MessageStatus::Keep;
    }

    gf::MessageStatus Inventory::onMyPlayerReceived(gf::Id id, gf::Message *msg)
    {
        assert(id == MyPlayerReceivedTypeMessage::type);
        
        MyPlayerReceivedTypeMessage *message = static_cast<MyPlayerReceivedTypeMessage*>(msg);

        if (message)
        {
            gf::Texture* texturePlayer = Player::getTexture(message->entityType);
            m_PlayerWidget.setDefaultSprite(*texturePlayer, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
            m_PlayerWidget.setDisabledSprite(*texturePlayer, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
            m_PlayerWidget.setSelectedSprite(*texturePlayer, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        }
    }
}