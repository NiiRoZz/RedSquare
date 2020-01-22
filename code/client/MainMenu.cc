#include "MainMenu.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
namespace redsquare
{
    MainMenu::MainMenu(gf::Font &font)
    : m_UI(font) 
    , m_ShowMainMenu(false)
    {

    }

    void MainMenu::update(gf::Time time)
    {
        
    }

    void MainMenu::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Coordinates coordinates(target);
        gf::Vector2f MainMenuWindowSize=coordinates.getRelativeSize({ 1.0f,1.0f });
        gf::Vector2f sizeNamePlayer=MainMenuWindowSize*gf::Vector2f(0.050,0.7);
        gf::Vector2f sizeEspace=MainMenuWindowSize*gf::Vector2f(0.010,0.7);
        gf::Vector2f sizeEspace2=MainMenuWindowSize*gf::Vector2f(0.030,0.7);
        if(m_ShowMainMenu)
        {
            /*
            if( m_UI.begin("", gf::RectF::fromPositionSize(coordinates.getRelativePoint({ 0.0f,0.0f }),MainMenuWindowSize), gf::UIWindow::NoScrollbar))
            {
                m_UI.layoutRowDynamic(sizeEspace[0], 3);
                m_UI.layoutRowDynamic(sizeNamePlayer[0], 3);
                if (m_UI.groupBegin(""))
                {

                    m_UI.groupEnd();
                } 
                if (m_UI.groupBegin("",gf::UIWindow::Border))
                {

                    m_UI.groupEnd();
                } 
                if (m_UI.groupBegin(""))
                {

                    m_UI.groupEnd();
                } 
                m_UI.layoutRowDynamic(sizeEspace2[0], 3);
                m_UI.layoutRowDynamic(sizeNamePlayer[0], 3);
                if (m_UI.groupBegin(""))
                {

                    m_UI.groupEnd();
                }
                if (m_UI.groupBegin("",gf::UIWindow::Border))
                {

                    m_UI.groupEnd();
                } 
                if (m_UI.groupBegin(""))
                {

                    m_UI.groupEnd();
                }
                m_UI.layoutRowDynamic(sizeEspace2[0], 3);
                float ratioTwo[] = { 0.2f, 0.6f, 0.2f };
                float widthTwo[] = { 100.0f, 200.0f, 50.0f };

                m_UI.layoutRowDynamic(30, 1);
                m_UI.label("Dynamic array-based custom column layout with generated position and custom size:");
                m_UI.layoutRow(gf::UILayout::Dynamic, 100, ratioTwo);
                m_UI.buttonLabel("button");
                m_UI.buttonLabel("button");
                m_UI.buttonLabel("button");
                    
                
                m_UI.end();
            }
        
            target.draw(m_UI);
            */
        }
    }


    void MainMenu::processEvent(const gf::Event &event)
    {
        m_UI.processEvent(event);
    }
}