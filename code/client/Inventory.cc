#include "Inventory.h"
#include "../common/Singletons.h"
#include "Game.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>


namespace redsquare
{
    Inventory::Inventory(gf::Font &font)
    : m_UI(font)
    , m_ShowInventory(false)
    {
         gMessageManager().registerHandler<InventoryUpdateMessage>(&Inventory::onInventoryUpdate, this);
    }


    void Inventory::update(gf::Time time)
    {
        if(m_ShowInventory){
            static gf::UICharBuffer text(64);

            m_UI.setCharacterSize(12);

            if( m_UI.begin("Inventory", gf::RectF::fromPositionSize( {450, 50}, {520, 400} ),gf::UIWindow::Title|gf::UIWindow::NoScrollbar))
            {
                m_UI.end();
            }
        }
    }

    void Inventory::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        if(m_ShowInventory){
            target.draw(m_UI);
        }
    }

    void Inventory::processEvent(const gf::Event &event)
    {
        m_UI.processEvent(event);
    }

    gf::MessageStatus Inventory::onInventoryUpdate(gf::Id id, gf::Message *msg)
    {
        assert(id == InventoryUpdateMessage::type);
        
        m_ShowInventory=!m_ShowInventory;
        
        return gf::MessageStatus::Keep;
    }
}