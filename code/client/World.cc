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
    : m_TileSet({ MapSize, MapSize } )
    , m_SquareMap( {MapSize, MapSize} )
    {
        tileTexture.loadFromFile( "data/redsquare/img/TileSet2.png" );
        m_TileSet.setTilesetTileSize({ TileSetSize, TileSetSize });
        m_TileSet.setTileSize({ TileSetSize, TileSetSize });
        m_TileSet.setMargin({ 0, 0 });
        m_TileSet.setSpacing({ 0, 0 });
        m_TileSet.setTexture( tileTexture );
    }

    int World::rigthTile(gf::Vector2i tile){ // return the tile number on the tile set for a given vector2i on the map
        if( tile[0] == MapSize-2 || tile[1] == MapSize-2 || tile[0] == 0 || tile[1] == 0){ // border of the map
            
            if(tile[0] == 0 && tile[1] == 0 && m_World(tile) == Tile::Wall){ // top left corner
                return  0;
            }
            if(tile[0] == MapSize-2 && tile[1] == 0 && m_World(tile) == Tile::Wall){ // top rigth corner
                return  5;
            }
            if(tile[0] == 0 && tile[1] == MapSize-2 && m_World(tile) == Tile::Wall){ // bottom left corner
                return  40;
            }
            if(tile[0] == MapSize-2 && tile[1] == MapSize-2 && m_World(tile) == Tile::Wall){ // bottom rigth corner
                return  45;
            }
            if(tile[0] == 0 && m_World(tile) == Tile::Wall){ // left side
                return  20;
            }
            if(tile[0] == MapSize-2 && m_World(tile) == Tile::Wall){ // right side
                return  15;
            }
            if(tile[1] == 0 && m_World(tile) == Tile::Wall){ // top side
                return  1;
            }
            if(tile[1] == MapSize-2 && m_World(tile) == Tile::Wall){ // bottom side
                return  42;
            }
        }else{
            if ( m_World( tile ) == Tile::Room || m_World( tile ) == Tile::Corridor ){ // room tile
                /*int randomTile = 0;
                do{
                    randomTile =   rand() % 35;
                }while( randomTile != 11 && randomTile != 12 & randomTile != 13 && randomTile != 14 && randomTile != 21 && randomTile != 22 && randomTile != 23 && randomTile != 24 && randomTile != 31 && randomTile != 32 && randomTile != 33 && randomTile != 34);
                std::cout << randomTile << std::endl;
                return randomTile;*/
                return 11; // TODO
            }else if ( m_World( tile ) == Tile::Wall ) // wall tile
            {
                if(m_World({tile[0]+1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]+1}) == Tile::Wall){ // top left corner in a room
                    return 0;
                }
                if(m_World({tile[0]+1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]-1}) == Tile::Wall){ // bottom left corner in a room
                    return 40;
                }
                if(m_World({tile[0]-1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]+1}) == Tile::Wall){ // top rigth corner in a room
                    return 5;
                }
                if(m_World({tile[0]-1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]-1}) == Tile::Wall){ // bottom rigth corner in a room
                    return 45;
                }
                if(m_World({tile[0]-1, tile[1]}) == Tile::Wall  && m_World({tile[0]+1, tile[1]}) == Tile::Wall){ // line wall bot
                    return 41;
                }
                if(m_World({tile[0], tile[1]-1}) == Tile::Wall  && m_World({tile[0], tile[1]+1}) == Tile::Wall){ // line wall bot
                    return 10;
                }
            }
        }
        return 79; // purple tile
    }
    void World::generateWorld( gf::Array2D<Tile> world )
    {
        m_World = std::move(world);
        int tile;

        for(int i = 0; i < MapSize; ++i){
            for (int j = 0; j < MapSize; ++j){  
                tile = rigthTile({i,j}); 
                if(tile == 11){ // room or corridor tile
                    m_SquareMap.setWalkable({i, j}); 
                }
                m_TileSet.setTile( {i, j}, tile );
            }
            std::cout << "\n";
        }
    }
}