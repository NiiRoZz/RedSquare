#include "World.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <stdlib.h>

namespace redsquare
{
    /*World::World()
    : m_SquareWorld( {MapSize, MapSize} )
    , m_World( {MapSize, MapSize} )
    {
        for(uint i = 0; i < MapSize; ++i)
        {
            for (uint j = 0; j < MapSize; ++j)
            { 
                m_World( {i, j} ) = Tile::Ground;
            }
        }
    }*/

    World::World()
    : m_SquareWorld( {MapSize, MapSize} )
    , m_World( {MapSize, MapSize} )
    {
        //int nbRoom = rand() % 10;  
        int nbRoom = 1;
        int length = rand() % 16;
        int width = rand() % 16;
        

        for(uint i =0; i < nbRoom ; i++){
            for(uint roomLength = 1; roomLength < length; ++roomLength){
                for(uint roomWidth = 1; roomWidth < width; ++roomWidth){
                    m_World( {roomLength, roomWidth} ) = Tile::Ground;
                    m_SquareWorld.setWalkable({roomLength,roomWidth});
                    m_SquareWorld.setTransparent({roomLength,roomWidth});
                }
            }
        }
    }
}