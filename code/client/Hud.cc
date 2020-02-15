#include "Hud.h"
#include "Scenes.h"
#include "GameScene.h"
#include "../common/Singletons.h"
#include "ClientNetwork.h"
#include "../common/ImGuiConstants.h"

#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Color.h>
#include <gf/Text.h>
#include <gf/Particles.h>
#include <gf/Unused.h>
#include <vector>

namespace redsquare
{
    Hud::Hud(Scenes &scenes, GameScene &game, gf::Font &font, ClientNetwork &network)
    : m_Scenes(scenes)
    , m_Network(network)
    , m_Game(game)
    , m_Chat(network, game)
    , m_InventoryUI(font, game, network)
    , m_Font(font)
    , m_UI(font)
    , m_SpellWidgetHover(nullptr)
    , m_ShowMap(false)
    , m_ShowChat(true)
    , m_ShowHelp(false)
    , m_ShowInventory(false)
    , m_ShowEscape(false)
    , m_QuitWidget("Back to Menu", m_Font)
    , m_BackgroundTexture(gResourceManager().getTexture("img/Inventory/BorderSlot.png"))
    , m_Background(m_BackgroundTexture)
    {
        gMessageManager().registerHandler<SpellUpdateMessage>(&Hud::onSpellUpdate, this);
    }

    void Hud::initialize()
    {
        m_SpellWidgetHover = nullptr;
        m_ShowMap = false;
        m_ShowChat = false;
        m_ShowHelp = false;
        m_ShowInventory = false;
        m_ShowEscape = false;
        m_InventoryUI.initialize();
    }

    static constexpr float HudSpellSize = 55.0f;
    static constexpr float HudSpellTextureSize = 16.0f;

    void Hud::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Coordinates coordinates(target);

        if (m_Game.playerDead())
        {
            gf::Text text;
            text.setFont(m_Font);
            text.setColor(gf::Color::Red);
            text.setOutlineColor(gf::Color::Black);
            text.setOutlineThickness(coordinates.getRelativeSize({ 1.0f, 0.002f }).height);
            text.setCharacterSize(coordinates.getRelativeCharacterSize(0.18f));
            text.setString("YOU DIED");
            text.setAnchor(gf::Anchor::TopLeft);
            text.setPosition(coordinates.getRelativePoint({ 0.24f, 0.46f }));
            target.draw(text, states);

            text.setColor(gf::Color::Red);
            text.setOutlineColor(gf::Color::Black);
            text.setOutlineThickness(coordinates.getRelativeSize({ 1.0f, 0.002f }).height);
            text.setCharacterSize(coordinates.getRelativeCharacterSize(0.045f));
            text.setString("Floor : " + std::to_string(m_Game.getCurrentFloor()));
            text.setAnchor(gf::Anchor::TopLeft);
            text.setPosition(coordinates.getRelativePoint({ 0.03f, 0.05f }));
            target.draw(text, states);

            unsigned characterSize = coordinates.getRelativeCharacterSize(0.1f);
            auto startPosition = coordinates.getRelativePoint({ 0.5f, 0.7f });
            m_QuitWidget.setCharacterSize(characterSize);
            m_QuitWidget.setAnchor(gf::Anchor::Center);
            m_QuitWidget.setPosition(startPosition);
            m_QuitWidget.setDefaultTextColor(gf::Color::White);
            m_QuitWidget.setDefaultTextOutlineColor(gf::Color::Black);
            m_QuitWidget.setSelectedTextOutlineColor(gf::Color::Black);
            m_QuitWidget.setTextOutlineThickness(characterSize * 0.05f);
            target.draw(m_QuitWidget, states);
        }
        else
        {
            //Draw MiniMap
            if (m_ShowMap)
            {
                gf::Vector2f baseCoordinatesMiniMap = coordinates.getRelativePoint({ 0.03f, 0.1f });
                gf::Vector2f miniMapShapeSize = coordinates.getRelativeSize({ 0.001953125f, 0.003472222f });
                Player *myPlayer = m_Game.getEntities().getPlayer(m_Scenes.myPlayerId);
                gf::ShapeParticles shapeParticles;

                for(uint i = 0; i < World::MapSize; ++i)
                {
                    for (uint j = 0; j < World::MapSize; ++j)
                    {
                        bool draw = true;
                        Tile tileType = m_Game.m_World.m_World({i,j});
                        gf::Color4f color;

                        switch (tileType)
                        {
                            case Tile::Room:
                            case Tile::Stair:
                            case Tile::Corridor:
                            {
                                color = std::move(gf::Color4f(0.0078, 0.1765, 0.451, 0.75));
                                break;
                            }

                            default:
                            {
                                draw = false;
                                break;
                            }
                        }

                        if (draw)
                        {
                            Player *playerAtPos = m_Game.getEntities().getPlayer({(int)i,(int)j});
                            if (playerAtPos != nullptr)
                            {
                                if (playerAtPos != myPlayer)
                                {
                                    color = std::move(gf::Color4f(0.0, 1.0, 0.0, 0.75));
                                }
                                else
                                {
                                    color = std::move(gf::Color4f(1.0, 0.0, 0.0, 0.75));
                                }
                            }

                            shapeParticles.addRectangle( {baseCoordinatesMiniMap[0] + i * miniMapShapeSize[0], baseCoordinatesMiniMap[1] + j * miniMapShapeSize[1]}, miniMapShapeSize, color );
                        }
                    }
                }

                target.draw( shapeParticles, states );
            }

            //Draw floor
            gf::Text text;
            text.setFont(m_Font);
            text.setOutlineColor(gf::Color::White);
            text.setOutlineThickness(coordinates.getRelativeSize({ 1.0f, 0.002f }).height);
            text.setCharacterSize(coordinates.getRelativeCharacterSize(0.045f));
            text.setString("Floor : " + std::to_string(m_Game.getCurrentFloor()));
            text.setAnchor(gf::Anchor::TopLeft);
            text.setPosition(coordinates.getRelativePoint({ 0.03f, 0.05f }));
            target.draw(text, states);

            //Draw HforHelp
            text.setCharacterSize(coordinates.getRelativeCharacterSize(0.02f));
            text.setString("Press H for help menu");
            text.setAnchor(gf::Anchor::TopLeft);
            text.setPosition(coordinates.getRelativePoint({ 0.875f, 0.97f }));
            target.draw(text, states);

            //Draw it's your turn
            if (m_Game.canPlay())
            {
                gf::Text text;
                text.setFont(m_Font);
                text.setOutlineColor(gf::Color::White);
                text.setOutlineThickness(coordinates.getRelativeSize({ 1.0f, 0.002f }).height);
                text.setCharacterSize(coordinates.getRelativeCharacterSize(0.045f));
                text.setString("C'est ton tour !");
                text.setAnchor(gf::Anchor::TopLeft);
                text.setPosition(coordinates.getRelativePoint({ 0.80f, 0.05f }));
                target.draw(text, states);
            }

            float x = 0;
            float y = 0;
            uint index = 1;

            for (auto &it: m_SpellsWidgets)
            {
                gf::Vector2f pos = coordinates.getRelativePoint({ 0.43f, 0.86f })+gf::Vector2f(x, y)+ HudSpellSize*gf::Vector2f(x, y)*coordinates.getRelativeSize({ 0.001f, 0.001f }).height;
                float scale = (HudSpellSize / HudSpellTextureSize)*coordinates.getRelativeSize({ 0.001f, 0.001f }).height;
                it.setPosition(pos);
                it.setScale(scale);
                target.draw(it, states);
                x += 1.2;
                
                if( it.m_SpellType == m_Game.m_CurrentSpell )
                {
                    gf::Sprite sprite;
                    sprite.setTexture( gResourceManager().getTexture("img/SpellIcon/frame-9-red.png") );
                    sprite.setPosition(pos);
                    sprite.setScale(scale);
                    target.draw(sprite, states);
                }

                if ( index % 4 == 0 )
                {
                    y += 1.2;
                    x = 0;
                }

                index++;
            }

            if (m_SpellWidgetHover != nullptr && !m_ShowHelp)
            {
                std::string desc = m_SpellWidgetHover->m_Description;
                std::string cost = m_SpellWidgetHover->m_ManaCost;

                gf::Vector2f SpellDescriptionWindowSize = coordinates.getRelativeSize({0.4f,0.2f });
                gf::Vector2f SpellDescriptionWindowPos = coordinates.getRelativePoint({ 0.3f,0.6f });
                ImGui::SetNextWindowSize(ImVec2(SpellDescriptionWindowSize[0], SpellDescriptionWindowSize[1]));
                ImGui::SetNextWindowPos(ImVec2(SpellDescriptionWindowPos[0], SpellDescriptionWindowPos[1]));

                ImGui::Begin("Spell desciption", nullptr, DefaultWindowFlags);
                ImVec2 charP=ImGui::GetWindowSize();
                std::string text= "! Spell need to be selected before using it ! | Mana cost: " + cost + "\n\n" + desc ;
                ImGui::TextWrapped(text.c_str());
                ImGui::SetWindowFontScale(charP[0]/477);
                ImGui::End();

                ImGui::Render();
                ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
            }

            if (m_ShowChat)
            {
                m_Chat.display(coordinates);
            }

            if (m_ShowHelp) 
            {
                gf::Vector2f DescriptionWindowSize = coordinates.getRelativeSize({0.4f,0.3f });
                gf::Vector2f DescriptionWindowPos = coordinates.getRelativePoint({ 0.6f,0.6f });
                ImGui::SetNextWindowSize(ImVec2(DescriptionWindowSize[0], DescriptionWindowSize[1]));
                ImGui::SetNextWindowPos(ImVec2(DescriptionWindowPos[0], DescriptionWindowPos[1]));

                ImGui::Begin("Help Menu", nullptr, DefaultWindowFlags);
                ImVec2 charP=ImGui::GetWindowSize();
                std::string text= "Escape -> Echap menu \n C -> Hide/Chat \n I -> Inventory/Hide \n F -> Fullscreen \n M -> Map/Hide \n Spell description : pass your mouse hover spells icons \n Spell selection : click on it or press number touch ";
                ImGui::TextWrapped(text.c_str());
                ImGui::SetWindowFontScale(charP[0]/470);
                ImGui::End();

                ImGui::Render();
                ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
            }

            if (m_ShowInventory)
            {
                m_InventoryUI.render(target, states);
            }

            if(m_ShowEscape)
            {
                gf::Vector2f InventoryWindowSize = coordinates.getRelativeSize({ 1.0f, 1.0f });
                m_Background.setScale({InventoryWindowSize[0],InventoryWindowSize[1]});
                
                unsigned characterSize = coordinates.getRelativeCharacterSize(0.095f);
                auto startPosition = coordinates.getRelativePoint({ 0.5f, 0.5f });
                m_QuitWidget.setCharacterSize(characterSize);
                m_QuitWidget.setAnchor(gf::Anchor::Center);
                m_QuitWidget.setPosition(startPosition);
                m_QuitWidget.setDefaultTextColor(gf::Color::White);
                m_QuitWidget.setDefaultTextOutlineColor(gf::Color::Black);
                m_QuitWidget.setSelectedTextOutlineColor(gf::Color::Black);
                m_QuitWidget.setTextOutlineThickness(characterSize * 0.05f);

                target.draw( m_Background, states );
                target.draw( m_QuitWidget, states );
            }
        }
    }

    void Hud::update(gf::Time time)
    {
        if (m_ShowInventory)
        {
            m_InventoryUI.update(time);
        }
    }

    void Hud::processEvent(const gf::Event &event)
    {
        if (m_ShowInventory)
        {
            m_InventoryUI.processEvent(event);
        }

        if (event.type == gf::EventType::MouseMoved)
        {
            bool found = false;
            for(auto &it: m_SpellsWidgets)
            {
                if (it.contains(event.mouseCursor.coords))
                {
                    m_SpellWidgetHover = &it;
                    m_MouseHoverPostionOnSpell = event.mouseCursor.coords;
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                m_SpellWidgetHover = nullptr;
                m_MouseHoverPostionOnSpell = {0,0};
            }

            if ((m_ShowEscape || m_Game.playerDead()) && m_QuitWidget.contains(event.mouseCursor.coords))
            {
                m_QuitWidget.setState(gf::WidgetState::Selected);
            }
            else if ((m_ShowEscape || m_Game.playerDead()))
            {
                m_QuitWidget.setState(gf::WidgetState::Default);
            }
        }
        else if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left)
        {
            if ((m_ShowEscape || m_Game.playerDead()) && m_QuitWidget.contains(event.mouseButton.coords))
            {
                m_ShowEscape = false;
                if (m_Network.isConnected())
                {
                    m_Network.disconnect();
                }
                m_Scenes.replaceScene(m_Scenes.mainMenu, m_Scenes.glitchEffect, gf::seconds(0.4f));
            }

            int index = 1;
            for(auto &it: m_SpellsWidgets)
            {
                if (it.contains(event.mouseButton.coords))
                {
                    m_Game.changeSpell(index);
                    break;
                }

                index++;
            }
        }
    }

    bool Hud::hoveringChat() const
    {
        return m_Chat.hoveringChat();
    }

    bool Hud::shownInventory() const
    {
        return m_ShowInventory;
    }

    gf::MessageStatus Hud::onSpellUpdate(gf::Id id, gf::Message *msg)
    {
        assert(id == SpellUpdateMessage::type);

        auto message = static_cast<SpellUpdateMessage*>(msg);

        if (m_SpellWidgetHover != nullptr)
        {
            m_SpellWidgetHover = nullptr;
            m_MouseHoverPostionOnSpell = {0,0};
        }

        m_SpellsWidgets.clear();

        for(auto it = message->spells.begin(); it != message->spells.end(); ++it)
        {
            if (*it != SpellType::Unknow)
            {
                m_SpellsWidgets.push_back(std::move(redsquare::SpellWidget(*it)));
            }
        }

        return gf::MessageStatus::Keep;
    }

    void Hud::showMap()
    {
        m_ShowMap = !m_ShowMap;
    }

    void Hud::showChat()
    {
        m_ShowChat = !m_ShowChat;
    }

    void Hud::showHelp()
    {
        m_ShowHelp = !m_ShowHelp;
    }

    void Hud::showInventory()
    {
        m_ShowInventory = !m_ShowInventory;

        if (!m_ShowInventory)
        {
            m_InventoryUI.setVinicityObject(nullptr);
            m_InventoryUI.inventoryHid();
        }
    }

    void Hud::showEscape()
    {
        m_ShowEscape = !m_ShowEscape;
    }

    bool Hud::escapeOpen() const
    {
        return m_ShowEscape;
    }

    InventoryUI& Hud::getInventoryUI()
    {
        return m_InventoryUI;
    }

    bool Hud::hoveringSpellWidgets() const
    {
        return (m_SpellWidgetHover != nullptr);
    }

    GameChat& Hud::getChat()
    {
        return m_Chat;
    }
}
