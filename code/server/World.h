#ifndef REDSQUARE_SERVER_WORLD_H
#define REDSQUARE_SERVER_WORLD_H

#include "../common/Packet.h"

#include <gf/Array2D.h>
#include <gf/Map.h>

namespace redsquare
{
    class World
    {
    public:
        static constexpr int MapSize = 128;
        static constexpr int TileSize = 16;
        gf::Array2D<Tile> m_World;
        
        World();

    private:
        gf::SquareMap m_SquareWorld;
    };
}

#endif