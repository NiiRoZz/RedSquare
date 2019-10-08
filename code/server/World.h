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
        static constexpr int MapSize = 50;
        static constexpr int TileSize = 16;
        gf::Array2D<Tile> m_World; // first coordinate is for column and the second is for row
        World();
        gf::SquareMap m_SquareWorld;
        


        private:
            void generateFloor(int nbRoom,int maxLength,int maxWidth);
            bool isOnRoom( std::vector<gf::Vector4i> &AllRoom , gf::Vector4i &TestRoom);

    };
}

#endif
