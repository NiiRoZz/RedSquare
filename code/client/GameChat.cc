#include "GameChat.h"

#include <gf/Color.h>
#include <gf/Log.h>

#include <imgui.h>

#include "GameScene.h"
#include "../common/Protocol.h"
#include "../common/ImGuiConstants.h"

namespace redsquare
{

    GameChat::GameChat(ClientNetwork& network, GameScene &game)
    : LobbyChat(network)
    , m_Game(game)
    , m_SelectedUserName("")
    , m_SelectedUserId(gf::InvalidId)
    , m_HoveringChat(false)
    {
    }

    bool GameChat::hoveringChat() const
    {
        return m_HoveringChat;
    }

    void GameChat::display(gf::Coordinates& coordinates)
    {
        // Chat window
        gf::Vector2f chatWindowSize = coordinates.getRelativeSize({ 0.35f, 0.46f });
        gf::Vector2f chatWindowPos = coordinates.getRelativePoint({ 0.82f, 0.76f });

        ImGui::SetNextWindowSize(ImVec2(chatWindowSize[0], chatWindowSize[1]));
        ImGui::SetNextWindowPos(ImVec2(chatWindowPos[0], chatWindowPos[1]), 0, ImVec2(0.5f, 0.5f));

        if (ImGui::Begin("Chat", nullptr, DefaultWindowFlags))
        {
            m_HoveringChat = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

            ImGui::Columns(2);

            ImGui::BeginGroup();
            ImGui::Text("Messages");
            ImGui::Spacing();
            ImVec2 size(0.0f, ImGui::GetWindowHeight() - 4.5f * ImGui::GetTextLineHeightWithSpacing());

            if (ImGui::BeginChild("Messages", size, false))
            {
                for (auto& message : m_Messages)
                {
                    std::string str;
                    if (message.recipient != gf::InvalidId)
                    {
                        str = "<" + message.author + "> ";
                    }
                    else
                    {
                        str = "[" + message.author + "] ";
                    }

                    ImGui::TextColored(toColor(message.origin), str.c_str());

                    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && message.origin != gf::InvalidId)
                    {
                        m_SelectedUserName = "@" + message.author;
                        m_SelectedUserId = message.origin;
                        m_LineBuffer.clear();
                    }

                    ImGui::SameLine();

                    if (message.origin == gf::InvalidId)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, toColor(message.origin));
                        ImGui::TextWrapped(message.content.c_str());
                        ImGui::PopStyleColor();
                    }
                    else
                    {
                        ImGui::TextWrapped(message.content.c_str());
                    }
                }
            }

            if (m_AutoScroll)
            {
                ImGui::SetScrollHereY(1.0f); // bottom
                m_AutoScroll = false;
            }

            ImGui::EndChild();

            ImGui::Spacing();

            //Detect if we are currently typing for a player
            if (m_SelectedUserId != gf::InvalidId)
            {
                ImGui::TextColored(toColor(m_SelectedUserId), m_SelectedUserName.c_str());
                ImGui::SameLine();
            }

            //Fix InputText not resized correctly
            ImGui::PushItemWidth(-1.0f);

            size_t oldSize = strlen(m_LineBuffer.getData());

            if (ImGui::InputText("###chat", m_LineBuffer.getData(), m_LineBuffer.getSize(), ImGuiInputTextFlags_EnterReturnsTrue) && m_LineBuffer[0] != '\0') {
                ClientChatMessage data;
                data.content = m_LineBuffer.getData();

                if (m_SelectedUserId != gf::InvalidId)
                {
                    data.recipient = m_SelectedUserId;
                    m_SelectedUserId = gf::InvalidId;
                    m_SelectedUserName = "";
                }

                m_Network.send(data);
                m_LineBuffer.clear();
                ImGui::SetKeyboardFocusHere(-1);
            }

            if (ImGui::IsItemActive() && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Backspace)) && oldSize == 0)
            {
                m_SelectedUserId = gf::InvalidId;
                m_SelectedUserName = "";
            }

            ImGui::EndGroup();

            ImGui::NextColumn();

            ImGui::BeginGroup();
            ImGui::Text("Players");
            ImGui::Spacing();

            size = ImVec2(0.0f, ImGui::GetWindowHeight() - 4.5f * ImGui::GetTextLineHeightWithSpacing());

            if (ImGui::BeginChild("Players", size, false))
            {
                // List players
                auto &players = m_Game.getPlayersData();
                for (auto &player: players)
                {
                    ImGui::TextColored(toColor(player.id), player.name.c_str());

                    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        m_SelectedUserName = "@" + player.name;
                        m_SelectedUserId = player.id;
                        m_LineBuffer.clear();
                    }
                }
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }
        ImGui::End();
    }
}
