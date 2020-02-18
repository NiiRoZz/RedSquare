#include "LobbyScene.h"

#include <cinttypes>

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Unused.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "Scenes.h"
#include "../common/Constants.h"
#include "../common/ImGuiConstants.h"
#include "../common/Protocol.h"

namespace redsquare
{
    LobbyScene::LobbyScene(Scenes& scenes, ClientNetwork& network)
    : gf::Scene(InitialSize)
    , m_Scenes(scenes)
    , m_Network(network)
    , m_Chat(network)
    , m_SelectedRoom(-1)
    , m_SelectedNmbPlayers(1)
    {
        setClearColor(gf::Color::Black);

        m_RoomBuffer.clear();
        m_NameBuffer.clear();
    }

    void LobbyScene::doProcessEvent(gf::Event& event)
    {
        ImGui_ImplGF_ProcessEvent(event);
    }

    void LobbyScene::doUpdate(gf::Time time)
    {
        if (!isActive())
        {
            return;
        }

        ImGui_ImplGF_Update(time);

        gf::Packet bytes;

        while (m_Network.queue.poll(bytes))
        {
            switch (bytes.getType())
            {
                case ServerHello::type:
                {
                    gf::Log::debug("(LOBBY) Receive ServerHello\n");
                    auto data = bytes.as<ServerHello>();
                    m_Scenes.myPlayerId = data.playerId;
                    break;
                }

                case ServerDisconnect::type:
                {
                    gf::Log::debug("(LOBBY) Receive ServerDisconnect\n");
                    m_Network.disconnect();
                    m_Scenes.replaceScene(m_Scenes.connection, m_Scenes.glitchEffect, gf::seconds(0.4f));
                    break;
                }
                    
                case ServerListPlayers::type:
                {
                    gf::Log::debug("(LOBBY) Receive ServerListPlayers\n");
                    auto data = bytes.as<ServerListPlayers>();
                    m_Players = std::move(data.players);

                    m_PlayersView.clear();
                    for (auto& player: m_Players)
                    {
                        m_PlayersView.push_back(player.name.c_str());
                    }
                    break;
                }

                case ServerListRooms::type:
                {
                    gf::Log::debug("(LOBBY) Receive ServerListRooms\n");
                    auto data = bytes.as<ServerListRooms>();
                    m_Rooms = std::move(data.rooms);

                    int32_t index = 0;
                    m_SelectedRoom = -1;
                    m_RoomsView.clear();
                    for (auto& room : m_Rooms)
                    {
                        m_RoomsView.push_back(room.name.c_str());
                        if (room.id == m_SelectedRoomData.id)
                        {
                            m_SelectedRoom = index;
                            m_SelectedRoomData = m_Rooms[m_SelectedRoom];
                        }
                        ++index;
                    }
                    break;
                }

                case ServerJoinRoom::type:
                {
                    gf::Log::debug("(LOBBY) Receive ServerJoinRoom\n");
                    //auto data = bytes.as<ServerJoinRoom>();
                    m_Scenes.room.startRoom();
                    m_Scenes.replaceScene(m_Scenes.room, m_Scenes.glitchEffect, gf::seconds(0.4f));
                    // do not poll any more message as the next messages are for the room
                    return;
                }

                case ServerChatMessage::type:
                {
                    gf::Log::debug("(LOBBY) Receive ServerChatMessage\n");
                    auto data = bytes.as<ServerChatMessage>();
                    m_Chat.appendMessage(std::move(data.message));
                    break;
                }

                case ServerError::type:
                {
                    gf::Log::debug("(LOBBY) Receive ServerError\n");
                    auto data = bytes.as<ServerError>();
                    MessageData message;
                    message.origin = gf::InvalidId;
                    message.author = "server";
                    message.content = serverErrorString(data.reason);

                    m_Chat.appendMessage(std::move(message));
                    break;
                }
            }
        }
    }

    void LobbyScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states)
    {
        gf::Coordinates coords(target);
        auto position = coords.getCenter();

        // UI
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(1000.0f, 0.0f));
        ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.5f));

        if (ImGui::Begin("Lobby", nullptr, DefaultWindowFlags))
        {
            ImGui::Columns(3);

            ImGui::BeginGroup();
            ImGui::Text("Players");
            ImGui::Spacing();
            int selectedPlayer = -1;
            ImGui::PushItemWidth(-1.0f);
            ImGui::ListBox("##players", &selectedPlayer, m_PlayersView.data(), m_PlayersView.size(), 10);
            ImGui::EndGroup();

            ImGui::NextColumn();

            ImGui::BeginGroup();
            ImGui::Text("Join a room");
            ImGui::Spacing();
            ImGui::PushItemWidth(-1.0f);


            if (ImGui::ListBox("###rooms", &m_SelectedRoom, m_RoomsView.data(), m_RoomsView.size(), 10))
            {
                assert(m_SelectedRoom != -1);
                m_SelectedRoomData = m_Rooms[m_SelectedRoom];
            }

            ImGui::PopItemWidth();
            ImGui::Spacing();
            ImGui::Indent();

            if (ImGui::Button("Join", DefaultButtonSize) && m_SelectedRoom != -1)
            {
                ClientJoinRoom data;
                data.room = m_Rooms[m_SelectedRoom].id;
                m_Network.send(data);
            }

            ImGui::EndGroup();

            ImGui::NextColumn();

            ImGui::BeginGroup();
            m_Chat.display(10);
            ImGui::EndGroup();

            ImGui::NextColumn();
            ImGui::Separator(); // second line

            ImGui::BeginGroup();
            ImGui::EndGroup();

            ImGui::NextColumn();

            ImGui::BeginGroup();
            ImGui::Text("Room info");

            if (m_SelectedRoom != -1)
            {
                ImGui::Indent();
                ImGui::Text("Name: %s", m_SelectedRoomData.name.c_str());
                ImGui::Text("Players: %" PRIi32 "/%" PRIi32, m_SelectedRoomData.players, MAX_PLAYERS);
            }

            ImGui::EndGroup();

            ImGui::NextColumn();

            ImGui::BeginGroup();
            ImGui::Text("Create room");
            ImGui::Spacing();
            ImGui::Text("Name:");
            ImGui::SameLine();
            ImGui::InputText("###room_name", m_RoomBuffer.getData(), m_RoomBuffer.getSize());

            ImGui::Spacing();
            ImGui::Indent();

            if (ImGui::Button("Create room", DefaultButtonSize) && m_RoomBuffer[0] != '\0') 
            {
                ClientCreateRoom data;
                data.name = m_RoomBuffer.getData();
                m_Network.send(data);
                m_RoomBuffer.clear();
            }

            ImGui::EndGroup();

            ImGui::Columns();
            ImGui::Separator();

            ImGui::Spacing();
            ImGui::Indent();

            if (ImGui::Button("Back", DefaultButtonSize))
            {
                m_Scenes.replaceScene(m_Scenes.connection, m_Scenes.glitchEffect, gf::seconds(0.4f));
                m_Network.disconnect();
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
