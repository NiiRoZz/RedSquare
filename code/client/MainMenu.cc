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
    : m_ShowMainMenu(false)
    , m_Test(gf::Vector2i(64,64),gf::Color::Red)
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

            gf::Vector2f MainMenuWindowSize = coordinates.getRelativeSize({1.0f,1.0f });
            gf::Vector2f MainMenuWindowPos = coordinates.getRelativePoint({ 0.2f,0.2f });
            auto position = coordinates.getCenter();

            // UI
            ImGui::SetNextWindowSize(ImVec2(MainMenuWindowSize[0], MainMenuWindowSize[1]));
            ImGui::SetNextWindowPos(ImVec2(MainMenuWindowPos[0], MainMenuWindowPos[1]), 0, ImVec2(0.2f, 0.2f));
            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_FittingPolicyMask_;
            
            if (ImGui::Begin("MainMenu", nullptr, DefaultWindowFlagsTest ))
            {   
                ImGui::NewLine();
                ImGui::NewLine();
                ImGui::Columns(2);

                if (ImGui::InputTextWithHint("Name", "enter your pseudo here (1-15)", m_LineBufferName.getData(), m_LineBufferName.getSize(), ImGuiInputTextFlags_EnterReturnsTrue) && m_LineBufferName[0] != '\0')
                {

                }

                ImGui::NextColumn();

                ImGui::Button("Join game"); ImGui::SameLine();
                if (ImGui::InputTextWithHint("", "enter ip adress here", m_LineBufferIp.getData(), m_LineBufferIp.getSize(), ImGuiInputTextFlags_EnterReturnsTrue) && m_LineBufferIp[0] != '\0')
                {
                    
                }
                
                
                ImGui::Columns(3);
                ImGui::NewLine();
                ImGui::NewLine();
                ImGui::NewLine();
                ImGui::NewLine();

                static float wrap_width = 200.0f;

                ImGui::BulletText("Description");
                ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
                ImGui::Text("The lazy dog is a good dog. This paragraph is made to fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.", wrap_width);
                ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255,255,255,255));

                static float wrap_width2 = 200.0f;
                ImGui::NextColumn();
                ImGui::NewLine();
                ImGui::NewLine();
                ImGui::NewLine();
                ImGui::NewLine();

                ImGui::Button("<--"); ImGui::SameLine();
                ImGui::BulletText("Character");ImGui::SameLine();
                ImGui::Button("-->"); ImGui::SameLine();
                ImGui::BulletText("mage");ImGui::SameLine();
                ImGui::NewLine();
                /*
                ImGuiIO& io = ImGui::GetIO();
                ImGui::Image((void*)(m_Test.getPixelsPtr()), ImVec2(64, 64));
                */
                static float wrap_width3 = 200.0f;
                ImGui::NextColumn();
                ImGui::NewLine();
                ImGui::NewLine();
                ImGui::NewLine();
                ImGui::NewLine();

                ImGui::BulletText("Characteristic");
                ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width3);
                ImGui::Text("The lazy dog is a good dog. This paragraph is made to fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.", wrap_width3);
                ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255,255,255,255));       
            }

            ImGui::End();
        }
    }


    void MainMenu::processEvent(const gf::Event &event)
    {
        
    }

    
}