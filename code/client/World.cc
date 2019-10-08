#include "World.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <iostream>

namespace redsquare
{
    void World::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        target.draw( m_TileSet, states );
    }

    World::World()
    : m_TileSet({ MapSize, MapSize })
    {
        tileTexture.loadFromFile( "data/redsquare/img/NewLayerTest.png" );
        m_TileSet.setTilesetTileSize({ TileSetSize, TileSetSize });
        m_TileSet.setTileSize({ TileSize, TileSize });
        m_TileSet.setMargin({ 0, 0 });
        m_TileSet.setSpacing({ 0, 0 });
        m_TileSet.setTexture( tileTexture );
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
                    m_TileSet.setTile( {i, j}, 0 );
                    std::cout << "X";
                }
                else if ( m_World( { i, j } ) == Tile::Wall )
                {
                    //std::cout << "Wall at {" << i << "," << j << "}\n";
                    m_TileSet.setTile( {i, j}, 1 );
                    
                }else if( m_World( { i, j } ) == Tile::Void){
                    //std::cout << "Void at {" << i << "," << j << "}\n";
                    m_TileSet.setTile( {i, j}, 3);
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
    }
}