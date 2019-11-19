#include "Inventory.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>

namespace redsquare
{
    Inventory::Inventory(gf::Font &font)
    : m_UI(font)
    {

    }


    void Inventory::update(gf::Time time)
    {
    
        
    }

    void Inventory::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        target.draw(m_UI);
    }

    void Inventory::processEvent(const gf::Event &event)
    {
        m_UI.processEvent(event);
    }
}