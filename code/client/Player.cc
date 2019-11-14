#include "Player.h"
#include "World.h"
#include "../common/Singletons.h"

#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Color.h>
#include <gf/Text.h>


namespace redsquare
{
    Player::Player( gf::Id entityID )
    : redsquare::Entity(entityID,static_cast<EntitySubType>(rand() % static_cast<int>(EntitySubType::EntityClassCount)))
    , m_Font(gResourceManager().getFont("font/arial.ttf"))
    {
        m_Pos = gf::Vector2i( 0, 0 );
        loadTexture();
    }

    Player::Player( gf::Id entityID, EntitySubType type, gf::Vector2i pos )
    : redsquare::Entity(entityID,type)
    , m_Font(gResourceManager().getFont("font/arial.ttf"))
    {
        m_Pos = pos;
        loadTexture(type);
    }

    void Player::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        static constexpr gf::Vector2f BarSize = { 20.0f, 3.0f } ;
        static constexpr gf::Vector2f BarSize2 = { 20.0f, 1.5f } ;
        static constexpr gf::Vector2f BarOffset = { 2.0f, 6.0f };
        static constexpr gf::Vector2f BarOffset2 = { 2.0f, 6.0f };
        static constexpr gf::Vector2f BarOffsetMana1 = { 2.0f, 3.0f };
        static constexpr gf::Vector2f BarOffsetMana2 = { 2.0f, 3.0f };
        static constexpr gf::Vector2f BarOffsetXp = { 4.0f, 4.0f };

        gf::Sprite sprite;

        sprite.setPosition( m_Pos * World::TileSize );
        sprite.setScale( 1 );
        sprite.setTexture( *playerTexture );
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

        gf::RectangleShape barMana2;
        color = gf::Color::Cyan;
     
        bar2.setSize({BarSize2.width * 1 , BarSize2.height});
        bar2.setColor(color);
        bar2.setPosition(m_Pos * World::TileSize-BarOffsetMana2);
        bar2.setAnchor(gf::Anchor::TopLeft);
        target.draw(bar2, states); 

        gf::RectangleShape barMana1;
        color = gf::Color::Blue;
        
        bar2.setSize({BarSize2.width * (static_cast<float>(m_ManaPoint)/static_cast<float>(m_MaxManaPoint)) , BarSize2.height});
        bar2.setColor(color);
        bar2.setPosition(m_Pos * World::TileSize-BarOffsetMana1);
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

    void Player::update(gf::Time time)
    {
        //Do something
    }

    void Player::loadTexture( const EntitySubType type )
    {
        switch (type)
        {
            case EntitySubType::Magus:
            {
                playerTexture = &gResourceManager().getTexture("img/goblin.png");
                break;
            }

            case EntitySubType::Warrior:
            {
                playerTexture = &gResourceManager().getTexture("img/knight.png");
                break;
            }

            /*
            case 2:
            {
                playerTexture.loadFromFile("data/redsquare/img/bat.png");
                break;
            }

            case 3:
            {
                playerTexture.loadFromFile("data/redsquare/img/slime.png");
                break;
            }
            */
        
            default:
            {
                break;
            }
        }
    }

    bool Player::canAttack(gf::Vector2i targetPos, std::map<gf::Id, Monster> &monsters, std::map<gf::Id, Prop> &props)
    {
        if ( m_Pos == targetPos )
        {
            return false;
        }
        
        gf::Distance2<int> distFn = gf::manhattanDistance<int, 2>;

        float distance = distFn(m_Pos, targetPos);

        if ( distance > m_Range )
        {
            return false;
        }

        auto it = monsters.begin();
 
        while ( it != monsters.end() )
        {
            if ( it->second.m_Pos == targetPos )
            {
                return true;
            }

            ++it;
        }

        auto it2 = props.begin();
 
        while ( it2 != props.end() )
        {
            if ( it2->second.m_Pos == targetPos )
            {
                return true;
            }

            ++it2;
        }

        return false;
    }

    bool Player::canMove(gf::Vector2i targetPos, std::map<gf::Id, Player> &players, std::map<gf::Id, Monster> &monsters, std::map<gf::Id, Prop> &props, gf::SquareMap &map)
    {
        if ( m_Pos == targetPos || targetPos[0] < 0 || targetPos[1] < 0 )
        {
            return false;
        }

        auto it = players.begin();
 
        while ( it != players.end() )
        {
            if ( it->second.m_Pos == targetPos )
            {
                return false;
            }

            ++it;
        }

        auto it2 = monsters.begin();
 
        while ( it2 != monsters.end() )
        {
            if ( it2->second.m_Pos == targetPos )
            {
                return false;
            }

            ++it2;
        }

        auto it3 = props.begin();
 
        while ( it3 != props.end() )
        {
            if ( it3->second.m_Pos == targetPos )
            {
                return false;
            }

            ++it3;
        }

        return map.isWalkable(targetPos);
    }
}
