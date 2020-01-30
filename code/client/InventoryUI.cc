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
    : m_Game(game)
    , m_UI(font)
    , m_PlayerEntity(nullptr)
    , m_VinicityEntity(nullptr)
    , m_PlayerWidget(gResourceManager().getTexture("img/goblin.png"))
    , m_OldSlot(nullptr)
    , m_CurrMovingWidget(nullptr)
    , m_CurrMovingItem(nullptr)
    , m_DraggingFromEntity(nullptr)
    , m_HoveringSlot(nullptr)
    , m_NameWidget("", font, 12)
    , m_DescriptionWidget("", font, 12)
    , m_RightClickedSlot(nullptr)
    , m_RightClickedFromEntity(nullptr)
    , m_DropButton("Drop", font, 12)
    , m_UseButton("Use", font, 12)
    {
        gMessageManager().registerHandler<ItemUpdateUIMessage>(&InventoryUI::onItemUpdateUI, this);
        gMessageManager().registerHandler<MyPlayerReceivedTypeMessage>(&InventoryUI::onMyPlayerReceived, this);

        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::Helmet, std::move(InventorySlot(InventorySlotType::Helmet))));
        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::ChestPlate, std::move(InventorySlot(InventorySlotType::ChestPlate))));
        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::Legging, std::move(InventorySlot(InventorySlotType::Legging))));
        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::Boot, std::move(InventorySlot(InventorySlotType::Boot))));
        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::Weapon, std::move(InventorySlot(InventorySlotType::Weapon))));
        m_PlayerSpecialSlots.insert(std::make_pair(InventorySlotType::Shield, std::move(InventorySlot(InventorySlotType::Shield))));

        m_NameWidget.setAlignment(gf::Alignment::Center);
        m_DescriptionWidget.setAlignment(gf::Alignment::Left);
        m_NameBackgroundShape.setColor(gf::Color4f(1,1,1,0.5));
        m_DescriptionBackgroundShape.setColor(gf::Color4f(1,0,0,0.5));
        m_RightClickedBackground.setColor(gf::Color4f(0.25,0.25,0.25,0.80));

        m_DropButton.setDefaultBackgroundColor(gf::Color4f(0.25,0.25,0.25,0.80));
        m_DropButton.setState(gf::WidgetState::Default);
        m_DropButton.setAnchor(gf::Anchor::TopLeft);

        m_UseButton.setDefaultBackgroundColor(gf::Color4f(0.25,0.25,0.25,0.80));
        m_UseButton.setState(gf::WidgetState::Default);
        m_UseButton.setAnchor(gf::Anchor::TopLeft);
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

            //Draw information about hovering item
            if (m_HoveringSlot != nullptr)
            {
                //Calculate correctly the size of the text
                float paragraphWidth = coordinates.getRelativeSize({ 0.11f, 1.f })[0];
                unsigned int characterSize = coordinates.getRelativeCharacterSize(0.025f);

                m_NameWidget.setParagraphWidth(paragraphWidth);
                m_NameWidget.setCharacterSize(characterSize);

                m_DescriptionWidget.setParagraphWidth(paragraphWidth);
                m_DescriptionWidget.setCharacterSize(characterSize);

                //Calculate correctly the backgroundShape for m_NameWidget
                gf::RectF bounds = m_NameWidget.getLocalBounds();
                m_NameBackgroundShape.setSize(bounds.getSize());
                m_NameBackgroundShape.setOrigin(m_NameWidget.getOrigin());

                gf::Vector2f position = m_NameWidget.getPosition();
                gf::Matrix3f matrix = gf::rotation(m_NameWidget.getRotation(), position) * gf::scaling(m_NameWidget.getScale(), position);

                m_NameBackgroundShape.setPosition(gf::transform(matrix, position + bounds.getPosition()));
                m_NameBackgroundShape.setRotation(m_NameWidget.getRotation());
                m_NameBackgroundShape.setScale(m_NameWidget.getScale());

                target.draw( m_NameBackgroundShape, states );
                target.draw( m_NameWidget, states );

                //Set correctly the position of m_DescriptionWidget
                m_DescriptionWidget.setPosition(m_NameBackgroundShape.getPosition() + gf::Vector2f(0 , m_NameBackgroundShape.getSize()[1] * 2));

                //Calculate correctly the backgroundShape for m_DescriptionWidget
                bounds = m_DescriptionWidget.getLocalBounds();
                m_DescriptionBackgroundShape.setSize(bounds.getSize());
                m_DescriptionBackgroundShape.setOrigin(m_DescriptionWidget.getOrigin());

                position = m_DescriptionWidget.getPosition();
                matrix = gf::rotation(m_DescriptionWidget.getRotation(), position) * gf::scaling(m_DescriptionWidget.getScale(), position);

                m_DescriptionBackgroundShape.setPosition(gf::transform(matrix, position + bounds.getPosition()));
                m_DescriptionBackgroundShape.setRotation(m_DescriptionWidget.getRotation());
                m_DescriptionBackgroundShape.setScale(m_DescriptionWidget.getScale());

                target.draw( m_DescriptionBackgroundShape, states );
                target.draw( m_DescriptionWidget, states );
            }

            //Draw use/drop buttons and his background
            if (m_RightClickedSlot != nullptr)
            {
                if (m_RightClickedSlot->haveItem())
                {
                    unsigned int characterSize = coordinates.getRelativeCharacterSize(0.035f);

                    m_DropButton.setCharacterSize(characterSize);
                    m_UseButton.setCharacterSize(characterSize);

                    m_RightClickedBackground.setPosition(m_RightClickedSlot->getBackgroundWidget()->getPosition());
                    gf::Vector2f rightClickedBackgroundSize = coordinates.getRelativeSize({ 0.11f, 0.05f });
                    m_RightClickedBackground.setSize(rightClickedBackgroundSize);

                    target.draw( m_RightClickedBackground, states );

                    m_DropButton.setPosition(m_RightClickedBackground.getPosition() + rightClickedBackgroundSize * gf::Vector2f(0.1f, 0.5f));
                    m_UseButton.setPosition(m_RightClickedBackground.getPosition() + rightClickedBackgroundSize * gf::Vector2f(0.6f, 0.5f));

                    target.draw( m_DropButton, states );

                    if (m_RightClickedSlot->getItem()->isUseable())
                    {
                        target.draw( m_UseButton, states );
                    }
                }
                else
                {
                    m_RightClickedSlot = nullptr;
                }
            }
        }
    }

    void InventoryUI::processEvent(const gf::Event &event)
    {
        m_UI.processEvent(event);

        switch (event.type)
        {
            case gf::EventType::MouseButtonPressed:
            {
                switch (event.mouseButton.button)
                {
                    case gf::MouseButton::Left:
                    {
                        //Remove right clicked widget if left clicked outside of button
                        if (m_RightClickedSlot != nullptr && !m_UseButton.contains(event.mouseButton.coords) && !m_DropButton.contains(event.mouseButton.coords))
                        {
                            m_RightClickedSlot = nullptr;
                            m_RightClickedFromEntity = nullptr;
                        }
                        else if (m_RightClickedSlot != nullptr)
                        {
                            if (m_UseButton.contains(event.mouseButton.coords))
                            {
                                if (m_RightClickedSlot != nullptr && m_RightClickedSlot->haveItem() && m_RightClickedSlot->getItem()->isUseable())
                                {
                                    Packet packet;
                                    packet.type = PacketType::RequestUse;
                                    packet.requestUse.entityID = m_RightClickedFromEntity->getEntityID();
                                    packet.requestUse.entityType = m_RightClickedFromEntity->getEntityType();
                                    packet.requestUse.playerID = m_PlayerEntity->getEntityID();
                                    packet.requestUse.slotType = m_RightClickedSlot->getSlotType();
                                    packet.requestUse.pos = m_RightClickedSlot->getSlotPos();

                                    m_Game.sendPacket(packet);
                                }

                                m_RightClickedSlot = nullptr;
                                m_RightClickedFromEntity = nullptr;
                            }
                            else if (m_DropButton.contains(event.mouseButton.coords))
                            {
                                if (m_RightClickedSlot != nullptr && m_RightClickedSlot->haveItem())
                                {
                                    Packet packet;
                                    packet.type = PacketType::RequestDrop;
                                    packet.requestDrop.entityID = m_RightClickedFromEntity->getEntityID();
                                    packet.requestDrop.entityType = m_RightClickedFromEntity->getEntityType();
                                    packet.requestDrop.playerID = m_PlayerEntity->getEntityID();
                                    packet.requestDrop.slotType = m_RightClickedSlot->getSlotType();
                                    packet.requestDrop.pos = m_RightClickedSlot->getSlotPos();

                                    m_Game.sendPacket(packet);
                                }

                                m_RightClickedSlot = nullptr;
                                m_RightClickedFromEntity = nullptr;
                            }
                        }

                        //Detect the drag
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
                                m_DraggingFromEntity = m_PlayerEntity;
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
                                    m_DraggingFromEntity = m_PlayerEntity;
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
                                        m_DraggingFromEntity = m_VinicityEntity;
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
                        bool found = false;

                        for( auto &x: m_PlayerSpecialSlots)
                        {
                            if (x.second.contains(event.mouseButton.coords) && x.second.haveItem())
                            {
                                m_RightClickedSlot = &(x.second);
                                m_RightClickedFromEntity = m_PlayerEntity;
                                found = true;
                                break;
                            }
                        }

                        if (!found)
                        {
                            for( auto &x: m_PlayerCargoSlots)
                            {
                                if (x.second.contains(event.mouseButton.coords) && x.second.haveItem())
                                {
                                    m_RightClickedSlot = &(x.second);
                                    m_RightClickedFromEntity = m_PlayerEntity;
                                    found = true;
                                    break;
                                }
                            }

                            if (!found && m_VinicityEntity != nullptr)
                            {
                                for( auto &x: m_VinicityCargoSlots)
                                {
                                    if (x.second.contains(event.mouseButton.coords) && x.second.haveItem())
                                    {
                                        m_RightClickedSlot = &(x.second);
                                        m_RightClickedFromEntity = m_VinicityEntity;
                                        found = true;
                                        break;
                                    }
                                }
                            }
                        }
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
                            packet.moveItem.oldEntityID = m_DraggingFromEntity->getEntityID();
                            packet.moveItem.oldEntityType = m_DraggingFromEntity->getEntityType();
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
                                packet.moveItem.oldEntityID = m_DraggingFromEntity->getEntityID();
                                packet.moveItem.oldEntityType = m_DraggingFromEntity->getEntityType();
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
                                    packet.moveItem.oldEntityID = m_DraggingFromEntity->getEntityID();
                                    packet.moveItem.oldEntityType = m_DraggingFromEntity->getEntityType();
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
                    m_DraggingFromEntity = nullptr;
                    m_OffsetDrag = {0,0};
                }
            }

            case gf::EventType::MouseMoved:
            {
                if (m_CurrMovingWidget != nullptr)
                {
                    if (m_HoveringSlot != nullptr) m_HoveringSlot = nullptr;
                    if (m_RightClickedSlot != nullptr) m_RightClickedSlot = nullptr;

                    m_CurrMovingWidget->setPosition(event.mouseCursor.coords - m_OffsetDrag);
                }
                else
                {
                    if (m_HoveringSlot != nullptr)
                    {
                        if (m_HoveringSlot->contains(event.mouseCursor.coords) && m_HoveringSlot->haveItem() && !(m_HoveringSlot->hasMoveItemRequest()) && m_RightClickedSlot == nullptr)
                        {
                            m_NameWidget.setPosition(event.mouseCursor.coords);
                        }
                        else
                        {
                            m_HoveringSlot = nullptr;
                        }
                    }
                    else if (m_RightClickedSlot == nullptr)
                    {
                        m_HoveringSlot = nullptr;
                        bool findWidget = false;

                        for( auto &x: m_PlayerSpecialSlots)
                        {
                            if (x.second.contains(event.mouseCursor.coords) && x.second.haveItem() && !(x.second.hasMoveItemRequest()))
                            {
                                m_HoveringSlot = &(x.second);

                                m_NameWidget.setString( x.second.getItem()->getName() );
                                m_NameWidget.setPosition(event.mouseCursor.coords);
                                m_DescriptionWidget.setString( x.second.getItem()->getDescription() );

                                findWidget = true;
                                break;
                            }
                        }

                        if (!findWidget)
                        {
                            for( auto &x: m_PlayerCargoSlots)
                            {
                                if (x.second.contains(event.mouseCursor.coords) && x.second.haveItem() && !(x.second.hasMoveItemRequest()))
                                {
                                    m_HoveringSlot = &(x.second);

                                    m_NameWidget.setString( x.second.getItem()->getName() );
                                    m_NameWidget.setPosition(event.mouseCursor.coords);
                                    m_DescriptionWidget.setString( x.second.getItem()->getDescription() );

                                    findWidget = true;
                                    break;
                                }
                            }

                            if (!findWidget)
                            {
                                for( auto &x: m_VinicityCargoSlots)
                                {
                                    if (x.second.contains(event.mouseCursor.coords) && x.second.haveItem() && !(x.second.hasMoveItemRequest()))
                                    {
                                        m_HoveringSlot = &(x.second);

                                        m_NameWidget.setString( x.second.getItem()->getName() );
                                        m_NameWidget.setPosition(event.mouseCursor.coords);
                                        m_DescriptionWidget.setString( x.second.getItem()->getDescription() );

                                        findWidget = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
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