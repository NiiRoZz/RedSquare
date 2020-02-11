#include "MainMenu.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Widgets.h>
namespace redsquare
{
    MainMenu::MainMenu(gf::Font &font)
    : m_ShowMainMenu(false)
    , m_Test(gf::Vector2i(64,64),gf::Color::Red)
    , m_DescriptionText("", font)
    {

    }

    void MainMenu::update(gf::Time time)
    {
        
    }

    void MainMenu::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        if(m_ShowMainMenu)
        {
            gf::Coordinates coordinates(target);

            gf::Vector2f MainMenuWindowSizeDescription = coordinates.getRelativeSize({0.4f,0.4f });
            gf::Vector2f MainMenuWindowPosDescription = coordinates.getRelativePoint({ 0.1f,0.2f });
            auto position = coordinates.getCenter();

            // UI
            ImGui::SetNextWindowSize(ImVec2(MainMenuWindowSizeDescription[0], MainMenuWindowSizeDescription[1]));
            ImGui::SetNextWindowPos(ImVec2(MainMenuWindowPosDescription[0], MainMenuWindowPosDescription[1]));
            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_FittingPolicyMask_;
            
            if (ImGui::Begin("Description Character", nullptr, DefaultWindowFlagsTest ))
            { 
            float characterSize = coordinates.getRelativeCharacterSize(0.026f);

            m_DescriptionText.setCharacterSize(characterSize);
            m_DescriptionText.setColor(gf::Color::White);
            m_DescriptionText.setAlignment(gf::Alignment::Left);
            m_DescriptionText.setParagraphWidth(MainMenuWindowSizeDescription[0]);
            m_DescriptionText.setString( "okbgdelanisssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssght");
            m_DescriptionText.setPosition(MainMenuWindowPosDescription);
            m_DescriptionText.setOutlineThickness(characterSize * 0.04f);
            target.draw(m_DescriptionText, states); 
            }
            
            
            ImGui::End();
        }
    }


    void MainMenu::processEvent(const gf::Event &event)
    {
        
    }

    
}