#include "InventorySlot.h"
#include "../common/Singletons.h"

#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>

namespace redsquare
{
    InventorySlot::InventorySlot(bool item)
    : m_BackgroundTexture(gResourceManager().getTexture("img/case_selected.png"))
    , m_BackgroundWidget(m_BackgroundTexture, m_BackgroundTexture, m_BackgroundTexture)
    , m_ItemTexture(gResourceManager().getTexture("img/attackCursor.png"))
    , m_ItemWidget(m_ItemTexture, m_ItemTexture, m_ItemTexture)
    , haveItem(item)
    {

    }

    void InventorySlot::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        target.draw( m_BackgroundWidget, states );

        /*if (m_Item != nullptr)
        {
            target.draw( m_ItemWidget, states );
        }*/
        if (haveItem)
        {
            target.draw( m_ItemWidget, states );
        }
    }

    void InventorySlot::update(gf::Vector2f pos, gf::Vector2f scale)
    {
        m_BackgroundWidget.setPosition(pos);
        m_BackgroundWidget.setScale(scale);

        /*if (m_Item != nullptr)
        {
            if (!m_ItemWidget.currDragging)
            {
                m_ItemWidget.setPosition(pos);
            }

            m_ItemWidget.setScale(scale);
        }*/

        if (haveItem)
        {
            if (!m_ItemWidget.currDragging)
            {
                m_ItemWidget.setPosition(pos);
            }

            m_ItemWidget.setScale(scale);
        }
    }

    InventoryWidget* InventorySlot::getItemWidget(gf::Vector2f coords)
    {
        /*if (m_Item != nullptr && m_ItemWidget.contains(coords))
        {
            return &m_ItemWidget;
        }*/

        if ( haveItem && m_ItemWidget.contains(coords) )
        {
            return &m_ItemWidget;
        }

        return nullptr;
    }

    bool InventorySlot::contains(gf::Vector2f coords)
    {
        return m_BackgroundWidget.contains(coords);
    }
}