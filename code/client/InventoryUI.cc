#include "InventoryUI.h"
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
    InventoryUI::InventoryUI(gf::Font &font, Game &game)
    : m_ShowInventory(false)
    , m_Game(game)
    , m_UI(font)
    , m_PlayerEntity(nullptr)
    , m_VinicityEntity(nullptr)
    , m_PlayerWidget(gResourceManager().getTexture("img/goblin.png"))
    , m_OldSlot(nullptr)
    , m_CurrMovingWidget(nullptr)
    , m_CurrMovingItem(nullptr)
    , m_OldEntity(nullptr)
    {
        gMessageManager().registerHandler<InventoryShowUpdateMessage>(&InventoryUI::onInventoryShowUpdate, this);
        gMessageManager().registerHandler<ItemUpdateUIMessage>(&InventoryUI::onItemUpdateUI, this);
        gMessageManager().registerHandler<MyPlayerReceivedTypeMessage>(&InventoryUI::onMyPlayerReceived, this);

        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::Helmet, std::move(InventorySlot(InventorySlotType::Helmet))));
        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::ChestPlate, std::move(InventorySlot(InventorySlotType::ChestPlate))));
        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::Legging, std::move(InventorySlot(InventorySlotType::Legging))));
        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::Boot, std::move(InventorySlot(InventorySlotType::Boot))));
        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::Weapon, std::move(InventorySlot(InventorySlotType::Weapon))));
        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::Shield, std::move(InventorySlot(InventorySlotType::Shield))));
    }

    void InventoryUI::update(gf::Time time)
    {
        for( auto &x: m_PlayerCargoSlots )
        {
            x.second.update(time);
        }

        for( auto &x: m_PlayerSpecialSlots )
        {
            x.second.update(time);
        }

        for( auto &x: m_VinicityCargoSlots )
        {
            x.second.update(time);
        }
    }

    void InventoryUI::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        if( m_ShowInventory )
        {
            m_UI.setCharacterSize(18);
            gf::Coordinates coordinates(target);

            gf::Vector2f PlayerInventoryWindowPoint = coordinates.getRelativePoint({ 0.56f, 0.1f });
            gf::Vector2f InventoryWindowSize = coordinates.getRelativeSize({ 0.42f, 0.80f });
        
            if( m_PlayerEntity != nullptr && m_UI.begin("InventoryPlayer", gf::RectF::fromPositionSize( PlayerInventoryWindowPoint, InventoryWindowSize ), gf::UIWindow::NoScrollbar))
            {
                //Image of player
                gf::Vector2f startPlayerWidgetPos({PlayerInventoryWindowPoint[0], PlayerInventoryWindowPoint[1] + InventoryWindowSize[1] * StartPercentagePosPlayerWidget});
                gf::Vector2f endPlayerWidgetPos({PlayerInventoryWindowPoint[0], PlayerInventoryWindowPoint[1] + InventoryWindowSize[1] * EndPercentagePosPlayerWidget});
                m_PlayerWidget.setPosition(startPlayerWidgetPos);
                m_PlayerWidget.setScale({(InventoryWindowSize[0] * WidthPlayerWidget) / PlayerTextureSize, (endPlayerWidgetPos[1] - startPlayerWidgetPos[1]) / PlayerTextureSize});

                //Cargo slot rendering
                gf::Vector2f firstSlotPos({PlayerInventoryWindowPoint[0], PlayerInventoryWindowPoint[1] + InventoryWindowSize[1] * m_StartPercentagePosPlayerCargo});
                float sizeWidthItem = (InventoryWindowSize[0] - (m_PlayerEntity->m_ColumnCargoSlotNmb - 1) * m_SpaceBetweenPlayerSlotsCargo) / m_PlayerEntity->m_ColumnCargoSlotNmb;
                float sizeHeightItem = (InventoryWindowSize[1] * (1 - m_StartPercentagePosPlayerCargo) - (m_PlayerEntity->m_RowCargoSlotNmb - 1) * m_SpaceBetweenPlayerSlotsCargo) / m_PlayerEntity->m_RowCargoSlotNmb;

                //Update pos of CargoSlot
                for(uint i = 0; i < m_PlayerEntity->m_RowCargoSlotNmb; ++i )
                {
                    for(uint j = 0; j < m_PlayerEntity->m_ColumnCargoSlotNmb; ++j )
                    {
                        gf::Vector2f pos = firstSlotPos + gf::Vector2f({sizeWidthItem * j + m_SpaceBetweenPlayerSlotsCargo * j, sizeHeightItem * i + m_SpaceBetweenPlayerSlotsCargo * i});
                        gf::Vector2f scale(sizeWidthItem / IconSize, sizeHeightItem / IconSize);

                        m_PlayerCargoSlots.at(i * m_PlayerEntity->m_ColumnCargoSlotNmb + j).update(pos, scale);
                    }
                }

                //Update pos of SpecialSlots
                gf::Vector2f startGearPos({PlayerInventoryWindowPoint[0] + (InventoryWindowSize[0] * WidthPositionGearStuff), PlayerInventoryWindowPoint[1] + InventoryWindowSize[1] * StartPercentageGearStuff});
                float sizeItem = ((endPlayerWidgetPos[1] - startGearPos[1]) - 3 * m_SpaceBetweenPlayerSlotsCargo) / 4;
                gf::Vector2f scale(sizeItem / IconSize, sizeItem / IconSize);

                for( auto &x : m_PlayerSpecialSlots )
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

                            gf::Vector2f posVector = startGearPos + gf::Vector2f({0, sizeItem * pos + m_SpaceBetweenPlayerSlotsCargo * pos});

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

                            gf::Vector2f posVector = startGearPos + gf::Vector2f({InventoryWindowSize[0] * 0.2f, sizeItem * pos + m_SpaceBetweenPlayerSlotsCargo * pos});
                            x.second.update(posVector, scale);
                            break;
                        }
                    }

                }
                m_UI.end();
            
                if( m_VinicityEntity != nullptr )
                {
                    gf::Vector2f VinicityInventoryWindowPoint = coordinates.getRelativePoint({ 0.13f, 0.1f });

                    if (m_UI.begin("InventoryVinicity", gf::RectF::fromPositionSize( VinicityInventoryWindowPoint, InventoryWindowSize ), gf::UIWindow::NoScrollbar))
                    {
                        //Image of vinicity
                        gf::Vector2f startVinictyWidgetPos({VinicityInventoryWindowPoint[0], VinicityInventoryWindowPoint[1] + InventoryWindowSize[1] * StartPercentagePosVinicityWidget});
                        gf::Vector2f endVinicityWidgetPos({VinicityInventoryWindowPoint[0], VinicityInventoryWindowPoint[1] + InventoryWindowSize[1] * EndPercentagePosVinicityWidget});
                        m_VinictyWidget.setPosition(startVinictyWidgetPos);
                        m_VinictyWidget.setScale({(InventoryWindowSize[0] * WidthVinictyWidget) / PlayerTextureSize, (endVinicityWidgetPos[1] - startVinictyWidgetPos[1]) / PlayerTextureSize});

                        //Cargo slot rendering
                        gf::Vector2f firstSlotPos({VinicityInventoryWindowPoint[0], VinicityInventoryWindowPoint[1] + InventoryWindowSize[1] * m_StartPercentagePosVinicityCargo});
                        float sizeWidthItem = (InventoryWindowSize[0] - (m_VinicityEntity->m_ColumnCargoSlotNmb - 1) * m_SpaceBetweenPlayerSlotsCargo) / m_VinicityEntity->m_ColumnCargoSlotNmb;
                        float sizeHeightItem = (InventoryWindowSize[1] * (1 - m_StartPercentagePosVinicityCargo) - (m_VinicityEntity->m_RowCargoSlotNmb - 1) * m_SpaceBetweenPlayerSlotsCargo) / m_VinicityEntity->m_RowCargoSlotNmb;

                        //Update pos of CargoSlot
                        for(uint i = 0; i < m_VinicityEntity->m_RowCargoSlotNmb; ++i )
                        {
                            for(uint j = 0; j < m_VinicityEntity->m_ColumnCargoSlotNmb; ++j )
                            {
                                gf::Vector2f pos = firstSlotPos + gf::Vector2f({sizeWidthItem * j + m_SpaceBetweenPlayerSlotsCargo * j, sizeHeightItem * i + m_SpaceBetweenPlayerSlotsCargo * i});
                                gf::Vector2f scale(sizeWidthItem / IconSize, sizeHeightItem / IconSize);

                                m_VinicityCargoSlots.at(i * m_VinicityEntity->m_ColumnCargoSlotNmb + j).update(pos, scale);
                            }
                        }
                    }
                    m_UI.end();
                }

                target.draw( m_UI , states );
                target.draw( m_PlayerWidget, states );

                if ( m_VinicityEntity != nullptr ) target.draw( m_VinictyWidget, states );

                for( auto &x: m_PlayerCargoSlots )
                {
                    x.second.render( target, states );
                }

                for( auto &x: m_PlayerSpecialSlots )
                {
                    x.second.render( target, states );
                }

                for( auto &x: m_VinicityCargoSlots )
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

    void InventoryUI::processEvent(const gf::Event &event)
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

                            for( auto &x: m_PlayerSpecialSlots)
                            {
                                InventoryWidget *itemWidget = x.second.getItemWidget(event.mouseButton.coords);
                                if ( itemWidget != nullptr )
                                {
                                    itemWidget->currDragging = true;
                                    m_OldSlot = &(x.second);
                                    m_CurrMovingWidget = itemWidget;
                                    m_CurrMovingItem = x.second.getItem();
                                    m_OffsetDrag = event.mouseButton.coords - itemWidget->getPosition();
                                    m_OldEntity = m_PlayerEntity;
                                    found = true;
                                    break;
                                }
                            }

                            if (!found)
                            {
                                for( auto &x: m_PlayerCargoSlots)
                                {
                                    InventoryWidget *itemWidget = x.second.getItemWidget(event.mouseButton.coords);
                                    if ( itemWidget != nullptr )
                                    {
                                        itemWidget->currDragging = true;
                                        m_OldSlot = &(x.second);
                                        m_CurrMovingWidget = itemWidget;
                                        m_CurrMovingItem = x.second.getItem();
                                        m_OffsetDrag = event.mouseButton.coords - itemWidget->getPosition();
                                        m_OldEntity = m_PlayerEntity;
                                        found = true;
                                        break;
                                    }
                                }

                                if (!found && m_VinicityEntity != nullptr)
                                {
                                    for( auto &x: m_VinicityCargoSlots)
                                    {
                                        InventoryWidget *itemWidget = x.second.getItemWidget(event.mouseButton.coords);
                                        if ( itemWidget != nullptr )
                                        {
                                            itemWidget->currDragging = true;
                                            m_OldSlot = &(x.second);
                                            m_CurrMovingWidget = itemWidget;
                                            m_CurrMovingItem = x.second.getItem();
                                            m_OffsetDrag = event.mouseButton.coords - itemWidget->getPosition();
                                            m_OldEntity = m_VinicityEntity;
                                            found = true;
                                            break;
                                        }
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

                        for( auto &x: m_PlayerSpecialSlots)
                        {
                            if (x.second.contains(event.mouseButton.coords) && !(x.second.haveItem()) && m_CurrMovingItem->canBeInSlot(x.second.getSlotType()))
                            {
                                //Request move item here
                                Packet packet;
                                packet.type = PacketType::MoveItem;
                                packet.moveItem.oldEntityID = m_OldEntity->getEntityID();
                                packet.moveItem.oldEntityType = m_OldEntity->getEntityType();
                                packet.moveItem.oldSlotType = m_OldSlot->getSlotType();
                                packet.moveItem.oldPos = m_OldSlot->getSlotPos();
                                packet.moveItem.newEntityID = m_PlayerEntity->getEntityID();
                                packet.moveItem.newEntityType = m_PlayerEntity->getEntityType();
                                packet.moveItem.newSlotType = x.second.getSlotType();
                                packet.moveItem.newPos = x.second.getSlotPos();

                                m_Game.sendPacket(packet);

                                sendedPacket = true;
                                break;
                            }
                        }

                        if (!sendedPacket)
                        {
                            for( auto &x: m_PlayerCargoSlots)
                            {
                                if (x.second.contains(event.mouseButton.coords) && (!(x.second.haveItem())) && (m_CurrMovingItem->canBeInSlot(x.second.getSlotType())))
                                {
                                    //Request move item here
                                    Packet packet;
                                    packet.type = PacketType::MoveItem;
                                    packet.moveItem.oldEntityID = m_OldEntity->getEntityID();
                                    packet.moveItem.oldEntityType = m_OldEntity->getEntityType();
                                    packet.moveItem.oldSlotType = m_OldSlot->getSlotType();
                                    packet.moveItem.oldPos = m_OldSlot->getSlotPos();
                                    packet.moveItem.newEntityID = m_PlayerEntity->getEntityID();
                                    packet.moveItem.newEntityType = m_PlayerEntity->getEntityType();
                                    packet.moveItem.newSlotType = x.second.getSlotType();
                                    packet.moveItem.newPos = x.second.getSlotPos();

                                    m_Game.sendPacket(packet);

                                    sendedPacket = true;
                                    break;
                                }
                            }

                            if (!sendedPacket)
                            {
                                for( auto &x: m_VinicityCargoSlots)
                                {
                                    if (x.second.contains(event.mouseButton.coords) && (!(x.second.haveItem())) && (m_CurrMovingItem->canBeInSlot(x.second.getSlotType())))
                                    {
                                        //Request move item here
                                        Packet packet;
                                        packet.type = PacketType::MoveItem;
                                        packet.moveItem.oldEntityID = m_OldEntity->getEntityID();
                                        packet.moveItem.oldEntityType = m_OldEntity->getEntityType();
                                        packet.moveItem.oldSlotType = m_OldSlot->getSlotType();
                                        packet.moveItem.oldPos = m_OldSlot->getSlotPos();
                                        packet.moveItem.newEntityID = m_VinicityEntity->getEntityID();
                                        packet.moveItem.newEntityType = m_VinicityEntity->getEntityType();
                                        packet.moveItem.newSlotType = x.second.getSlotType();
                                        packet.moveItem.newPos = x.second.getSlotPos();

                                        m_Game.sendPacket(packet);

                                        sendedPacket = true;
                                        break;
                                    }
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
                        m_OldEntity = nullptr;
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

    void InventoryUI::setVinicityObject(ClientEntity *vinicityObject)
    {
        m_VinicityCargoSlots.clear();

        if (vinicityObject != nullptr)
        {
            for(uint i = 0; i < vinicityObject->m_RowCargoSlotNmb; ++i )
            {
                for(uint j = 0; j < vinicityObject->m_ColumnCargoSlotNmb; ++j )
                {
                    auto it = m_VinicityCargoSlots.insert(std::make_pair(i * vinicityObject->m_ColumnCargoSlotNmb + j, std::move(InventorySlot(InventorySlotType::Cargo, i * vinicityObject->m_ColumnCargoSlotNmb + j))));
                    assert(it.second);

                    ClientItem *item = vinicityObject->getInventory().getItem(InventorySlotType::Cargo, i * vinicityObject->m_ColumnCargoSlotNmb + j);
                    if (item != nullptr)
                    {
                        it.first->second.setItem(item);
                    }
                }
            }

            gf::Texture *vinicityTexture = vinicityObject->getEntityTexture();
            m_VinictyWidget.setDefaultSprite(*vinicityTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
            m_VinictyWidget.setDisabledSprite(*vinicityTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
            m_VinictyWidget.setSelectedSprite(*vinicityTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        }

        m_VinicityEntity = vinicityObject;
    }

    gf::MessageStatus InventoryUI::onInventoryShowUpdate(gf::Id id, gf::Message *msg)
    {
        assert(id == InventoryShowUpdateMessage::type);
        
        m_ShowInventory = !m_ShowInventory;

        if (!m_ShowInventory && m_VinicityEntity != nullptr)
        {
            setVinicityObject(nullptr);
        }
        
        return gf::MessageStatus::Keep;
    }

    gf::MessageStatus InventoryUI::onMyPlayerReceived(gf::Id id, gf::Message *msg)
    {
        assert(id == MyPlayerReceivedTypeMessage::type);
        
        MyPlayerReceivedTypeMessage *message = static_cast<MyPlayerReceivedTypeMessage*>(msg);
        
        if (message && message->player)
        {
            gf::Texture *playerTexture = message->player->getEntityTexture();
            m_PlayerWidget.setDefaultSprite(*playerTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
            m_PlayerWidget.setDisabledSprite(*playerTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
            m_PlayerWidget.setSelectedSprite(*playerTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));

            m_PlayerEntity = message->player;

            m_PlayerCargoSlots.clear();
            for(uint i = 0; i < m_PlayerEntity->m_RowCargoSlotNmb; ++i )
            {
                for(uint j = 0; j < m_PlayerEntity->m_ColumnCargoSlotNmb; ++j )
                {
                    m_PlayerCargoSlots.insert(std::make_pair(i * m_PlayerEntity->m_ColumnCargoSlotNmb + j, std::move(InventorySlot(InventorySlotType::Cargo, i * m_PlayerEntity->m_ColumnCargoSlotNmb + j))));
                }
            }
        }

        return gf::MessageStatus::Keep;
    }

    gf::MessageStatus InventoryUI::onItemUpdateUI(gf::Id id, gf::Message *msg)
    {
        assert(id == ItemUpdateUIMessage::type);
        
        ItemUpdateUIMessage *message = static_cast<ItemUpdateUIMessage*>(msg);

        if (message)
        {
            if (m_PlayerEntity != nullptr && m_PlayerEntity->getEntityID() == message->entityID)
            {
                if (message->slotType == InventorySlotType::Cargo)
                {
                    auto it = m_PlayerCargoSlots.find(message->slotPos);
                    if (it != m_PlayerCargoSlots.end())
                    {
                        if (message->itemRemoved)
                        {
                            it->second.setItem(nullptr);
                        }
                        else if (message->item)
                        {
                            it->second.setItem(message->item);
                        }
                    }
                }
                else
                {
                    auto it = m_PlayerSpecialSlots.find(message->slotType);
                    if (it != m_PlayerSpecialSlots.end())
                    {
                        if (message->itemRemoved)
                        {
                            it->second.setItem(nullptr);
                        }
                        else if (message->item)
                        {
                            it->second.setItem(message->item);
                        }
                    }
                }
            }
            else if ( m_VinicityEntity != nullptr && m_VinicityEntity->getEntityID() == message->entityID)
            {
                //Could only be cargo for entity other than player
                if (message->slotType == InventorySlotType::Cargo)
                {
                    auto it = m_VinicityCargoSlots.find(message->slotPos);
                    if (it != m_VinicityCargoSlots.end())
                    {
                        if (message->itemRemoved)
                        {
                            it->second.setItem(nullptr);
                        }
                        else if (message->item)
                        {
                            it->second.setItem(message->item);
                        }
                    }
                }
            }
        }

        return gf::MessageStatus::Keep;
    }
}