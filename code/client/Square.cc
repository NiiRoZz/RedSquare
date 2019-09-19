#include "Square.h"
#include "../common/World.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

namespace redsquare
{
    void Square::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Sprite sprite;

        sprite.setPosition( { m_Pos[0] * World::TileSize, m_Pos[1] * World::TileSize } );
        sprite.setScale( 1 );
        sprite.setTexture( testTexture );

        target.draw(sprite, states);
    }

    void Square::update(gf::Time time)
    {
        //Do something
    }

    void Square::loadTexture()
    {
        testTexture.loadFromFile("data/redsquare/img/redsquare.png");
    }

    gf::Vector2i Square::getPos()
    {
        return m_Pos;
    }

    Square::Square()
    {
        setPos( gf::Vector2i( 0, 0 ) );

        loadTexture();
    }

    Square::Square( gf::Vector2i pos )
    {
        setPos( pos );

        loadTexture();
    }

    void Square::setPos( gf::Vector2i pos )
    {
        m_Pos = pos;
    }
}