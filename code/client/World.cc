#include "World.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <iostream>

namespace redsquare
{
    void World::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        target.draw( m_GroundLayer, states );
        target.draw( m_WallLayer, states );
    }

    World::World()
    : m_GroundLayer({ MapSize, MapSize })
    , m_WallLayer({ MapSize, MapSize })
    {
        tileTexture.loadFromFile( "data/redsquare/img/TileLayer.png" );
        m_GroundLayer.setTileSize({ TileSize, TileSize });
        m_GroundLayer.setTilesetTileSize({ TileSize, TileSize });
        m_GroundLayer.setMargin({ 0, 0 });
        m_GroundLayer.setSpacing({ 0, 0 });
        m_GroundLayer.setTexture( tileTexture );

        m_WallLayer.setTileSize({ TileSize, TileSize });
        m_WallLayer.setTilesetTileSize({ TileSize, TileSize });
        m_WallLayer.setMargin({ 32, 32 });
        m_WallLayer.setSpacing({ 0, 0 });
        m_WallLayer.setTexture( tileTexture );
    }

    void World::generateWorld( gf::Array2D<Tile> world )
    {
        m_World = std::move(world);

        for(uint i = 0; i < MapSize; ++i)
        {
            for (uint j = 0; j < MapSize; ++j)
            {     
                if ( m_World( { i, j } ) == Tile::Ground )
                {
                    m_GroundLayer.setTile( {i, j}, 0 );
                }
                else if ( m_World( { i, j } ) == Tile::Wall )
                {
                    std::cout << "Wall at {" << i << "," << j << "}\n";
                    m_WallLayer.setTile( {i, j}, 0 );
                }
            }
        }
    }
}