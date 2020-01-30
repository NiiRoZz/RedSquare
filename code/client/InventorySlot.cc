#include "InventorySlot.h"
#include "../common/Singletons.h"

#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>
#include <gf/Rect.h>

namespace redsquare
{
    InventorySlot::InventorySlot(InventorySlotType slotType, uint pos)
    : m_BackgroundTexture(gResourceManager().getTexture("img/Inventory/BorderSlot.png"))
    , m_BackgroundWidget(m_BackgroundTexture)
    , m_Item(nullptr)
    , m_ItemWidget()
    , m_SlotType(slotType)
    , m_SlotPos(pos)
    , m_MoveItemRequested(false)
    , m_TimeSinceMoveItemRequest(gf::seconds(0))
    {

    }

    void InventorySlot::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        target.draw( m_BackgroundWidget, states );

        if (m_Item != nullptr && !(m_ItemWidget.currDragging) && !m_MoveItemRequested)
        {
            target.draw( m_ItemWidget, states );
        }
    }

    void InventorySlot::update(gf::Vector2f pos, gf::Vector2f scale)
    {
        m_BackgroundWidget.setPosition(pos);
        m_BackgroundWidget.setScale(scale);

        if (m_Item != nullptr)
        {
            if (!(m_ItemWidget.currDragging))
            {
                m_ItemWidget.setPosition(pos);
            }

            m_ItemWidget.setScale(scale);
        }
    }

    void InventorySlot::update(gf::Time time)
    {
        if (m_MoveItemRequested)
        {
            m_TimeSinceMoveItemRequest += time;

            if (m_TimeSinceMoveItemRequest.asSeconds() > 2.0f)
            {
                resetMoveItemRequest();
            }
        }
    }

    InventoryWidget* InventorySlot::getBackgroundWidget()
    {
        return &m_BackgroundWidget;
    }

    InventoryWidget* InventorySlot::getItemWidget(gf::Vector2f coords)
    {
        if (m_Item != nullptr && m_ItemWidget.contains(coords))
        {
            return &m_ItemWidget;
        }

        return nullptr;
    }

    bool InventorySlot::contains(gf::Vector2f coords)
    {
        return m_BackgroundWidget.contains(coords);
    }

    InventorySlotType InventorySlot::getSlotType() const
    {
        return m_SlotType;
    }

    uint InventorySlot::getSlotPos() const
    {
        return m_SlotPos;
    }

    bool InventorySlot::haveItem() const
    {
        return (m_Item != nullptr);
    }

    ClientItem* InventorySlot::getItem() const
    {
        return m_Item;
    }

    void InventorySlot::setItem(ClientItem *item)
    {
        resetMoveItemRequest();

        if (item == nullptr)
        {
            m_Item = nullptr;
            m_ItemWidget.unsetTextures();
            return;
        }

        m_Item = item;
        m_ItemWidget.setDefaultSprite(item->getIcon(), gf::RectF::fromPositionSize({0, 0}, {1, 1}));
        m_ItemWidget.setSelectedSprite(item->getIcon(), gf::RectF::fromPositionSize({0, 0}, {1, 1}));
        m_ItemWidget.setDisabledSprite(item->getIcon(), gf::RectF::fromPositionSize({0, 0}, {1, 1}));
    }

    void InventorySlot::setMoveItemRequest()
    {
        m_MoveItemRequested = true;
        m_TimeSinceMoveItemRequest = gf::Time::Zero;
    }

    void InventorySlot::resetMoveItemRequest()
    {
        m_MoveItemRequested = false;
        m_TimeSinceMoveItemRequest = gf::Time::Zero;
    }

    bool InventorySlot::hasMoveItemRequest() const
    {
        return m_MoveItemRequested;
    }
}