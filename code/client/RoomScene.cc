#include "RoomScene.h"

#include <cinttypes>
#include <algorithm>

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>
#include <gf/Widgets.h>
#include <gf/Unused.h>

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
    , m_Chat(network)
    , m_Font(scenes.resources.getFont("font/arial.ttf"))
    , m_GaucheButton("<===", m_Font, 25)
    , m_DroiteButton("===>", m_Font, 25)
    , m_DisplayEntitySubType(EntitySubType::Unknow)
    , m_InfoText("", m_Font)
    {
        setClearColor(gf::Color::Black);

        m_GaucheButton.setCallback([this]() { displayNextSubEntityTexture(-1); });
        m_Container.addWidget(m_GaucheButton);

        m_DroiteButton.setCallback([this]() { displayNextSubEntityTexture(1); });
        m_Container.addWidget(m_DroiteButton);

        displayNextSubEntityTexture(1);
    }

    void RoomScene::startRoom()
    {
        m_Ready = false;
        m_DisplayEntitySubType = EntitySubType::Unknow;
        displayNextSubEntityTexture(1);
    }

    void RoomScene::doProcessEvent(gf::Event& event)
    {
        ImGui_ImplGF_ProcessEvent(event);

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

    void RoomScene::doUpdate(gf::Time time)
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

                case ServerChatMessage::type:
                {
                    gf::Log::debug("(ROOM) Receive ServerChatMessage\n");
                    auto data = bytes.as<ServerChatMessage>();
                    m_Chat.appendMessage(std::move(data.message));
                    break;
                }

                case ServerStartGame::type:
                {
                    gf::Log::debug("(ROOM) Receive ServerStartGame\n");
                    m_Scenes.game.initialize();
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

                    m_Chat.appendMessage(std::move(message));
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
        ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.8f));
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
                std::string subTypeName = entitySubTypeString(player.subType);
                ImGui::BulletText("%s - %s [%s]", player.name.c_str(), subTypeName.c_str(), player.ready ? "ready" : "not ready");
            }

            ImGui::EndGroup();

            ImGui::NextColumn();

            m_Chat.display(7);

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
                ImGui::ProgressBar(static_cast<float>(readyPlayers) / (m_Players.size()), DefaultProgressSize, "Waiting players...");
            }
        }
        ImGui::End();

        gf::Vector2f MainMenuWindowSizeDescription = coords.getRelativeSize({0.25f,0.25f });
        gf::Vector2f MainMenuWindowPosDescription = coords.getRelativePoint({ 0.025,0.65f });
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

        
        gf::Vector2f GauchePosButton = coords.getRelativePoint({ 0.35,0.65f });
        m_GaucheButton.setPosition({ GauchePosButton[0],GauchePosButton[1]});
        m_GaucheButton.setTextOutlineThickness(1.0f);
        m_GaucheButton.setAnchor(gf::Anchor::TopLeft);
        m_GaucheButton.setDefaultTextColor(gf::Color::White);
        m_GaucheButton.setDefaultTextOutlineColor(gf::Color::Black);
        m_GaucheButton.setSelectedTextOutlineColor(gf::Color::Black);
        target.draw(m_GaucheButton,states);
        
        gf::Vector2f startPlayerWidgetSize = coords.getRelativeSize({0.20f,0.27f });
        gf::Vector2f startPlayerWidgetPos = coords.getRelativePoint({ 0.40,0.67f });
        m_PlayerWidget.setPosition(startPlayerWidgetPos);
        m_PlayerWidget.setPosition(startPlayerWidgetPos);
        m_PlayerWidget.setScale(startPlayerWidgetSize/16.f);
        target.draw(m_PlayerWidget,states);

        float characterSize = coords.getRelativeCharacterSize(0.035f);
        
        m_InfoText.setCharacterSize(characterSize);
        m_InfoText.setColor(gf::Color::White);
        m_InfoText.setString( "Select a Character");
        m_InfoText.setPosition(coords.getRelativePoint({0.42f,0.67f}));
        m_InfoText.setOutlineThickness(characterSize * 0.04f);
        target.draw(m_InfoText, states);

        gf::Vector2f DroitePosButton = coords.getRelativePoint({ 0.60,0.65f });
        m_DroiteButton.setPosition({ DroitePosButton[0],DroitePosButton[1]});
        m_DroiteButton.setTextOutlineThickness(1.0f);
        m_DroiteButton.setAnchor(gf::Anchor::TopLeft);
        m_DroiteButton.setDefaultTextColor(gf::Color::White);
        m_DroiteButton.setDefaultTextOutlineColor(gf::Color::Black);
        m_DroiteButton.setSelectedTextOutlineColor(gf::Color::Black);
        target.draw(m_DroiteButton,states);

        gf::Vector2f MainMenuWindowSizeCharac = coords.getRelativeSize({0.25f,0.25f });
        gf::Vector2f MainMenuWindowPosCharac = coords.getRelativePoint({ 0.73,0.65f });
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
        
        // Display
        renderWorldEntities(target, states);
        renderHudEntities(target, states);

        ImGui::Render();
        ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
    }

    void RoomScene::displayNextSubEntityTexture(int offset)
    {
        uint8_t nextIndex = static_cast<uint8_t>(m_DisplayEntitySubType);
        nextIndex += offset;

        if (nextIndex == 0u)
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

        ClientSubType packet;
        packet.entitySubType = m_DisplayEntitySubType;
        m_Network.send(packet);
    }
}
