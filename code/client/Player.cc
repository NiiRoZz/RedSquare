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

        static constexpr gf::Vector2f BarSize = { 20.0f, 5.0f } ;
        static constexpr gf::Vector2f BarOffset = { 2.0f, 6.0f };
        static constexpr gf::Vector2f BarOffset2 = { 2.0f, 6.0f };

        gf::Sprite sprite;

        sprite.setPosition( m_Pos * World::TileSize );
        sprite.setScale( 1 );
        sprite.setTexture( testTexture );
        target.draw(sprite, states);
        
        gf::Color4f color(255,0,0,174);
        gf::RectangleShape bar;

        bar.setSize(BarSize);
        bar.setColor(color);
        bar.setColor(gf::Color::darker(color) * gf::Color::Opaque(0.3f));
        bar.setOutlineColor(gf::Color::darker(color));
        bar.setPosition(m_Pos * World::TileSize-BarOffset);
        bar.setAnchor(gf::Anchor::TopLeft);
        target.draw(bar, states);   

        gf::RectangleShape bar2;
        color = gf::Color::Green;
        
        bar2.setSize({BarSize.width * 0.5 , BarSize.height});
        bar2.setColor(color);
        bar2.setPosition(m_Pos * World::TileSize-BarOffset2);
        bar2.setAnchor(gf::Anchor::TopLeft);
        target.draw(bar2, states); 

    
    }

    void Player::update(gf::Time time)
    {
        //Do something
    }

    void Player::loadTexture()
    {
        testTexture.loadFromFile("data/redsquare/img/redsquare.png");
    }

    Player::Player()
    : m_Pos( gf::Vector2i( 0, 0 ) )
    {
        loadTexture();
    }

    Player::Player( gf::Vector2i pos )
    : m_Pos( pos )
    {
        loadTexture();
    }
}
