#include "Hud.h"
#include "../common/Singletons.h"
#include "Game.h"

#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Color.h>
#include <gf/Text.h>
#include <gf/Particles.h>
#include <vector>

namespace redsquare
{
    Hud::Hud(Game &game, gf::Font &font,char* port,char* hostname, gf::ExtendView &view,const char* name)
    : m_Game(game)
    , m_Chat(font,port,hostname,name)
    , m_Inventory(font, game)
    , m_MainMenu(font)
    , m_Font(font)
    , m_View(view)
    , m_SpellWidgetHover(nullptr)
    , m_ShowMap(false)
    , m_HideChat(true)
    , m_ShowHelp(false)
    , m_UI(font) 
    {
        gMessageManager().registerHandler<SpellUpdateMessage>(&Hud::onSpellUpdate, this);

    }

    static constexpr float HudSpellSize = 55.0f;
    static constexpr float HudSpellTextureSize = 16.0f;

    void Hud::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Coordinates coordinates(target);

        //Draw MiniMap
        if (m_ShowMap)
        {
            gf::Vector2f baseCoordinatesMiniMap = coordinates.getRelativePoint({ 0.03f, 0.1f });
            gf::Vector2f miniMapShapeSize = coordinates.getRelativeSize({ 0.001953125f, 0.003472222f });
            Player *myPlayer = m_Game.getMyPlayer();
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
                        Player *playerAtPos = m_Game.getPlayer({(int)i,(int)j});
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
        text.setCharacterSize(coordinates.getRelativeCharacterSize(0.05f));
        text.setString("Etage : " + std::to_string(m_Game.m_Floor));
        text.setAnchor(gf::Anchor::TopLeft);
        text.setPosition(coordinates.getRelativePoint({ 0.03f, 0.05f }));
        target.draw(text, states);

        //Draw it's your turn
        if (m_Game.m_CanPlay)
        {
            gf::Text text;
            text.setFont(m_Font);
            text.setOutlineColor(gf::Color::White);
            text.setOutlineThickness(coordinates.getRelativeSize({ 1.0f, 0.002f }).height);
            text.setCharacterSize(coordinates.getRelativeCharacterSize(0.05f));
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
            
            if(it.m_SpellType == m_Game.m_CurrentSpell)
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

        if (m_SpellWidgetHover != nullptr)
        {
            gf::Coordinates coordinates(target);
            gf::Vector2f DescriptionWindowSize=coordinates.getRelativeSize({ 0.4f,0.3f });
            

            std::string desc = m_SpellWidgetHover->m_Description;
            std::cout << m_MouseHoverPostionOnSpell[0] << " " << m_MouseHoverPostionOnSpell[1] << " " << desc << std::endl;

            if( m_UI.begin("Description", gf::RectF::fromPositionSize(coordinates.getRelativePoint({ 0.40f,0.4f }),DescriptionWindowSize), gf::UIWindow::Title|gf::UIWindow::NoScrollbar))
            {   
                m_UI.layoutRowDynamic(15, 1);
                m_UI.label("Spell need to be selected before using it");
                m_UI.layoutRowDynamic(15, 2);
                m_UI.label("Shortcuts");
                m_UI.label(" 1 2 3 4 ");
                m_UI.layoutRowDynamic(5, 2);
                m_UI.label("");
                m_UI.label(" 5 6 7 8 ");
                m_UI.layoutRowDynamic(30, 1);
                m_UI.label(desc);
                m_UI.end();
            }
            target.draw(m_UI);
        }

        if (m_HideChat)
        {
            m_Chat.render(target, states);
        }
        if (m_ShowHelp) 
        {
            
            gf::Coordinates coordinates(target);
            gf::Vector2f DescriptionWindowSize=coordinates.getRelativeSize({ 0.4f,0.3f });
            if( m_UI.begin("Help", gf::RectF::fromPositionSize(coordinates.getRelativePoint({ 0.60f,0.6f }),DescriptionWindowSize), gf::UIWindow::Title|gf::UIWindow::NoScrollbar))
            {   
                m_UI.setCharacterSize(12);
                m_UI.layoutRowDynamic(13, 1);
                m_UI.label("Echap -> Close the game");
                m_UI.label("C -> Hide/Chat");
                m_UI.label("I -> Inventory/Hide");
                m_UI.label("F -> Fullscreen");
                m_UI.label("M -> Map/Hide");
                m_UI.label("Spell description : pass your mouse hover spells icons");
                m_UI.layoutRowDynamic(5, 2);
                m_UI.label("Spell Shortcuts");
                m_UI.label(" 1 2 3 4 ");
                m_UI.layoutRowDynamic(8, 2);
                m_UI.label("");
                m_UI.label(" 5 6 7 8 ");
                m_UI.end();
            }
            target.draw(m_UI);
        }

        m_Inventory.render(target, states);
        m_MainMenu.render(target,states);
    }

    void Hud::update(gf::Time time)
    {
        if (m_HideChat)
        {
            m_Chat.update(time);
        }

        m_Inventory.update(time);
        m_MainMenu.update(time);
    }

    void Hud::processEvent(const gf::Event &event)
    {
        if (m_HideChat)
        {
            m_Chat.processEvent(event);
        }

        m_Inventory.processEvent(event);
        m_MainMenu.processEvent(event);

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
        }
    }

    bool Hud::hoveringChat()
    {
        return m_Chat.m_HoveringChat;
    }

    bool Hud::typingInChat()
    {
        return m_Chat.m_TypingInChat;
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
                m_SpellsWidgets.emplace_back(std::move(redsquare::SpellWidget(*it)));
            }
        }

        return gf::MessageStatus::Keep;
    }

    void Hud::showMap()
    {
        m_ShowMap = !m_ShowMap;
    }

    void Hud::hideChat()
    {
        m_HideChat = !m_HideChat;
    }

    void Hud::showHelp()
    {
        m_ShowHelp = !m_ShowHelp;
    }
}
