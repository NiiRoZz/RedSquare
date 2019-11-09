#include "Prop.h"
#include "World.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

namespace redsquare
{
    Prop::Prop( gf::Id entityID, EntityClass type, gf::Vector2i pos )
    : redsquare::Entity(entityID,type)
    {
        m_Pos = pos;
        loadTexture(type);
    }

    void Prop::loadTexture(const EntityClass type)
    {
        switch (type)
        {
            case EntityClass::Chair:
            {
                propTexture.loadFromFile("data/redsquare/img/spirit.png");
                break;
            }

            default:
            {
                break;
            }
        }
    }

    void Prop::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Sprite sprite;

        sprite.setPosition( m_Pos * World::TileSize );
        sprite.setScale( 1 );
        sprite.setTexture( propTexture );
        target.draw(sprite, states);
    }

    void Prop::update(gf::Time time)
    {
        //Do something
    }
}