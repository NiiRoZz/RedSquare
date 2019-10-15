#include "World.h"

#include <stdlib.h>
#include <cstdlib>
#include <cstdio>

#include <gf/Vector.h>
#include <gf/Map.h>


#define MINSIZE 6

namespace redsquare
{

    World::World()
    : m_SquareWorld({MapSize, MapSize})
    , m_World({MapSize, MapSize})
    {
        uint SizeGrind = 20; // sizegrind must divide MapSize to be usefull
        uint numberRoom = 10; // number of room, must be < TabRoom.size()

        /**** GENERATE ****/
        std::vector<gf::Vector4u> TabGrid = grid(SizeGrind); // build grind
        std::vector<gf::Vector4u> TabRoom = generateFloorV2(numberRoom,SizeGrind,TabGrid); // generate the room
        TabRoom = buildWall(TabRoom); // build wall around room
        destroyGrid(); // destroy the grid

        road(TabRoom);
        // TODO set a tile unwalkable 

        buildWallCorridor(); // build the wall of corridor

        putStair(); // put stair on the map
       
        /**** GENERATE ****/

        /**** PRINT ****/
        prettyPrint(); // print the map
        /**** PRINT ****/
    }

    std::vector<gf::Vector4u> World::grid(uint sizeGrind){ // sizegrind must divide MapSize to be usefull

        //std::cout << "grid STARTED\n";
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

        std::vector<gf::Vector4u> TabCell; //  number of the cell in the grid / coord x / coor y / is filled

        uint number = 0;
        for(uint i = 0; i < MapSize ; i +=sizeGrind){
            for(uint j = 0; j < MapSize ; j +=sizeGrind){
                TabCell.push_back({number,i,j,false});
                //std::cout << "number :" << number <<" coordonnée : " << i << "," << j << '\n' ;
                number++;
            }
        }
        //std::cout << "grid ENDED\n";
        return TabCell;
    }

    std::vector<gf::Vector4u> World::generateFloorV2(uint nbRoom,uint sizeGrind,std::vector<gf::Vector4u> MapGrind){

        //std::cout << "generateFloorV2 STARTED\n";
        
        uint numberCell = MapGrind.size(); // number of cell in the grind

        uint randNumCase;
        uint length;
        uint width;

        uint posX;
        uint posY;

        std::vector<gf::Vector4u> TabRoom;


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


            gf::Vector4u Room({posX,posY,length,width});
            TabRoom.push_back(Room);
            MapGrind[randNumCase][3] = 1; // set the cell N°randNumCase to 1 because he is now filled with a room 
        }

        for(gf::Vector4u room : TabRoom){ // set the floor on map
            for(uint length = 0; length < room[2]; ++length){
                for(uint width = 0; width < room[3]; ++width){
                    m_World( {room[0]+length, room[1]+width} ) = Tile::Room;
                    m_SquareWorld.setEmpty({room[0]+length, room[1]+width} );  
                }
            }
            //std::cout << "X :"  << room[0] << " Y : " << room[1] << " Taille : " << room[2] << "x" << room[3] << "\n" ;
        }
        //std::cout << "generateFloorV2 ENDED\n";
        return TabRoom;
    }


     std::vector<gf::Vector4u> World::buildWall(std::vector<gf::Vector4u> TabRoom){ // TODO 
        //std::cout << "buildWall STARTED\n";
        uint length , width;
        std::vector<gf::Vector4u> fullRoom;

        for(gf::Vector4u room : TabRoom){
            length = room[2] + 2; // length of the room + 2 to put wall on both side
            width = room[3] + 2; // width of the room + 2 to put wall on both side
            //std::cout << "length: " << length << " width : " << width << "\n";
            for(uint i = 0; i < length; ++i){
                for(uint j = 0; j < width; ++j){
                    if(i == 0 || j == 0 || i == length-1 || j == width-1){
                        m_World({(room[0] -1)+i,(room[1]-1)+j}) = Tile::Wall; // set tile to wall
                        //std::cout << "SET WALL\n";
                    }
                }
            }
            gf::Vector4u wall({(room[0]-1),(room[1]-1),length,width});
            fullRoom.push_back(wall);
        }
        //std::cout << "buildWall ENDED\n";
        return fullRoom;
     }

    void World::destroyGrid(){ // destroy the grid 
        //std::cout << "destroyGrid STARTED\n";
        for(uint i = 0; i < MapSize; ++i){
            for(uint j = 0; j < MapSize; ++j){
                if(m_World({i,j}) == Tile::Grid){
                    m_World({i,j}) = Tile::Void; // set tile to Void
                }
            }
        }
        //std::cout << "destroyGrid ENDED\n";
    }

    gf::Vector2i World::MiddleRoom(std::vector<gf::Vector4u> TabRoom , uint random){ // take the tile in the miidle of the room
        gf::Vector4u firstRoom = TabRoom[random]; // random room
        gf::Vector2i road({firstRoom[0]+(firstRoom[2]/2),firstRoom[1]+(firstRoom[3]/2)}); // will stock the tile int the middle of the selected room

        return road;    
    }

    void World::road(std::vector<gf::Vector4u> TabRoom){ // take 2 room and link them with a corridor
        gf::SquareMap tampon = m_SquareWorld;

        for(uint i = 0; i < MapSize; ++i){
            for (uint j = 0; j < MapSize; ++j){  
                tampon.setWalkable({i,j}); // every tile are walkable at the start the the generation
            }
        }


        uint cpt = 0;
        do{
            uint random = rand()%TabRoom.size();
            uint tmp = random;

            gf::Vector2i start = MiddleRoom(TabRoom,random); // center of the first room
            do{
                random = rand()%TabRoom.size();
            }while(tmp == random);
            gf::Vector2i end = MiddleRoom(TabRoom,random); // center of the second room


            gf::Vector2i start2({start[0]-1,start[1]-1}); // tile next to the middle of the first room
            gf::Vector2i end2({end[0]-1,end[1]-1}); // tile next to the middle of the second room

            //start2 and end2 are there to make a 2 tile width corridor who's better in my opinion =)


            std::vector<gf::Vector2i> points = tampon.computeRoute(start, end, 0.0); // first set of tile for the corridor
            std::vector<gf::Vector2i> points2 = tampon.computeRoute(start2, end2, 0.0); // second set of tile for the corridor
            

            for(gf::Vector2i road : points){
                tampon.setEmpty({road[0],road[1]});
                if(m_World({road[0],road[1]}) != Tile::Room){
                    m_World({road[0],road[1]}) = Tile::Corridor; // set tile to Corridor   
                } // set tile to Ground        
            }

            for(gf::Vector2i road2 : points2){
                tampon.setEmpty({road2[0],road2[1]});
                 if(m_World({road2[0],road2[1]}) != Tile::Room){
                    m_World({road2[0],road2[1]}) = Tile::Corridor; // set tile to Corridor   
                }       
            }
            cpt++;
        }while(cpt != TabRoom.size()*3); // dummy ways to be sure that no room is isolated.   implemtation can be better with graphe algorithm
    }

    void World::buildWallCorridor(){ // put wall where there should be a wall
        for(uint i = 0; i < MapSize; ++i){
            for (uint j = 0; j < MapSize; ++j){
                if(m_World( { i, j } ) == Tile::Void){
                    if(nextToGround(i,j) == true){ // check if the tile next to this one is a ground 
                        m_World( { i, j } ) = Tile::Wall; // set the current tile to a wall
                    }
                }
            }
        }
    }

    void World::putStair(){ // put a stair somewhere on the map
        uint x = rand() % MapSize;
        uint y = rand() % MapSize;

        do{
            x = rand() % MapSize;
            y = rand() % MapSize;
        }while(m_World( { x, y } ) != Tile::Room); // only putting stair on a  randon room's tile

        m_World( { x, y } ) = Tile::Stair; // 1 stair for a floor
    }

    bool World::nextToGround(uint x, uint y){ // check if the current tile is newt to a tile ground
        if( x == 0 || y == 0 || x == MapSize-1 || y == MapSize-1){
            return false;   
        }
        if(m_World( { x-1, y } ) == Tile::Room || m_World( { x-1, y } ) == Tile::Corridor){
            return true;    
        }
        if(m_World( { x+1, y } ) == Tile::Room || m_World( { x+1, y } ) == Tile::Corridor){
            return true;    
        }
        if(m_World( { x, y-1 } ) == Tile::Room || m_World( { x, y-1 } ) == Tile::Corridor){
            return true;    
        }
        if(m_World( { x, y+1 } ) == Tile::Room || m_World( { x, y+1 } ) == Tile::Corridor){
            return true;    
        }
        return false;
    }

    void World::prettyPrint(){ // printing the map on server's console
        std::cout << "\n";
        for(uint i = 0; i < MapSize; ++i){
            for (uint j = 0; j < MapSize; ++j){     
                if (m_World( { j, i } ) == Tile::Room) {
                    std::cout << "R";
                }else if( m_World( { j, i } ) == Tile::Wall){
                    std::cout << "W";
                }else if( m_World( { j, i } ) == Tile::Void){
                    std::cout << " ";
                }else if( m_World( { j, i } ) == Tile::Grid){
                    std::cout << "=";
                }else if( m_World( { j, i } ) == Tile::Test){
                    std::cout << " ";
                }else if( m_World( { j, i } ) == Tile::Corridor){
                    std::cout << "C";
                }else if( m_World( { j, i } ) == Tile::Stair){
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
    }
}
