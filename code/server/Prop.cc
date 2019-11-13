#include "Prop.h"

namespace redsquare
{
    Prop::Prop(gf::Id entityID, EntitySubType type)
    : redsquare::Entity(entityID,type)
    {
        //m_Pos = pos;
    }


    void Prop::spawnProps(World &world,gf::Vector4i currentRoom){
        uint posX, posY;
        do{
            posX = rand() % currentRoom[2]; // length
            posY = rand() % currentRoom[3]; // width
        }while(  world.m_World( { (currentRoom[0]+posX), (currentRoom[1]+posY) }) != Tile::Room && world.m_World({ (currentRoom[0]+posX), (currentRoom[1]+posY) }) != Tile::Corridor);   
        m_Pos = {currentRoom[0]+posX,currentRoom[1]+posY};
        world.setUnWalkable( {currentRoom[0]+posX,currentRoom[1]+posY});
    }
}