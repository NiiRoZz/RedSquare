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
        static constexpr uint MapSize = 100; // size of the map in tile
        static constexpr uint TileSize = 16;
        gf::Array2D<Tile> m_World; // first coordinate is for column and the second is for row
        gf::SquareMap m_SquareWorld;


        World();

        private:
            std::vector<gf::Vector4u> grid(uint sizeGrid); // return all the cell of the grid in a vector
            std::vector<gf::Vector4u> generateFloorV2(uint nbRoom, uint sizeGrind, std::vector<gf::Vector4u> MapGrind); // return all the room in a vector
            std::vector<gf::Vector4u> buildWall(std::vector<gf::Vector4u> TabRoom); // return all the room with wall in a vector
            void destroyGrid(); // destroy the grid
            gf::Vector2i MiddleRoom(std::vector<gf::Vector4u> TabRoom,uint random); // select the middle of a random room
            void road(std::vector<gf::Vector4u> TabRoom); // link two room together
            void buildWallCorridor(); // build wall around ground
            bool nextToGround(uint x, uint y); // check if the tile is next to a ground tile
            void putStair();
            void SetWalkable();
            void prettyPrintWalkable();
            void prettyPrint(); // print the map with char in the console
    };
}

#endif

