#include "RoomScene.h"

#include <cinttypes>
#include <algorithm>

#include <gf/Coordinates.h>
#include <gf/Log.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "../common/Constants.h"
#include "../common/ImGuiConstants.h"
#include "../common/Protocol.h"

#include "Scenes.h"

namespace redsquare 
{
    RoomScene::RoomScene(Scenes& scenes, ClientNetwork& network)
    : gf::Scene(InitialSize)
    , m_Scenes(scenes)
    , m_Network(network)
    {
        setClearColor(gf::Color::Black);
    }

    void RoomScene::startRoom()
    {
        m_Ready = false;
    }

    void RoomScene::doProcessEvent(gf::Event& event)
    {
        ImGui_ImplGF_ProcessEvent(event);
    }

    void RoomScene::doUpdate(gf::Time time)
    {
        if (!isActive())
        {
            return;
        }

        ImGui_ImplGF_Update(time);

        ProtocolBytes bytes;

        while (m_Network.queue.poll(bytes))
        {
            switch (bytes.getType())
            {
                case ServerLeaveRoom::type:
                {
                    gf::Log::debug("(ROOM) Receive ServerLeaveRoom\n");
                    m_Scenes.replaceScene(m_Scenes.lobby, m_Scenes.glitchEffect, gf::seconds(0.4f));
                    // do not poll any more message as the next messages are for the lobby
                    return;
                }

                case ServerReady::type:
                {
                    gf::Log::debug("(ROOM) Receive ServerReady\n");
                    auto data = bytes.as<ServerReady>();
                    m_Ready = data.ready;
                    break;
                }

                case ServerListRoomPlayers::type:
                {
                    gf::Log::debug("(ROOM) Receive ServerListRoomPlayers\n");
                    auto data = bytes.as<ServerListRoomPlayers>();
                    m_Players = std::move(data.players);
                    break;
                }

                /*case ServerChatMessage::type:
                {
                    gf::Log::debug("(ROOM) Receive ServerChatMessage\n");
                    auto data = bytes.as<ServerChatMessage>();
                    m_chat.appendMessage(std::move(data.message));
                    break;
                }
                */

                case ServerStartGame::type:
                {
                    gf::Log::debug("(ROOM) Receive ServerStartGame\n");
                    m_Scenes.replaceScene(m_Scenes.game, m_Scenes.glitchEffect, gf::seconds(0.4f));
                    // do not poll any more message as the next messages are for the game
                    return;
                }

                case ServerError::type:
                {
                    gf::Log::debug("(ROOM) Receive ServerError\n");
                    auto data = bytes.as<ServerError>();
                    MessageData message;
                    message.origin = gf::InvalidId;
                    message.author = "server";
                    message.content = serverErrorString(data.reason);

                    //m_chat.appendMessage(std::move(message));
                    break;
                }
            }
        }
    }

    void RoomScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states)
    {
        gf::Coordinates coords(target);
        auto position = coords.getCenter();

        // UI
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(1000.0f, 0.0f));

        if (ImGui::Begin("Room", nullptr, DefaultWindowFlags))
        {
            ImGui::Text("Players required: %" PRIi32, MIN_PLAYERS);
            ImGui::Separator();

            ImGui::Columns(2);

            ImGui::BeginGroup();
            ImGui::Text("Players");
            ImGui::Spacing();

            for (auto& player : m_Players)
            {
                ImGui::BulletText("%s [%s]", player.name.c_str(), player.ready ? "ready" : "not ready");
            }

            ImGui::EndGroup();

            ImGui::NextColumn();

            //m_chat.display(10);

            ImGui::Columns();

            ImGui::Spacing();

            if (!m_Ready)
            {
                ImGui::Indent();

                if (ImGui::Button("Leave room", DefaultButtonSize))
                {
                    ClientLeaveRoom data;
                    m_Network.send(data);
                }

                ImGui::SameLine();

                if (ImGui::Button("Ready", DefaultButtonSize))
                {
                    ClientReady data;
                    data.ready = true;
                    m_Network.send(data);
                }
            }
            else
            {
                int32_t readyPlayers = std::count_if(m_Players.begin(), m_Players.end(), [](auto& player) { return player.ready; });
                ImGui::PushItemWidth(-1.0f);
                //ImGui::ProgressBar(static_cast<float>(readyPlayers) / (m_settings.teams * m_settings.playersByTeam), DefaultProgressSize, "Waiting players...");
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
