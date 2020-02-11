#include "MainMenu.h"
#include "InventoryWidget.h"

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

            auto position = coordinates.getCenter();
            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_FittingPolicyMask_;

            gf::Vector2f MainMenuWindowSizeDescription = coordinates.getRelativeSize({0.25f,0.25f });
            gf::Vector2f MainMenuWindowPosDescription = coordinates.getRelativePoint({ 0.025,0.65f });
            ImGui::SetNextWindowSize(ImVec2(MainMenuWindowSizeDescription[0], MainMenuWindowSizeDescription[1]));
            ImGui::SetNextWindowPos(ImVec2(MainMenuWindowPosDescription[0], MainMenuWindowPosDescription[1]));
            
            ImGui::Begin("Description Character", nullptr, DefaultWindowFlagsTest);
            
            ImVec2 descP=ImGui::GetWindowSize();
            ImGui::TextWrapped("The logging API redirects all text output so you can easily capture the content of a window or a block. Tree nodes can be automatically expanded.");
            ImGui::SetWindowFontScale(descP[0]/200);

            ImGui::End();
            
            gf::Vector2f startPlayerWidgetSize = coordinates.getRelativeSize({0.20f,0.20f });
            gf::Vector2f startPlayerWidgetPos = coordinates.getRelativePoint({ 0.40,0.65f });
            m_PlayerWidget.setPosition(startPlayerWidgetPos);
            gf::Texture playerTexture("../data/redsquare/img/Character/Magus.png");
            m_PlayerWidget.setDefaultSprite(playerTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
            m_PlayerWidget.setDisabledSprite(playerTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
            m_PlayerWidget.setSelectedSprite(playerTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
            m_PlayerWidget.setPosition(startPlayerWidgetPos);
            m_PlayerWidget.setScale(startPlayerWidgetSize/playerTexture.getSize());
            target.draw(m_PlayerWidget,states);

            gf::Vector2f MainMenuWindowSizeCharac = coordinates.getRelativeSize({0.25f,0.25f });
            gf::Vector2f MainMenuWindowPosCharac = coordinates.getRelativePoint({ 0.73,0.65f });
            ImGui::SetNextWindowSize(ImVec2(MainMenuWindowSizeCharac[0], MainMenuWindowSizeCharac[1]));
            ImGui::SetNextWindowPos(ImVec2(MainMenuWindowPosCharac[0], MainMenuWindowPosCharac[1]));

            ImGui::Begin("Characteristic Character", nullptr, DefaultWindowFlagsTest);
            
            ImVec2 charP=ImGui::GetWindowSize();
            ImGui::TextWrapped("The logging API redirects all text output so you can easily capture the content of a window or a block. Tree nodes can be automatically expanded.");
            ImGui::SetWindowFontScale(charP[0]/200);

            ImGui::End();
            
        }
    }


    void MainMenu::processEvent(const gf::Event &event)
    {
        
    }

    
}