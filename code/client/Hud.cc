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
    Hud::Hud(Game &game, gf::Font &font, gf::ExtendView &view)
    : m_Game(game)
    , m_Chat(font)
    , m_Inventory(font, game)
    , m_Font(font)
    , m_View(view)
    , m_ShowMap(false)
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

        m_Chat.render(target, states);
        m_Inventory.render(target, states);
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
        std::string texture;

        switch (type)
        {
        case SpellType::FireBall :
            texture = "img/SpellIcon/Named/Fireball1.png";
            break;
        case SpellType::Devastate :
            texture = "img/SpellIcon/Named/Devastate1.png";
            break;
        case SpellType::Lacerate :
            texture = "img/SpellIcon/Named/Lacerate1.png";
            break;
        case SpellType::ArmorUp :
            texture = "img/SpellIcon/Named/ArmorUP1.png";
            break;
        case SpellType::Heal :
            texture = "img/SpellIcon/Named/Heal1.png";
            break;
        case SpellType::Berserk :
            texture = "img/SpellIcon/Named/Berserk1.png";
            break;
        case SpellType::DamageUp :
            texture = "img/SpellIcon/Named/DamageUP1.png";
            break;
        case SpellType::Revenge :
            texture = "img/SpellIcon/Named/Revenge1.png";
            break;
        case SpellType::Unknow :
            texture = "img/redsquare.png";
        default:
            texture = "img/SpellIcon/Named/Basic1.png";
            break;
        }
        return &gResourceManager().getTexture(texture);
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