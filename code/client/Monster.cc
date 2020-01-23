#include "Monster.h"
#include "World.h"
#include "../common/Singletons.h"

#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Color.h>
#include <gf/Text.h>

#define NBMONSTER 5

namespace redsquare
{
    Monster::Monster( gf::Id entityID, EntitySubType entitySubType, gf::Vector2i pos )
    : ClientEntity(entityID, EntityType::Monster, entitySubType)
    , m_Font(gResourceManager().getFont("font/arial.ttf"))
    {
        m_Pos = pos;
        loadTexture(entitySubType);
    }

    void Monster::loadTexture( const EntitySubType type )
    {
        switch (type)
        {
            case EntitySubType::Bat:
                monsterTexture = &gResourceManager().getTexture("img/Monster/Bat.png");
                break;

            case EntitySubType::SkeletonKnife:
                monsterTexture = &gResourceManager().getTexture("img/Monster/SkeletonKnife.png");
                break;

            case EntitySubType::SkeletonMagus:
                monsterTexture = &gResourceManager().getTexture("img/Monster/SkeletonMagus.png");
                break;
    
            case EntitySubType::Slime:
                monsterTexture = &gResourceManager().getTexture("img/Monster/Slime.png");
                break;

            case EntitySubType::Spirit:
                monsterTexture = &gResourceManager().getTexture("img/Monster/Spirit.png");
                break;

            default:
                monsterTexture = &gResourceManager().getTexture("img/Monster/Slime.png");
                break;
        }
    }

    void Monster::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Sprite sprite;
        static constexpr gf::Vector2f BarSize = { 20.0f, 3.0f } ;
         static constexpr gf::Vector2f BarOffset = { 2.0f, 6.0f };
        static constexpr gf::Vector2f BarOffset2 = { 2.0f, 6.0f };
        static constexpr gf::Vector2f BarOffsetXp = { 4.0f, 5.0f };

        sprite.setPosition( m_Pos * World::TileSize );
        sprite.setScale( 1 );
        sprite.setTexture( *monsterTexture );
        target.draw(sprite, states);

        gf::Color4f color(255,0,0,174);
        gf::RectangleShape bar;

        bar.setSize({BarSize.width * 1 , BarSize.height});
        bar.setColor(color);
        bar.setOutlineColor(gf::Color::darker(color));
        bar.setPosition(m_Pos * World::TileSize-BarOffset);
        bar.setAnchor(gf::Anchor::TopLeft);
        target.draw(bar, states);   

        gf::RectangleShape bar2;
        color = gf::Color::Green;
        
        bar2.setSize({BarSize.width * (static_cast<float>(m_LifePoint)/static_cast<float>(m_MaxLifePoint)), BarSize.height});
        bar2.setColor(color);
        bar2.setPosition(m_Pos * World::TileSize-BarOffset2);
        bar2.setAnchor(gf::Anchor::TopLeft);
        target.draw(bar2, states); 

         gf::Text m_lvl;
        m_lvl.setCharacterSize(8);
        m_lvl.setColor(gf::Color::Black);
        m_lvl.setFont(m_Font);
        m_lvl.setString("" + std::to_string(m_Level));
        m_lvl.setPosition(m_Pos * World::TileSize-BarOffsetXp);
        m_lvl.setAnchor(gf::Anchor::CenterRight);
        target.draw(m_lvl, states);
    }

    void Monster::update(gf::Time time)
    {
        //Do something
    }
}