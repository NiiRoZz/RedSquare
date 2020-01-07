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
#include <vector> 

namespace redsquare
{
    Hud::Hud(Game &game, gf::Font &font, gf::ExtendView &view)
    : m_Game(game)
    , m_Chat(font)
    , m_Inventory(font)
    , m_Font(font)
    , m_View(view)
    , m_ShowMap(false)
    {
        gMessageManager().registerHandler<SpellUpdateMessage>(&Hud::onSpellUpdate, this);

    }

    static constexpr float HudSpellSize = 55.0f;
    static constexpr float HudSpellTextureSize = 16.0f;
    static constexpr float HudMinimapSize = 3.0f;
    
    void Hud::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        m_Chat.render(target, states);
        m_Inventory.render(target, states);

        gf::Coordinates coordinates(target);

        //Draw MiniMap
        if (m_ShowMap)
        {
            gf::Vector2f baseCoordinatesMiniMap = coordinates.getRelativePoint({ 0.03f, 0.1f });
            gf::RectangleShape miniMapShape({HudMinimapSize,HudMinimapSize});
            gf::Vector2i posPlayer = m_Game.getMyPlayer()->m_Pos;

            for(int i = 0; i < World::MapSize; ++i)
            {
                for (int j = 0; j < World::MapSize; ++j)
                {  
                    bool draw = true;
                    Tile tileType = m_Game.m_World.m_World({i,j});
                    switch (tileType)
                    {
                        case Tile::Room:
                        case Tile::Stair:
                        case Tile::Corridor:
                        {
                            miniMapShape.setColor(gf::Color::Green);
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
                        if (i == posPlayer[0] && j == posPlayer[1])
                        {
                            miniMapShape.setColor(gf::Color::Red);
                        }
                        miniMapShape.setPosition({baseCoordinatesMiniMap[0] + i * HudMinimapSize, baseCoordinatesMiniMap[1] + j * HudMinimapSize});

                        target.draw( miniMapShape, states );
                    }
                }
            } 
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

        for (auto it = m_spellsTextures.begin(); it != m_spellsTextures.end(); ++it)
        {
            if (*it != nullptr)
            {
                gf::Sprite sprite;
                sprite.setTexture(**it);
                sprite.setAnchor(gf::Anchor::TopLeft);
                sprite.setPosition(coordinates.getRelativePoint({ 0.43f, 0.86f })+gf::Vector2f(x, y)+ HudSpellSize*gf::Vector2f(x, y)*coordinates.getRelativeSize({ 0.001f, 0.001f }).height);
                sprite.setScale((HudSpellSize / HudSpellTextureSize)*coordinates.getRelativeSize({ 0.001f, 0.001f }).height);
                target.draw(sprite, states);
                x += 1.2;

                if ( index % 4 == 0 )
                {
                    y += 1.2;
                    x = 0;
                }

                index++;
            }
        }
    }

    void Hud::update(gf::Time time)
    {
        m_Chat.update(time);
        m_Inventory.update(time);
    }

    void Hud::processEvent(const gf::Event &event)
    {
        m_Chat.processEvent(event);
        m_Inventory.processEvent(event);
    }

    bool Hud::hoveringChat()
    {
        return m_Chat.m_HoveringChat;
    }

    bool Hud::typingInChat()
    {
        return m_Chat.m_TypingInChat;
    }

    gf::Texture* Hud::getTextureFromSpellType(SpellType type)
    {
        return &gResourceManager().getTexture("img/redsquare.png");
    }

    gf::MessageStatus Hud::onSpellUpdate(gf::Id id, gf::Message *msg)
    {
        assert(id == SpellUpdateMessage::type);

        auto message = static_cast<SpellUpdateMessage*>(msg);
        
        m_spellsTextures.clear();

        for(auto it = message->spells.begin(); it != message->spells.end(); ++it)
        {
            if (*it != SpellType::Unknow)
            {
                m_spellsTextures.push_back(getTextureFromSpellType(*it));
            }
        }

        return gf::MessageStatus::Keep;
    }

    void Hud::showMap()
    {
        m_ShowMap = !m_ShowMap;
    }
}