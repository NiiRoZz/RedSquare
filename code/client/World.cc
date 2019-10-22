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
        m_TileSet.setTileSize({ TileSetSize, TileSetSize });
        m_TileSet.setMargin({ 0, 0 });
        m_TileSet.setSpacing({ 0, 0 });
        m_TileSet.setTexture( tileTexture );
    }

    void World::generateWorld( gf::Array2D<Tile> world )
    {
        m_World = std::move(world);

        for(int i = 0; i < MapSize; ++i){
            for (int j = 0; j < MapSize; ++j){     
                if ( m_World( {(uint) i,(uint) j } ) == Tile::Room || m_World( { (uint) i, (uint) j } ) == Tile::Corridor ){
                    m_TileSet.setTile( {i, j}, 1 );
                }else if ( m_World( {(uint) i, (uint) j } ) == Tile::Wall ){
                    m_TileSet.setTile( {i, j}, 2 );
                }else if( m_World( {(uint) i,(uint) j } ) == Tile::Void){
                    m_TileSet.setTile( {i, j}, 3);
                }else if( m_World( {(uint) i,(uint) j } ) == Tile::Stair){
                    m_TileSet.setTile( {i, j}, 3);
                }
            }
            std::cout << "\n";
        }
    }
}