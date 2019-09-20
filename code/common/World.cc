#include "World.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

namespace redsquare
{
    void World::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        m_GroundLayer.draw( target, states );
    }

    World::World()
    : m_GroundLayer({ MapSize, MapSize })
    {
        testTexture.loadFromFile( "data/redsquare/img/TileLayer.png" );
        m_GroundLayer.setTileSize({ TileSize, TileSize });
        m_GroundLayer.setTilesetTileSize({ TileSize, TileSize });
        m_GroundLayer.setMargin({ 0, 0 });
        m_GroundLayer.setSpacing({ 0, 0 });
        m_GroundLayer.setTexture( testTexture );

        for(int i = 0; i < MapSize; ++i)
        {
            for (int j = 0; j < MapSize; ++j)
            {
                m_GroundLayer.setTile( {i, j}, 0 );
            }
        }
    }
}