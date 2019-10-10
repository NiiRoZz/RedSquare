#include "World.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Vector.h>
#include <stdlib.h>

#define MINSIZE 6

namespace redsquare
{

    World::World()
    : m_SquareWorld({MapSize, MapSize})
    , m_World({MapSize, MapSize})
    {
        int SizeGrind = 20; // sizegrind must divide MapSize to be usefull
        int numberRoom = 5; // number of room whi must be < TabRoom.size()

        /**** GENERATE ****/
        std::vector<gf::Vector4i> TabGrind = grid(SizeGrind); // build grind
        std::vector<gf::Vector4i> TabRoom = generateFloorV2(numberRoom,SizeGrind,TabGrind); // generate the room
        buildWall(TabRoom); // build wall around room
        destroyGrid(); // destroy the grid 
        /**** GENERATE ****/

        /**** PRINT ****/
        prettyPrint(); // print the map
        /**** PRINT ****/
    }

    std::vector<gf::Vector4i> World::grid(uint sizeGrind){ // sizegrind must divide MapSize to be usefull

        std::cout << "grid STARTED\n";
        uint gridX = 0;
        uint gridY = 0;

        for(uint length =0; length < MapSize; length++){
            gridX++;
            for(uint width =0; width < MapSize; width++){
                gridY++;
                if(gridY == sizeGrind){
                    m_World( {length,width} ) = Tile::Grid; // set tile to ground
                    gridY = 0;
                }
                if(gridX == sizeGrind){
                    m_World({length,width}) = Tile::Grid; // set tile to ground
                }
                if(length == 0 || width == 0){
                    m_World({length,width}) = Tile::Grid; // set tile to ground
                }
            }
            if(gridX == sizeGrind){
                gridX = 0;
            }
        }

        std::vector<gf::Vector4i> TabCell; //  number of the cell in the grid / coord x / coor y / is filled

        int number = 0;
        for(int i = 0; i < MapSize ; i +=sizeGrind){
            for(int j = 0; j < MapSize ; j +=sizeGrind){
                TabCell.push_back({number,i,j,false});
                //std::cout << "number :" << number <<" coordonnée : " << i << "," << j << '\n' ;
                number++;
            }
        }
        std::cout << "grid ENDED\n";
        return TabCell;
    }

    std::vector<gf::Vector4i> World::generateFloorV2(uint nbRoom,uint sizeGrind,std::vector<gf::Vector4i> MapGrind){

        std::cout << "generateFloorV2 STARTED\n";
        
        int numberCell = MapGrind.size(); // number of cell in the grind

        int randNumCase;
        int length;
        int width;

        int posX;
        int posY;

        std::vector<gf::Vector4i> TabRoom;


        for(uint room = 0; room < nbRoom ; room++){
            do{
                randNumCase = rand() % numberCell; // randomly select a cell on the grind not already filled by another room
            }while(MapGrind[randNumCase][3] == 1);

            posX = MapGrind[randNumCase][1]+1; // room always at x=1 in the cell's grind
            posY = MapGrind[randNumCase][2]+1; // room always at x=1 in the cell's grind

            do{
                length = rand() % sizeGrind +MINSIZE;
                width = rand() % sizeGrind +MINSIZE;
            }while(posX+length > (MapGrind[randNumCase][1]+sizeGrind)-2|| posY+width > (MapGrind[randNumCase][2]+sizeGrind)-2 ||  posY+width == MapSize);


            gf::Vector4i Room({posX,posY,length,width});
            TabRoom.push_back(Room);
            MapGrind[randNumCase][3] = 1; // set the cell N°randNumCase to 1 because he is now filled with a room 
        }

        for(gf::Vector4i room : TabRoom){ // set the floor on map
            for(uint length = 0; length < room[2]; ++length){
                for(uint width = 0; width < room[3]; ++width){
                    m_World( {room[0]+length, room[1]+width} ) = Tile::Ground;
                    m_SquareWorld.setWalkable({room[0]+length, room[1]+width} );
                    m_SquareWorld.setTransparent({room[0]+length, room[1]+width});   
                }
            }
            //std::cout << "X :"  << room[0] << " Y : " << room[1] << " Taille : " << room[2] << "x" << room[3] << "\n" ;
        }
        std::cout << "generateFloorV2 ENDED\n";
        return TabRoom;
    }


     std::vector<gf::Vector4i> World::buildWall(std::vector<gf::Vector4i> TabRoom){ // build wall around all the room
        std::cout << "buildWall STARTED\n";
        int length , width;
        for(gf::Vector4i room : TabRoom){
            length = room[2] + 2; // length of the room + 2 to put wall on both side
            width = room[3] + 2; // width of the room + 2 to put wall on both side
            //std::cout << "length: " << length << " width : " << width << "\n";
            for(int i = 0; i < length; ++i){
                for(int j = 0; j < width; ++j){
                    if(i == 0 || j == 0 || i == length-1 || j == width-1){
                        m_World({(room[0] -1)+i,(room[1]-1)+j}) = Tile::Wall; // set tile to wall
                        //std::cout << "SET WALL\n";
                    }
                }
            }
        }
        std::cout << "buildWall ENDED\n";
        return TabRoom;
     }

    void World::destroyGrid(){ // destroy the grid 
        std::cout << "buildWall STARTED\n";
        for(int i = 0; i < MapSize; ++i){
            for(int j = 0; j < MapSize; ++j){
                if(m_World({i,j}) == Tile::Grid){
                    m_World({i,j}) = Tile::Void; // set tile to Void
                }
            }
        }
        std::cout << "buildWall ENDED\n";
    }

    void World::prettyPrint(){ // printing the map on server's console
        std::cout << "\n";
        for(uint i = 0; i < MapSize; ++i){
            for (uint j = 0; j < MapSize; ++j){     
                if (m_World( { j, i } ) == Tile::Ground) {
                    std::cout << " ";
                }else if ( m_World( { j, i } ) == Tile::Wall){
                    std::cout << "X";
                }else if( m_World( { j, i } ) == Tile::Void){
                    std::cout << " ";
                }else if( m_World( { j, i } ) == Tile::Grid){
                    std::cout << "=";
                }
            }
            std::cout << "\n";
        }
    }
}