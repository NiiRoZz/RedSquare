#include "Monster.h"
#include "World.h"

#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

namespace redsquare
{
    Monster::Monster()
    {
        m_Pos = gf::Vector2i( 0, 0 );
        loadTexture();
    }

    Monster::Monster( gf::Vector2i pos, uint8_t type )
    {
        m_Pos = pos;
        loadTexture(type);
    }

    void Monster::loadTexture( uint8_t type )
    {
        switch (type)
        {
            case 0:
            {
                monsterTexture.loadFromFile("data/redsquare/img/knifeSkull.png");
                break;
            }

            case 1:
            {
                monsterTexture.loadFromFile("data/redsquare/img/pickAxeSkull.png");
                break;
            }

            case 2:
            {
                monsterTexture.loadFromFile("data/redsquare/img/spirit.png");
                break;
            }

            case 3:
            {
                monsterTexture.loadFromFile("data/redsquare/img/vampire.png");
                break;
            }
        
            default:
            {
                break;
            }
        }
    }

    void Monster::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Sprite sprite;

        sprite.setPosition( m_Pos * World::TileSize );
        sprite.setScale( 1 );
        sprite.setTexture( monsterTexture );
        target.draw(sprite, states);
    }

    void Monster::update(gf::Time time)
    {
        //Do something
    }
}