#include "World.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

namespace redsquare
{
    World::World()
    : m_SquareWorld( {MapSize, MapSize} )
    {
    }
}