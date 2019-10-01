#include "World.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

namespace redsquare
{
    World::World()
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

        m_World( {0, 0} ) = Tile::Wall;
    }
}