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
        static constexpr int MapSize = 60;
        static constexpr int TileSize = 16;
        gf::Array2D<Tile> m_World; // first coordinate is for column and the second is for row
        gf::SquareMap m_SquareWorld;

        World();
        


        private:

            std::vector<gf::Vector4i> grid(uint sizeGrid); // return all the cell of the grid in a vector
            std::vector<gf::Vector4i> generateFloorV2(uint nbRoom, uint sizeGrind, std::vector<gf::Vector4i> MapGrind); // return all the room in a vector
            std::vector<gf::Vector4i> buildWall(std::vector<gf::Vector4i> TabRoom); // return all the room with wall in a vector
            void destroyGrid(); // destroy the grid
            void prettyPrint(); // print the map with char in the console
    };
}

#endif
