#include "Chat.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <vector>

namespace redsquare
{
    Chat::Chat(gf::Font &font, char *port, char *hostname,const char* name)
    : m_HoveringChat(false)
    , m_UI(font)
    , m_Name(name)
    , m_ChatCom(hostname, port, m_ChatQueue)
    , m_AutoScroll(false)
    {
        m_ChatCom.start();
        m_LineBuffer.clear();
    }

    void Chat::update(gf::Time time)
    {
        Message packet;
        while(m_ChatQueue.poll(packet))
        {
            m_ChatMessageBuffer.push_back(std::move(packet));
            m_AutoScroll = true;
        }
    }

    void Chat::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {   
        gf::Coordinates coordinates(target);

        gf::Vector2f ChatWindowSize = coordinates.getRelativeSize({ 0.22f,0.40f });
        gf::Vector2f ChatWindowPos = coordinates.getRelativePoint({ 0.12f,0.80f });
        auto position = coordinates.getCenter();

        // UI
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(ChatWindowSize[0], ChatWindowSize[1]));
        ImGui::SetNextWindowPos(ImVec2(ChatWindowPos[0], ChatWindowPos[1]), 0, ImVec2(0.5f, 0.5f));

        if (ImGui::Begin("Chat", nullptr, DefaultWindowFlags | ImGuiWindowFlags_NoScrollbar))
        {
            m_HoveringChat = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

            ImGui::BeginGroup();

            ImVec2 size(0.0f, ChatWindowSize[1] - 3.5f * ImGui::GetTextLineHeightWithSpacing());

            if (ImGui::BeginChild("Messages", size, false))
            {
                for (auto& message : m_ChatMessageBuffer)
                {
                    std::string strFrom = message.from;
                    strFrom.insert(strFrom.length(), std::string(" :"));

                    ImGui::TextColored(ImVec4(1,0,0,1), strFrom.c_str());
                    ImGui::SameLine();
                    ImGui::TextWrapped(message.message);
                }
            }

            if (m_AutoScroll)
            {
                ImGui::SetScrollHereY(1.0f); // bottom
                m_AutoScroll = false;
            }

            ImGui::EndChild();

            ImGui::Spacing();
            ImGui::SetNextItemWidth(ImGui::GetWindowWidth());

            if (ImGui::InputText("###chat", m_LineBuffer.getData(), m_LineBuffer.getSize(), ImGuiInputTextFlags_EnterReturnsTrue) && m_LineBuffer[0] != '\0')
            {
                Message sendPacket;

                std::size_t length = m_Name.copy(sendPacket.from, m_Name.length());
                sendPacket.from[length]='\0';

                std::string message = m_LineBuffer.getData();
                length = message.copy(sendPacket.message, message.length());
                sendPacket.message[length]='\0';

                m_ChatCom.sendPacket(sendPacket);

                m_LineBuffer.clear();
                ImGui::SetKeyboardFocusHere(-1);
            }

            ImGui::EndGroup();
        }

        ImGui::End();
    }

    void Chat::processEvent(const gf::Event &event)
    {
        m_UI.processEvent(event);
    }
}
