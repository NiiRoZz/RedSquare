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
    ConnectionScene::ConnectionScene(Scenes& scenes, ClientNetwork& network)
    : gf::Scene(InitialSize)
    , m_Scenes(scenes)
    , m_Network(network)
    , m_Font(scenes.resources.getFont("font/arial.ttf"))
    , m_ConnectionAsked(false)
    {
        setClearColor(gf::Color::Black);

        m_HostNameBuffer = "localhost";
        m_NameBuffer = "toto";
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
        if (!isActive())
        {
            return;
        }
        
        ImGui_ImplGF_Update(time);
    }

    void ConnectionScene::doRender(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        if (!isActive())
        {
            return;
        }

        ImGui::NewFrame();

        gf::Coordinates coordinates(target);

        auto position = coordinates.getCenter();
        ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 1.1f));

        static const ImVec2 DefaultButtonSize = { 170.0f, 40.0f };

        if (ImGui::Begin("Connect", nullptr, DefaultWindowFlags)) {
            if (m_Network.isConnecting())
            {
                ImGui::Text("Connecting...");

                if (m_Network.isConnected())
                {
                    m_ConnectionAsked = false;
                    m_Scenes.replaceScene(m_Scenes.lobby, m_Scenes.glitchEffect, gf::seconds(0.4f));

                    ClientHello data;
                    data.name = m_NameBuffer.getData();
                    m_Network.send(data);
                }
            }
            else
            {
                ImGui::Text("Hostname:");
                ImGui::SameLine();
                float x = ImGui::GetCursorPosX();
                ImGui::InputText("###hostname", m_HostNameBuffer.getData(), m_HostNameBuffer.getSize());

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
                    m_Network.connect(m_HostNameBuffer.getData());
                    m_ConnectionAsked = true;
                }

                if (m_ConnectionAsked)
                {
                    ImGui::Text("Error: unable to connect to server.");
                }
            }
        }
        ImGui::End();

        // Display
        renderWorldEntities(target, states);
        renderHudEntities(target, states);

        ImGui::Render();
        ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
    }
}