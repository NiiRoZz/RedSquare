#ifndef REDSQUARE_SERVER_WORLD_H
#define REDSQUARE_SERVER_WORLD_H

#include "../common/Protocol.h"
#include "../common/Entity.h"

#include <gf/Array2D.h>
#include <gf/Map.h>
#include <list>
#include "Monster.h"
#include "Prop.h"

namespace redsquare
{
    //Forward classes
    class RedsquareInstance;
    class Monster;
    class Prop;

    class World
    {
    public:
        World(RedsquareInstance &instance);

        void getSpawnPoint(); // point of player spawn
        void putStair(); // put stair on map
        void drawRoutine(Monster &monster); // draw path of player
        void monsterSpawn(Monster &monster, uint floor); // spawn of a monster
        void spawnProps(Prop &prop, gf::Vector4u currentRoom); // spawn of props
        void setWalkableFromEntity(redsquare::Entity *entity, bool walkable);
        void setTransparentFromEntity(redsquare::Entity *entity, bool transparent);
        void prettyPrintWalkable(); // print if the map on which tile is walkable
        void generateWorld(bool bossRoom); // generate world
        void prettyPrint(); // print the map with char in the console

    public:
        static constexpr uint MapSize = 100; // size of the map in tile
        static constexpr uint TileSize = 16;

    public:
        gf::Array2D<Tile> m_World; // first coordinate is for column and the second is for row

        std::vector<gf::Vector4u> TabRoom; // posX , posY , length, width

        gf::SquareMap m_SquareWorld;

        gf::Vector2i m_Spawn; // position of player's spawn
        gf::Vector2i m_StairPosition; // position of the stair in the floor

    private:
        std::vector<gf::Vector4u> grid(uint sizeGrid); // return all the cell of the grid in a vector
        std::vector<gf::Vector4u> buildWall(std::vector<gf::Vector4u> TabRoom); // return all the room with wall in a vector

        gf::Vector2u MiddleRoom(std::vector<gf::Vector4u> TabRoom,uint random); // select the middle of a random room

        void generateFloorV2(uint nbRoom, uint sizeGrind, std::vector<gf::Vector4u> MapGrind); // return all the room in a vector
        void destroyGrid(); // destroy the grid
        void road(std::vector<gf::Vector4u> TabRoom); // link two room together
        void buildWallCorridor(); // build wall around ground
        bool nextToGround(uint x, uint y); // check if the tile is next to a ground tile
        void setWalkable(); // set a tile walkable
    
    private:
        RedsquareInstance &m_Instance;
    };
}

#endif

