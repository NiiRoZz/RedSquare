#include "World.h"
#include "../common/Singletons.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <iostream>

#define TOP 160
#define BOTTOM 160

#define TOPLEFTCORNER 181 // intern
#define TOPRIGTHCORNER 182 // intern

#define BOTTOMLEFTCORNER 201 // intern
#define BOTTOMRIGTHCORNER 202 // intern


#define TOPLEFTCORNEREXT 181 // extern
#define TOPRIGTHCORNEREXT 182 // extern

#define BOTTOMLEFTCORNEREXT 201 // extern
#define BOTTOMRIGTHCORNEREXT 202 // extern

#define LEFT 200
#define RIGHT 200

// gray floor
#define FLOOR1 172
#define FLOOR2 173
#define FLOOR3 192
#define FLOOR4 193

// pink floor
#define FLOOR5 174
#define FLOOR6 175
#define FLOOR7 194
#define FLOOR8 195

// brown floor
#define FLOOR9 176
#define FLOOR10 177
#define FLOOR11 196
#define FLOOR12 197


#define PILLARTOP 220
#define PILLARBOTTOM 200
#define VOID  83
#define STAIR 327

namespace redsquare
{
    void World::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        target.draw( m_TileSet, states );
    }

    World::World()
    : m_TileSet({ MapSize, MapSize } )
    , m_SquareMap( {MapSize, MapSize} )
    , m_TileTexture(gResourceManager().getTexture("img/TileSet3.png"))
    {
        m_TileSet.setTilesetTileSize({ TileSetSize, TileSetSize });
        m_TileSet.setTileSize({ TileSetSize, TileSetSize });
        m_TileSet.setMargin({ 0, 0 });
        m_TileSet.setSpacing({ 0, 0 });
        m_TileSet.setTexture( m_TileTexture );
    }

    int World::rigthTile(gf::Vector2i titi){ // return the tile number on the tile set for a given vector2i on the map
        gf::Vector2u tile;
        tile[0] = (uint) titi[0];
        tile[1] = (uint) titi[1];
        if( tile[0] == MapSize-2 || tile[1] == MapSize-2 || tile[0] == 0 || tile[1] == 0){ // border of the map
            if(tile[0] != 0){
                if(m_World({tile[0]-1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]+1}) == Tile::Wall){ // top rigth corner in a room
                    return TOPRIGTHCORNER;
                }
            }
            if(tile[1] != MapSize-2){
                if(m_World({tile[0]+1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]+1}) == Tile::Wall){ // top left corner in a room
                    return TOPLEFTCORNER;
                }
            }
            if(tile[0] == 0 && tile[1] != MapSize-2){
                if(m_World({tile[0]+1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]-1}) == Tile::Wall){ // bottom left corner in a room
                    return BOTTOMLEFTCORNER;
                }
            }
            if(tile[0] != 0 && tile[1] == MapSize-2){
                if(m_World({tile[0]-1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]-1}) == Tile::Wall){ // bottom rigth corner in a room
                    return BOTTOMRIGTHCORNER;
                }
            }

    

            if(tile[0] == 0 && tile[1] == MapSize-2 && m_World(tile) == Tile::Wall){ // bottom left corner
                return  BOTTOMLEFTCORNER;
            }
            if(tile[0] == MapSize-2 && tile[1] == MapSize-2 && m_World(tile) == Tile::Wall){ // bottom rigth corner
                return  BOTTOMRIGTHCORNER;
            }
            if(tile[0] == 0 && m_World(tile) == Tile::Wall){ // left side
                return  LEFT;
            }
            if(tile[0] == MapSize-2 && m_World(tile) == Tile::Wall){ // right side
                return  RIGHT;
            }
            if(tile[1] == 0 && m_World(tile) == Tile::Wall){ // top side
                return  TOP;
            }
            if(tile[1] == MapSize-2 && m_World(tile) == Tile::Wall){ // bottom side
                return  BOTTOM;
            }
        }else{if ( m_World( tile ) == Tile::Room || m_World( tile ) == Tile::Corridor ){ // room tile
                int randomTile = 0;
                randomTile =   rand() % 11;

                if(randomTile == 0){
                    return FLOOR1;
                }else if(randomTile == 1){
                    return FLOOR2;
                }else if(randomTile == 2){
                    return FLOOR3;
                }else if(randomTile == 3){
                    return FLOOR4;
                }/*else if(randomTile == 4){
                    return FLOOR5;
                }else if(randomTile == 5){
                    return FLOOR6;
                }else if(randomTile == 6){
                    return FLOOR7;
                }else if(randomTile == 7){
                    return FLOOR8;
                }*/else if(randomTile == 8){
                    return FLOOR9;
                }else if(randomTile == 9){
                    return FLOOR10;
                }else if(randomTile == 10){
                    return FLOOR11;
                }else if(randomTile == 11){
                    return FLOOR12;
                }
                return FLOOR1;


            }else if( m_World( tile ) == Tile::Stair){
                return STAIR;
            }else if ( m_World( tile ) == Tile::Wall ) // wall tile
            {
                if(m_World({tile[0]+1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]+1}) == Tile::Wall && (m_World({tile[0], tile[1]-1}) == Tile::Room || m_World({tile[0], tile[1]-1}) == Tile::Corridor) && (m_World({tile[0]-1, tile[1]}) == Tile::Room || m_World({tile[0]-1, tile[1]}) == Tile::Corridor)){ // top left corner in a room ext
                    return TOPLEFTCORNEREXT;
                }
                if(m_World({tile[0]-1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]+1}) == Tile::Wall && (m_World({tile[0]+1, tile[1]}) == Tile::Room || m_World({tile[0]+1, tile[1]}) == Tile::Corridor) && (m_World({tile[0], tile[1]-1}) == Tile::Room || m_World({tile[0], tile[1]-1}) == Tile::Corridor )){ // top rigth corner in a room ext
                    return TOPRIGTHCORNEREXT;
                }
                if(m_World({tile[0]+1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]-1}) == Tile::Wall && (m_World({tile[0]-1, tile[1]}) == Tile::Room || m_World({tile[0]-1, tile[1]}) == Tile::Corridor) && (m_World({tile[0], tile[1]+1}) == Tile::Room || m_World({tile[0], tile[1]+1}) == Tile::Corridor )){ // bottom left corner in a room ext
                    return TOP;
                }
                if(m_World({tile[0]-1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]-1}) == Tile::Wall && (m_World({tile[0]+1, tile[1]}) == Tile::Room || m_World({tile[0]+1, tile[1]}) == Tile::Corridor) && (m_World({tile[0], tile[1]+1}) == Tile::Room || m_World({tile[0], tile[1]+1}) == Tile::Corridor )){ // bottom right corner in a room ext
                    return TOP;
                }


                if(m_World({tile[0]+1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]+1}) == Tile::Wall){ // top left corner in a room
                    return TOPLEFTCORNER;
                }
                if(m_World({tile[0]-1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]+1}) == Tile::Wall){ // top rigth corner in a room
                    return TOPRIGTHCORNER;
                }
                if(m_World({tile[0]+1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]-1}) == Tile::Wall){ // bottom left corner in a room
                    return BOTTOMLEFTCORNER;
                }
                if(m_World({tile[0]-1, tile[1]}) == Tile::Wall  && m_World({tile[0], tile[1]-1}) == Tile::Wall){ // bottom rigth corner in a room
                    return BOTTOMRIGTHCORNER;
                }
                
                if(m_World({tile[0], tile[1]-1}) == Tile::Wall  && m_World({tile[0], tile[1]+1}) == Tile::Wall && m_World({tile[0]-1, tile[1]}) == Tile::Void  ){ // line wall bot
                    return LEFT;
                }
                /*if(m_World({tile[0], tile[1]-1}) == Tile::Wall  && m_World({tile[0], tile[1]+1}) == Tile::Wall && m_World({tile[0]+1, tile[1]}) == Tile::Void  ){ // line wall bot
                    return RIGHT;
                }*/
                if(m_World({tile[0]-1, tile[1]}) == Tile::Wall  && m_World({tile[0]+1, tile[1]}) == Tile::Wall && m_World({tile[0], tile[1]-1}) == Tile::Void){ // line wall bot
                    return TOP;
                }
                if(m_World({tile[0]-1, tile[1]}) == Tile::Wall  && m_World({tile[0]+1, tile[1]}) == Tile::Wall && m_World({tile[0], tile[1]+1}) == Tile::Void){ // line wall bot
                    return BOTTOM;
                }

                if(m_World({tile[0], tile[1]-1}) == Tile::Wall && m_World({tile[0], tile[1]+1}) == Tile::Wall && m_World({tile[0], tile[1]}) == Tile::Wall){ // line wall bot
                    return RIGHT;
                }    

                if(( m_World({tile[0]-1 , tile[1]}) == Tile::Room || m_World({tile[0]-1 , tile[1]}) == Tile::Corridor) && ( m_World({tile[0]-1 , tile[1]}) != Tile::Room) ){ // end of a line of 1 wall
                    return PILLARTOP;
                }  

                if(( m_World({tile[0]+1 , tile[1]}) == Tile::Room || m_World({tile[0]+1 , tile[1]}) == Tile::Corridor) ){ // end of a line of 1 wall
                    return PILLARBOTTOM;
                }  

                if(( m_World({tile[0] , tile[1]+1}) == Tile::Room || m_World({tile[0] , tile[1]+1}) == Tile::Corridor) ){ // line wall bot
                    return BOTTOM;
                } 
            }
        }
        return VOID; // purple tile
    }

    void World::generateWorld( gf::Array2D<Tile> world )
    {
        m_World = std::move(world);
        m_SquareMap.reset(gf::Flags<gf::CellProperty>());
        int tile;

        for(int i = 0; i < MapSize; ++i)
        {
            for (int j = 0; j < MapSize; ++j)
            {  
                tile = rigthTile({i,j});

                if(tile == FLOOR1 || tile == FLOOR2 || tile == FLOOR3 || tile == FLOOR4 || tile == FLOOR5|| tile == FLOOR6|| tile == FLOOR7|| tile == FLOOR8|| tile == FLOOR9|| tile == FLOOR10|| tile == FLOOR11|| tile == FLOOR12 || tile == STAIR ) // room or corridor tile
                {
                    m_SquareMap.setWalkable({i, j}, true); 
                }

                m_TileSet.setTile( {i, j}, tile );
            }
        }
    }

    void World::setWalkableFromEntity(redsquare::Entity *entity, bool walkable)
    {
        for(uint i = 0; i < entity->m_Size[0]; ++i)
        {
            for(uint j = 0; j < entity->m_Size[1]; ++j)
            {
                m_SquareMap.setWalkable({entity->m_Pos[0] + (int)i, entity->m_Pos[1] + (int)j}, walkable);
            }
        }
    }
}