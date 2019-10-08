#include "World.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Vector.h>
#include <stdlib.h>

namespace redsquare
{

    World::World()
    : m_SquareWorld( {MapSize, MapSize} )
    , m_World( {MapSize, MapSize} )
    {
        generateFloor(5,6,5);

        for(uint i = 0; i < MapSize; ++i){
            for (uint j = 0; j < MapSize; ++j){     
                if ( m_World( { i, j } ) == Tile::Ground ) {
                    std::cout << "X";
                }
                else if ( m_World( { i, j } ) == Tile::Wall ){
                    // no wall for now
                }else if( m_World( { i, j } ) == Tile::Void){
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
    }


    void World::generateFloor(int nbRoom, int maxLength, int maxWidth){

        int length;
        int width;

        int posX;
        int posY;
        int i =0;

        std::vector<gf::Vector4i> TabRoom;

        for(int room = 0; room < nbRoom; room++){   
            do{
                
                length = rand() % maxLength +4;
                width = rand() % maxWidth +4;

                posX = rand() % MapSize +1;
                posY = rand() % MapSize +1;

            }while( (posX+length >= MapSize || posY+width >= MapSize) );

            gf::Vector4i Room({posX,posY,length,width});

            while(i != 10 && isOnRoom(TabRoom,Room)){
                i++;
            }
            if(i == 10){
                continue;
            }
            TabRoom.push_back(Room);
        }
        for(gf::Vector4i room : TabRoom){
            for(uint length=0; length < room[2]; ++length){
                for(uint width=0; width < room[3]; ++width){
                    m_World( {room[0]+length, room[1]+width} ) = Tile::Ground;
                    m_SquareWorld.setWalkable({room[0]+length, room[1]+width} );
                    m_SquareWorld.setTransparent({room[0]+length, room[1]+width});   
                }
            }
            std::cout << "X :"  << room[0] << " Y : " << room[1] << "\n" ;
        }
        
    }

    // x , y , longueur , largeur

    bool World::isOnRoom( std::vector<gf::Vector4i> &AllRoom , gf::Vector4i &TestRoom){
        for(gf::Vector4i room : AllRoom){
            if(TestRoom[0] <= room[0]+room[2] && TestRoom[0] >= room[0] && TestRoom[1] <= room[1] + room[3] && TestRoom[1] >= room[1]){
                return true;
            }
        }
        return false;
    }
}