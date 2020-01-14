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
    Inventory::Inventory(gf::Font &font)
    : m_UI(font)
    , m_ShowInventory(false)
    , m_CurrMovingWidget(nullptr)
    , m_OldSlot(nullptr)
    {
        gMessageManager().registerHandler<InventoryUpdateMessage>(&Inventory::onInventoryUpdate, this);

        for(uint i = 0; i < RowSlotNmb; ++i )
        {
            for(uint j = 0; j < ColumnSlotNmb; ++j )
            {
                m_CargoSlots.push_back(std::move(InventorySlot((i == 0u && j == 0u))));
            }
        }
    }

    void Inventory::update(gf::Time time)
    {
        
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
                gf::Vector2f firstSlotPos({InventoryWindowPoint[0], InventoryWindowPoint[1] + InventoryWindowSize[1] * StartPercentagePos});
                float sizeWidthItem = (InventoryWindowSize[0] - (ColumnSlotNmb - 1) * SpaceBetweenSlots) / ColumnSlotNmb;
                float sizeHeightItem = (InventoryWindowSize[1] * (1 - StartPercentagePos) - (RowSlotNmb - 1) * SpaceBetweenSlots) / RowSlotNmb;

                //Update pos of CargoSlot
                for(uint i = 0; i < RowSlotNmb; ++i )
                {
                    for(uint j = 0; j < ColumnSlotNmb; ++j )
                    {
                        gf::Vector2f pos = firstSlotPos + gf::Vector2f({sizeWidthItem * j + SpaceBetweenSlots * j, sizeHeightItem * i + SpaceBetweenSlots * i});
                        gf::Vector2f scale(sizeWidthItem / IconSize, sizeHeightItem / IconSize);

                        m_CargoSlots[i * ColumnSlotNmb + j].update(pos, scale);
                    }
                }
                 
                m_UI.end();

                target.draw(m_UI);

                for( InventorySlot &currSlot: m_CargoSlots )
                {
                    currSlot.render( target, states );
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
                    for( InventorySlot &currCargoSlot: m_CargoSlots)
                    {
                        InventoryWidget *itemWidget = currCargoSlot.getItemWidget(event.mouseButton.coords);
                        if ( itemWidget != nullptr )
                        {
                            itemWidget->currDragging = true;
                            m_OldSlot = &currCargoSlot;
                            m_CurrMovingWidget = itemWidget;
                            break;
                        }
                    }
                    break;
                }

                case gf::EventType::MouseButtonReleased:
                {
                    if (m_CurrMovingWidget != nullptr)
                    {
                        for( InventorySlot &currCargoSlot: m_CargoSlots)
                        {
                            if (currCargoSlot.contains(event.mouseButton.coords) && &currCargoSlot != m_OldSlot)
                            {
                                currCargoSlot.haveItem = true;
                                m_OldSlot->haveItem = false;
                                break;
                            }
                        }

                        m_CurrMovingWidget->currDragging = false;
                        m_OldSlot = nullptr;
                        m_CurrMovingWidget = nullptr;
                    }
                }

                case gf::EventType::MouseMoved:
                {
                    if (m_CurrMovingWidget != nullptr)
                    {
                        m_CurrMovingWidget->setPosition(event.mouseCursor.coords);
                    }
                    break;
                }
            }
        }
    }

    gf::MessageStatus Inventory::onInventoryUpdate(gf::Id id, gf::Message *msg)
    {
        assert(id == InventoryUpdateMessage::type);
        
        m_ShowInventory = !m_ShowInventory;
        
        return gf::MessageStatus::Keep;
    }
}