#include "LobbyChat.h"
#include "ClientNetwork.h"
#include "../common/Protocol.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Unused.h>
#include <vector>

namespace redsquare
{
    ImVec4 LobbyChat::toColor(gf::Id id)
    {
        gf::Color4f color;

        if (id == gf::InvalidId) // Server message
        { 
            color = gf::Color::lighter(gf::Color::Red);
        }
        else
        {
            color = gf::Color::lighter(gf::Color::fromRgba32(static_cast<uint32_t>(id)));
        }

        return ImVec4(color.r, color.g, color.b, 1.0f);
    }
    
    LobbyChat::LobbyChat(ClientNetwork& network)
    : m_Network(network)
    , m_AutoScroll(false)
    {
        m_LineBuffer.clear();
    }

    void LobbyChat::appendMessage(const MessageData& message)
    {
        m_Messages.push_back(message);
        m_AutoScroll = true;
    }

    void LobbyChat::appendMessage(MessageData&& message)
    {
        m_Messages.push_back(std::move(message));
        m_AutoScroll = true;
    }

    void LobbyChat::display(int lines)
    {
        ImGui::Text("Chat");
        ImGui::Spacing();
        ImVec2 size(0.0f, lines * ImGui::GetTextLineHeightWithSpacing());

        if (ImGui::BeginChild("Messages", size, false))
        {
            for (auto& message : m_Messages)
            {
                std::string str = "[" + message.author + "] ";
                ImGui::TextColored(toColor(message.origin), str.c_str());
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

        ImGui::PushItemWidth(-1.0f);

        if (ImGui::InputText("###chat", m_LineBuffer.getData(), m_LineBuffer.getSize(), ImGuiInputTextFlags_EnterReturnsTrue) && m_LineBuffer[0] != '\0')
        {
            ClientChatMessage data;
            data.content = m_LineBuffer.getData();
            m_Network.send(data);
            m_LineBuffer.clear();
            ImGui::SetKeyboardFocusHere(-1);
        }
    }
}
