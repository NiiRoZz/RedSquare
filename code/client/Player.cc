#include "Player.h"
#include "World.h"

#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Color.h>

namespace redsquare
{
    void Player::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        static constexpr gf::Vector2f BarSize = { 20.0f, 3.0f } ;
        static constexpr gf::Vector2f BarSize2 = { 20.0f, 1.5f } ;
        static constexpr gf::Vector2f BarOffset = { 2.0f, 6.0f };
        static constexpr gf::Vector2f BarOffset2 = { 2.0f, 6.0f };
        static constexpr gf::Vector2f BarOffsetMana1 = { 2.0f, 3.0f };
        static constexpr gf::Vector2f BarOffsetMana2 = { 2.0f, 3.0f };

        gf::Sprite sprite;

        sprite.setPosition( m_Pos * World::TileSize );
        sprite.setScale( 1 );
        sprite.setTexture( testTexture );
        target.draw(sprite, states);
        
        gf::Color4f color(255,0,0,174);
        gf::RectangleShape bar;

        bar.setSize(BarSize);
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
    }

    void Player::update(gf::Time time)
    {
        //Do something
    }

    void Player::loadTexture( uint8_t type )
    {
        switch (type)
        {
            case 0:
            {
                testTexture.loadFromFile("data/redsquare/img/goblin.png");
                break;
            }

            case 1:
            {
                testTexture.loadFromFile("data/redsquare/img/knight.png");
                break;
            }

            case 2:
            {
                testTexture.loadFromFile("data/redsquare/img/bat.png");
                break;
            }

            case 3:
            {
                testTexture.loadFromFile("data/redsquare/img/slime.png");
                break;
            }
        
            default:
            {
                break;
            }
        }
    }

    Player::Player()
    {
        m_Pos = gf::Vector2i( 0, 0 );
        loadTexture();
    }

    Player::Player( gf::Vector2i pos, uint8_t type )
    {
        m_Pos = pos;
        loadTexture(type);
    }

    bool Player::canAttack(gf::Vector2i targetPos, std::map<gf::Id, Player> &players, std::map<gf::Id, Monster> &monsters)
    {
        if ( m_Pos == targetPos )
        {
            return false;
        }
        
        gf::Distance2<int> distFn = gf::euclideanDistance<int, 2>;

        float distance = distFn(m_Pos, targetPos);

        if ( distance > m_Range )
        {
            return false;
        }

        auto it = players.begin();
 
        while ( it != players.end() )
        {
            if ( it->second.m_Pos == targetPos )
            {
                return true;
            }

            ++it;
        }

        auto it2 = monsters.begin();
 
        while ( it2 != monsters.end() )
        {
            if ( it2->second.m_Pos == targetPos )
            {
                return true;
            }

            ++it2;
        }

        return false;
    }

    bool Player::canMove(gf::Vector2i targetPos, std::map<gf::Id, Player> &players, std::map<gf::Id, Monster> &monsters, gf::SquareMap &map)
    {
        if ( m_Pos == targetPos )
        {
            return false;
        }
        
        gf::Distance2<int> distFn = gf::euclideanDistance<int, 2>;

        float distance = distFn(m_Pos, targetPos);

        if ( distance > m_MovePoint )
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

        return map.isWalkable(targetPos);
    }
}
