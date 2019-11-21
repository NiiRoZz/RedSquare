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
        static gf::UICharBuffer text(64);
        if(m_ShowInventory){
            m_UI.setCharacterSize(18);

            if( m_UI.begin("Inventory", gf::RectF::fromPositionSize( {400, 90}, {535, 400} ),gf::UIWindow::Title|gf::UIWindow::NoScrollbar))
            {
                m_UI.layoutRowStatic(80, 80, 1);

                if (m_UI.groupBegin(""))
                {
                    m_UI.layoutRowDynamic(32, 1);
                    m_UI.image(gResourceManager().getTexture("img/attackCursor.png"),gf::RectF());
                    m_UI.groupEnd();
                }

                m_UI.layoutRowStatic(350, 525, 1);
                int i=0;
                    if (m_UI.groupBegin(""))
                    {
                        for (int i =0; i<7;i++){
                        m_UI.layoutRowDynamic(32, 14);
                        
                            for(int y =0; y<14;y++){
                                 m_UI.image(gResourceManager().getTexture("img/attackCursor.png"),gf::RectF());
                            }
                        }
                        m_UI.groupEnd();
                    } 
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