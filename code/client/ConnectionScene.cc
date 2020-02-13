#include "ConnectionScene.h"
#include "Scenes.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Widgets.h>
#include <gf/Unused.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

constexpr ImGuiWindowFlags DefaultWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;

namespace redsquare
{
    ConnectionScene::ConnectionScene(Scenes& scenes)
    : gf::Scene({1024, 576})
    , m_Scenes(scenes)
    , m_Font(scenes.resources.getFont("font/arial.ttf"))
    , m_GaucheButton("<===", m_Font, 25)
    , m_DroiteButton("===>", m_Font, 25)
    , m_ConnectionAsked(false)
    , m_DisplayEntitySubType(EntitySubType::Unknow)
    {
        setClearColor(gf::Color::Black);

        m_HostNameBuffer = "localhost";
        m_PortBuffer = "6000";
        m_NameBuffer = "toto";

        m_GaucheButton.setCallback([this]() { displayNextSubEntityTexture(-1); });
        m_Container.addWidget(m_GaucheButton);

        m_DroiteButton.setCallback([this]() { displayNextSubEntityTexture(1); });
        m_Container.addWidget(m_DroiteButton);

        displayNextSubEntityTexture(1);
    }

    void ConnectionScene::doHandleActions(gf::Window& window)
    {
        gf::unused(window);
    }

    bool ConnectionScene::doEarlyProcessEvent(gf::Event &event)
    {
        return ImGui_ImplGF_ProcessEvent(event);
    }

    void ConnectionScene::doProcessEvent(gf::Event &event)
    {
        switch (event.type) {
        case gf::EventType::MouseMoved:
            m_Container.pointTo(m_Scenes.getRenderer().mapPixelToCoords(event.mouseCursor.coords));
            break;

        case gf::EventType::MouseButtonPressed:
            m_Container.pointTo(m_Scenes.getRenderer().mapPixelToCoords(event.mouseButton.coords));
            m_Container.triggerAction();
            break;

        default:
            break;
        }
    }

    void ConnectionScene::doUpdate(gf::Time time)
    {
        ImGui_ImplGF_Update(time);
    }

    void ConnectionScene::doRender(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Coordinates coordinates(target);

        ImGui::NewFrame();

        auto position = coordinates.getCenter();
        ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 1.1f));

        static const ImVec2 DefaultButtonSize = { 170.0f, 40.0f };

        if (ImGui::Begin("Connect", nullptr, DefaultWindowFlags)) {
            ImGui::Text("Hostname:");
            ImGui::SameLine();
            float x = ImGui::GetCursorPosX();
            ImGui::InputText("###hostname", m_HostNameBuffer.getData(), m_HostNameBuffer.getSize());

            ImGui::Text("Port:");
            ImGui::SameLine();
            ImGui::SetCursorPosX(x);
            ImGui::InputText("###port", m_PortBuffer.getData(), m_PortBuffer.getSize());

            ImGui::Text("Name:");
            ImGui::SameLine();
            ImGui::SetCursorPosX(x);
            ImGui::InputText("###name", m_NameBuffer.getData(), m_NameBuffer.getSize());
            ImGui::SetItemDefaultFocus();

            ImGui::Indent();

            if (ImGui::Button("Back", DefaultButtonSize))
            {
                m_Scenes.replaceScene(m_Scenes.mainMenu, m_Scenes.glitchEffect, gf::seconds(0.4f));
                m_ConnectionAsked = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Connect", DefaultButtonSize))
            {
                if (m_Scenes.game.connect(m_HostNameBuffer.getData(), m_PortBuffer.getData(), m_NameBuffer.getData(), m_DisplayEntitySubType))
                {
                    m_Scenes.replaceScene(m_Scenes.game, m_Scenes.glitchEffect, gf::seconds(0.4f));
                }
                else
                {
                    m_ConnectionAsked = true;
                }
            }

            if (m_ConnectionAsked)
            {
                ImGui::Text("Error: unable to connect to server.");
            }
        }
        ImGui::End();

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_FittingPolicyMask_;

        gf::Vector2f MainMenuWindowSizeDescription = coordinates.getRelativeSize({0.25f,0.25f });
        gf::Vector2f MainMenuWindowPosDescription = coordinates.getRelativePoint({ 0.025,0.65f });
        ImGui::SetNextWindowSize(ImVec2(MainMenuWindowSizeDescription[0], MainMenuWindowSizeDescription[1]));
        ImGui::SetNextWindowPos(ImVec2(MainMenuWindowPosDescription[0], MainMenuWindowPosDescription[1]));
        
        ImGui::Begin("Description Character", nullptr, DefaultWindowFlags);
        
        ImVec2 descP=ImGui::GetWindowSize();
        if(m_DisplayEntitySubType == EntitySubType::Magus)
        {
            ImGui::TextWrapped("Magus demolish their foes with incantations. Although they wield powerful offensive spells, mages are fragile and lightly armored, making them particularly vulnerable to close-range attacks. Wise mages make careful use of their spells to keep their foes at a distance or hold them in place.");
        }
        if(m_DisplayEntitySubType == EntitySubType::Warrior)
        {
            ImGui::TextWrapped("Warriors equip themselves carefully for combat and engage their enemies head-on, letting attacks glance off their heavy armor. They use diverse combat tactics and a wide variety of weapon types to protect their more vulnerable allies.");
        }
        if(m_DisplayEntitySubType == EntitySubType::Rogue)
        {
            ImGui::TextWrapped("Rogues often initiate combat with a surprise attack from the shadows, leading with vicious melee strikes. When in protracted battles, they utilize a successive combination of carefully chosen attacks to soften the enemy up for a killing blow.");
        }
        if(m_DisplayEntitySubType == EntitySubType::Ranger)
        {
            ImGui::TextWrapped("Ranger battle their foes at a distance, they nock their arrows between the two eyes. Though their missile weapons are effective at short and long ranges, hunters are also highly mobile.");
        }
        if(m_DisplayEntitySubType == EntitySubType::Healer)
        {
            ImGui::TextWrapped("Priests use powerful healing magic to fortify themselves and their allies. They also wield powerful offensive spells from a distance, but can be overwhelmed by enemies due to their physical frailty and minimal armor. Experienced priests carefully balance the use of their offensive powers when tasked with keeping their party alive.");
        }
        ImGui::SetWindowFontScale(descP[0]/290);

        ImGui::End();

        
        gf::Vector2f GauchePosButton = coordinates.getRelativePoint({ 0.35,0.65f });
        m_GaucheButton.setPosition({ GauchePosButton[0],GauchePosButton[1]});
        m_GaucheButton.setTextOutlineThickness(1.0f);
        m_GaucheButton.setAnchor(gf::Anchor::TopLeft);
        m_GaucheButton.setDefaultTextColor(gf::Color::White);
        m_GaucheButton.setDefaultTextOutlineColor(gf::Color::Black);
        m_GaucheButton.setSelectedTextOutlineColor(gf::Color::Black);
        target.draw(m_GaucheButton,states);
        
        gf::Vector2f startPlayerWidgetSize = coordinates.getRelativeSize({0.20f,0.27f });
        gf::Vector2f startPlayerWidgetPos = coordinates.getRelativePoint({ 0.40,0.67f });
        m_PlayerWidget.setPosition(startPlayerWidgetPos);
        m_PlayerWidget.setPosition(startPlayerWidgetPos);
        m_PlayerWidget.setScale(startPlayerWidgetSize/16.f);
        target.draw(m_PlayerWidget,states);

        gf::Vector2f DroitePosButton = coordinates.getRelativePoint({ 0.60,0.65f });
        m_DroiteButton.setPosition({ DroitePosButton[0],DroitePosButton[1]});
        m_DroiteButton.setTextOutlineThickness(1.0f);
        m_DroiteButton.setAnchor(gf::Anchor::TopLeft);
        m_DroiteButton.setDefaultTextColor(gf::Color::White);
        m_DroiteButton.setDefaultTextOutlineColor(gf::Color::Black);
        m_DroiteButton.setSelectedTextOutlineColor(gf::Color::Black);
        target.draw(m_DroiteButton,states);

        gf::Vector2f MainMenuWindowSizeCharac = coordinates.getRelativeSize({0.25f,0.25f });
        gf::Vector2f MainMenuWindowPosCharac = coordinates.getRelativePoint({ 0.73,0.65f });
        ImGui::SetNextWindowSize(ImVec2(MainMenuWindowSizeCharac[0], MainMenuWindowSizeCharac[1]));
        ImGui::SetNextWindowPos(ImVec2(MainMenuWindowPosCharac[0], MainMenuWindowPosCharac[1]));

        ImGui::Begin("Characteristic Character", nullptr, DefaultWindowFlags);
        
        ImVec2 charP=ImGui::GetWindowSize();
        if(m_DisplayEntitySubType == EntitySubType::Magus)
        {
            std::string text= "Lifepoint : " + std::to_string(MAGUS_LIFEPOINT)+"\n"+"Manapoint : " + std::to_string(MAGUS_MANAPOINT)+"\n"+"Attack : " + std::to_string(MAGUS_ATTACKPOINT)+"\n"+"Defense : " + std::to_string(MAGUS_DEFENSEPOINT)+"\n"+"Range : " + std::to_string(MAGUS_RANGE);
            ImGui::TextWrapped(text.c_str());
        }
        if(m_DisplayEntitySubType == EntitySubType::Warrior)
        {
            std::string text= "Lifepoint : " + std::to_string(WARRIOR_LIFEPOINT)+"\n"+"Manapoint : " + std::to_string(WARRIOR_MANAPOINT)+"\n"+"Attack : " + std::to_string(WARRIOR_ATTACKPOINT)+"\n"+"Defense : " + std::to_string(WARRIOR_DEFENSEPOINT)+"\n"+"Range : " + std::to_string(WARRIOR_RANGE);
            ImGui::TextWrapped(text.c_str());
        }  
        if(m_DisplayEntitySubType == EntitySubType::Rogue)
        {
            std::string text= "Lifepoint : " + std::to_string(ROGUE_LIFEPOINT)+"\n"+"Manapoint : " + std::to_string(ROGUE_MANAPOINT)+"\n"+"Attack : " + std::to_string(ROGUE_ATTACKPOINT)+"\n"+"Defense : " + std::to_string(ROGUE_DEFENSEPOINT)+"\n"+"Range : " + std::to_string(ROGUE_RANGE);
            ImGui::TextWrapped(text.c_str());
        }  
        if(m_DisplayEntitySubType == EntitySubType::Ranger)
        {
            std::string text= "Lifepoint : " + std::to_string(RANGER_LIFEPOINT)+"\n"+"Manapoint : " + std::to_string(RANGER_MANAPOINT)+"\n"+"Attack : " + std::to_string(RANGER_ATTACKPOINT)+"\n"+"Defense : " + std::to_string(RANGER_DEFENSEPOINT)+"\n"+"Range : " + std::to_string(RANGER_RANGE);
            ImGui::TextWrapped(text.c_str());
        }  
        if(m_DisplayEntitySubType == EntitySubType::Healer)
        {
            std::string text= "Lifepoint : " + std::to_string(HEALER_LIFEPOINT)+"\n"+"Manapoint : " + std::to_string(HEALER_MANAPOINT)+"\n"+"Attack : " + std::to_string(HEALER_ATTACKPOINT)+"\n"+"Defense : " + std::to_string(HEALER_DEFENSEPOINT)+"\n"+"Range : " + std::to_string(HEALER_RANGE);
            ImGui::TextWrapped(text.c_str());
        }          
        ImGui::SetWindowFontScale(charP[0]/290);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
    }

    void ConnectionScene::displayNextSubEntityTexture(int offset)
    {
        uint8_t nextIndex = static_cast<uint8_t>(m_DisplayEntitySubType);
        nextIndex += offset;

        if (nextIndex <= 0u)
        {
            nextIndex = 5;
        }

        if (nextIndex >= 6u)
        {
            nextIndex = 1;
        }

        m_DisplayEntitySubType = static_cast<EntitySubType>(nextIndex);
        gf::Texture *texture = ClientEntity::loadTexture(m_DisplayEntitySubType);
        m_PlayerWidget.setDefaultSprite(*texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        m_PlayerWidget.setDisabledSprite(*texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        m_PlayerWidget.setSelectedSprite(*texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
    }
}