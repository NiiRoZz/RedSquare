#include "Player.h"
#include "../common/World.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

namespace redsquare
{
    void Player::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Sprite sprite;

        sprite.setPosition( { m_Pos[0] * World::TileSize, m_Pos[1] * World::TileSize } );
        sprite.setScale( 1 );
        sprite.setTexture( testTexture );

        target.draw(sprite, states);
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