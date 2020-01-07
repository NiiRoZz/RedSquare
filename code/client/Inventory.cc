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
        
    }

    void Inventory::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {

        if(m_ShowInventory)
        {
            m_UI.setCharacterSize(18);
            gf::Coordinates coordinates(target);
            gf::Vector2f InventoryWindowSize=coordinates.getRelativeSize({ 0.60f,0.60f });
            gf::Vector2f sizeItem32=InventoryWindowSize*gf::Vector2f(0.050,0.094);  
            gf::Vector2f sizeItem48=coordinates.getRelativeSize({ 0.046f, 0.083f });
            gf::Vector2f sizeItem140=InventoryWindowSize*gf::Vector2f(0.3,0.5);
            gf::Vector2f sizeGroupItem=InventoryWindowSize*gf::Vector2f(0.408,1.14);
        
        
            if( m_UI.begin("Inventory", gf::RectF::fromPositionSize( coordinates.getRelativePoint({ 0.35f,0.15f }), InventoryWindowSize ), gf::UIWindow::Title|gf::UIWindow::NoScrollbar))
            {
                
                m_UI.layoutRowStatic((InventoryWindowSize[0]*0.3), InventoryWindowSize[1], 1);
                /*
                if (m_UI.groupBegin("", gf::UIWindow::NoScrollbar))
                {
                    m_UI.layoutRowStatic(sizeItem140[0],sizeItem140[1], 2);
                    m_UI.image(gResourceManager().getTexture("img/goblin.png"),gf::RectF::fromPositionSize({0,0},{1,1}));
                    m_UI.groupEnd();
                }
                if (m_UI.groupBegin("StuffDef", gf::UIWindow::NoScrollbar))
                {   
                    m_UI.layoutRowStatic(sizeItem32[0],sizeItem32[1], 1);
                    m_UI.image(gResourceManager().getTexture("img/goblin.png"),gf::RectF::fromPositionSize({0,0},{1,1}));

                    m_UI.layoutRowStatic(sizeItem32[0],sizeItem32[1], 1);
                    m_UI.image(gResourceManager().getTexture("img/goblin.png"),gf::RectF::fromPositionSize({0,0},{1,1}));

                    m_UI.layoutRowStatic(sizeItem32[0],sizeItem32[1], 1);
                    m_UI.image(gResourceManager().getTexture("img/goblin.png"),gf::RectF::fromPositionSize({0,0},{1,1}));

                    m_UI.layoutRowStatic(sizeItem32[0],sizeItem32[1], 1);
                    m_UI.image(gResourceManager().getTexture("img/goblin.png"),gf::RectF::fromPositionSize({0,0},{1,1}));
                        
                    m_UI.groupEnd();
                }
                if (m_UI.groupBegin("StuffOff", gf::UIWindow::NoScrollbar))
                {
                    m_UI.layoutRowStatic(sizeItem48[0],sizeItem48[1], 2);
                    m_UI.layoutRowStatic(sizeItem32[0],sizeItem32[1], 1);
                    m_UI.image(gResourceManager().getTexture("img/goblin.png"),gf::RectF::fromPositionSize({0,0},{1,1}));
                    m_UI.image(gResourceManager().getTexture("img/goblin.png"),gf::RectF::fromPositionSize({0,0},{1,1}));

                    m_UI.groupEnd();
                }   
                    
                m_UI.layoutRowStatic(sizeGroupItem[0], sizeGroupItem[1], 1);
                
                */
                //m_UI.layoutRowStatic((InventoryWindowSize[0]*0.7), InventoryWindowSize[1], );
                
                    
                    for (int i =0; i<6;i++){
                        m_UI.layoutRowDynamic(InventoryWindowSize[0] / 15, 12);
                        for(int y =0; y<12;y++){
                            
                            m_UI.image(gResourceManager().getTexture("img/attackCursor.png"),gf::RectF(gf::RectF::fromPositionSize({0,0},{1,1})));
                        }
                    }
                 
                m_UI.end();
            }

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