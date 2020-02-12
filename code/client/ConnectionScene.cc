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
    ,m_GaucheButton("<===", m_Font, 25)
    ,m_DroiteButton("===>", m_Font, 25)

    {
        setClearColor(gf::Color::Black);

        m_HostNameBuffer = "localhost";
        m_PortBuffer = "6000";
        m_NameBuffer = "toto";

        m_Container.addWidget(m_GaucheButton);
        m_Container.addWidget(m_DroiteButton);
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
                m_Scenes.replaceScene(m_Scenes.mainMenu, m_Scenes.glitchEffect, gf::seconds(0.8f));
            }

            ImGui::SameLine();

            if (ImGui::Button("Connect", DefaultButtonSize))
            {
                m_Scenes.game.connect(m_HostNameBuffer.getData(), m_PortBuffer.getData(), m_NameBuffer.getData());
                m_Scenes.replaceScene(m_Scenes.game, m_Scenes.glitchEffect, gf::seconds(1.0f));
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
        ImGui::TextWrapped("The logging API redirects all text output so you can easily capture the content of a window or a block. Tree nodes can be automatically expanded.");
        ImGui::SetWindowFontScale(descP[0]/300);

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
        gf::Texture playerTexture("../data/redsquare/img/Character/Magus.png");
        m_PlayerWidget.setDefaultSprite(playerTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        m_PlayerWidget.setDisabledSprite(playerTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        m_PlayerWidget.setSelectedSprite(playerTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        m_PlayerWidget.setPosition(startPlayerWidgetPos);
        m_PlayerWidget.setScale(startPlayerWidgetSize/playerTexture.getSize());
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
        ImGui::TextWrapped("The logging API redirects all text output so you can easily capture the content of a window or a block. Tree nodes can be automatically expanded.");
        ImGui::SetWindowFontScale(charP[0]/300);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
    }
}