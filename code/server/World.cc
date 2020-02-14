#include "World.h"

#include <stdlib.h>
#include <cstdlib>
#include <cstdio>

#include <gf/Vector.h>
#include <gf/Map.h>

#include "Game.h"

#include "../common/Constants.h"


constexpr int BOSSROOM = 20;

static const char *BossRoom1[BOSSROOM] = {
    "###################",
    "#  AC    S    CA  #",
    "#       T T       #",
    "# PA           AP #",
    "#                 #",
    "#                 #",
    "#                 #",
    "# P             P #",
    "#    X       X    #",
    "#     X     X     #",
    "#                 #",
    "# P             P #",
    "#                 #",
    "#                 #",
    "#                 #",
    "# PA           AP #",
    "#        O        #",
    "#                 #",
    "#                 #",
    "###################",
};

static const char *BossRoom2[BOSSROOM] = {
    "###################",
    "#X       S       X#",
    "#                 #",
    "#                 #",
    "#     P     P     #",
    "#                X#",
    "#     P     P     #",
    "#                 #",
    "#     P     P     #",
    "#C       O       C#",
    "#     P     P     #",
    "#                 #",
    "#     P     P     #",
    "#X                #",
    "#     P     P     #",
    "#                 #",
    "#     P     P     #",
    "#                 #",
    "#X               X#",
    "###################",
};

static const char *BossRoom3[BOSSROOM] = {
    "###################",
    "#                 #",
    "#V               V#",
    "#                 #",
    "#V               V#",
    "#                 #",
    "#V               V#",
    "#                 #",
    "#V      ###      V#",
    "#       ###       #",
    "#V      CSC      V#",
    "#   X   X X   X   #",
    "#V               V#",
    "#                 #",
    "#V               V#",
    "#        O        #",
    "#V               V#",
    "#                 #",
    "#V               V#",
    "###################",
};

static const std::vector<const char**> BossRoomTab = {BossRoom1,BossRoom2,BossRoom3};


namespace redsquare
{

    World::World()
    : m_SquareWorld({MapSize, MapSize})
    , m_World({MapSize, MapSize})
    {
    }

    void World::generateWorld(bool bossRoom, Game& game)
    {
        if(!bossRoom){

            uint SizeGrind = 20; // sizegrind must divide MapSize to be usefull
            uint numberRoom = 10;

            m_SquareWorld.reset(gf::Flags<gf::CellProperty>());
            std::fill(m_World.begin(),m_World.end(),Tile::Void);
            TabRoom.clear();
            
            /**** GENERATE ****/
            std::vector<gf::Vector4u> TabGrid = grid(SizeGrind); // build grind
            generateFloorV2(numberRoom,SizeGrind,TabGrid); // generate the room
            buildWall(TabRoom); // build wall around room
            destroyGrid(); // destroy the grid

            road(TabRoom); // build the road
            buildWallCorridor(); // build the wall of corridor
            setWalkable(); // Set walkable the tile that should be
            /**** GENERATE ****/
        }else{

            m_SquareWorld.reset(gf::Flags<gf::CellProperty>());
            std::fill(m_World.begin(),m_World.end(),Tile::Void);
            TabRoom.clear();
            std::map<gf::Id, Prop>::iterator itNewProp; // map of props

            int randomBoss = rand() % BossRoomTab.size();
            const char** BossRoom =  BossRoomTab[randomBoss];

            for(uint row = 0 ; row < BOSSROOM; ++row){
                for(uint column = 0 ; column < BOSSROOM; ++column){
                    gf::Vector2u pos = {row,column};
                    if (BossRoom[column][row] == ' ') { // room

                        m_SquareWorld.setEmpty(pos);
                        m_World( pos ) = Tile::Room;
                        m_SquareWorld.setWalkable(pos, true);

                    }else if (BossRoom[column][row] == '#') { // wall

                        m_World( pos ) = Tile::Wall;
                        m_SquareWorld.setWalkable(pos, false);

                    }else if (BossRoom[column][row] == 'X') { // spawn of ennemy

                        m_World( pos ) = Tile::Room;
                        m_SquareWorld.setWalkable(pos, true);
                        gf::Id id = game.generateId();
                        std::map<gf::Id, Monster>::iterator itNewMonster;

                        // Create a new monster
                        std::tie(itNewMonster, std::ignore) = game.m_Monsters.emplace(id, Monster(id));

                        itNewMonster->second.m_Pos = {pos};
                        setWalkableFromEntity(&(itNewMonster->second),false);
                        setTransparentFromEntity(&(itNewMonster->second),false);
                        drawRoutine(itNewMonster->second);

                        itNewMonster->second.levelUp(game.m_Floor); // spawn ennemy with level equals to m_floor

                    }else if (BossRoom[column][row] == 'O') { // spawn of player

                        m_World( pos ) = Tile::Room;
                        m_Spawn = pos;
                        m_SquareWorld.setWalkable(pos, true);

                    }else if (BossRoom[column][row] == 'S'){ // spawn of stair

                        m_World( pos ) = Tile::Stair;
                        m_StairPosition = pos; 
                        m_SquareWorld.setWalkable(pos, true);

                    }else if (BossRoom[column][row] == 'C'){ // spawn of chest

                        m_World( pos ) = Tile::Room;
                        gf::Id id = game.generateId();
                        std::tie(itNewProp, std::ignore) = game.m_Props.emplace(id, Prop(id, EntitySubType::Chest));
                        itNewProp->second.m_Pos = pos;

                        m_SquareWorld.setWalkable(pos, false);

                    }else if (BossRoom[column][row] == 'P'){ // spawn pillar props

                        m_World( pos ) = Tile::Room;
                        gf::Id id = game.generateId();
                        std::tie(itNewProp, std::ignore) = game.m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                        itNewProp->second.m_Pos = pos;

                        m_SquareWorld.setWalkable(pos, false);
                    }else if (BossRoom[column][row] == 'A'){ // spawn torch

                        m_World( pos ) = Tile::Room;
                        gf::Id id = game.generateId();
                        std::tie(itNewProp, std::ignore) = game.m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                        itNewProp->second.m_Pos = pos;

                        m_SquareWorld.setWalkable(pos, false);
                    }else if (BossRoom[column][row] == 'T'){ // spawn throne

                        m_World( pos ) = Tile::Room;
                        gf::Id id = game.generateId();
                        std::tie(itNewProp, std::ignore) = game.m_Props.emplace(id, Prop(id, EntitySubType::Throne));
                        itNewProp->second.m_Pos = pos;

                        m_SquareWorld.setWalkable(pos, false);
                    }else if (BossRoom[column][row] == 'V'){ // spawn vase

                        m_World( pos ) = Tile::Room;
                        gf::Id id = game.generateId();
                        std::tie(itNewProp, std::ignore) = game.m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                        itNewProp->second.m_Pos = pos;

                        m_SquareWorld.setWalkable(pos, false);
                    }
                }
            }
        }

    }

    std::vector<gf::Vector4u> World::grid(uint sizeGrind){ // sizegrind must divide MapSize to be usefull

        uint gridX = 0;
        uint gridY = 0;

        for(uint length = 0; length < MapSize; length++){
            gridX++;
            for(uint width = 0; width < MapSize; width++){
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
        for(uint i = 0; i < MapSize ; i += sizeGrind){
            for(uint j = 0; j < MapSize ; j += sizeGrind){
                TabCell.push_back({number,i,j,false});
                number++;
            }
        }
        return TabCell;
    }

    void World::generateFloorV2(uint nbRoom,uint sizeGrind,std::vector<gf::Vector4u> MapGrind){
        
        uint numberCell = MapGrind.size(); // number of cell in the grind

        uint randNumCase;
        uint length;
        uint width;

        uint posX;
        uint posY;


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
                    m_SquareWorld.setEmpty({(int) (room[0]+length), (int) (room[1]+width)} );  
                }
            }
        }
    }


     std::vector<gf::Vector4u> World::buildWall(std::vector<gf::Vector4u> TabRoom){ // TODO 

        uint length , width;
        std::vector<gf::Vector4u> fullRoom;

        for(gf::Vector4u room : TabRoom){
            length = room[2] + 2; // length of the room + 2 to put wall on both side
            width = room[3] + 2; // width of the room + 2 to put wall on both side
            for(uint i = 0; i < length; ++i){
                for(uint j = 0; j < width; ++j){
                    if(i == 0 || j == 0 || i == length-1 || j == width-1){
                        m_World({(room[0] -1)+i,(room[1]-1)+j}) = Tile::Wall; // set tile to wall
                    }
                }
            }
            gf::Vector4u wall({(room[0]-1),(room[1]-1),length,width});
            fullRoom.push_back(wall);
        }
        return fullRoom;
     }

    void World::destroyGrid(){ // destroy the grid 
        for(uint i = 0; i < MapSize; ++i){
            for(uint j = 0; j < MapSize; ++j){
                if(m_World({i,j}) == Tile::Grid){
                    m_World({i,j}) = Tile::Void; // set tile to Void
                }
            }
        }
    }

    gf::Vector2u World::MiddleRoom(std::vector<gf::Vector4u> TabRoom , uint random){ // take the tile in the miidle of the room
        gf::Vector4u firstRoom = TabRoom[random]; // random room
        gf::Vector2u road({firstRoom[0]+(firstRoom[2]/2),firstRoom[1]+(firstRoom[3]/2)}); // will stock the tile int the middle of the selected room

        return road;    
    }

    void World::road(std::vector<gf::Vector4u> TabRoom){ // take 2 room and link them with a corridor
        gf::SquareMap tampon = m_SquareWorld; // using tampon to not modify m_squareworld flags

        for(int i = 0; i < MapSize; ++i){
            for (int j = 0; j < MapSize; ++j){  
                tampon.setWalkable({i,j}, true); // every tile are walkable  on the tampon
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
                if(m_World({(uint)road[0],(uint)road[1]}) != Tile::Room){
                    m_World({(uint)road[0],(uint)road[1]}) = Tile::Corridor; // set tile to Corridor   
                } // set tile to Ground        
            }

            for(gf::Vector2i road2 : points2){
                tampon.setEmpty({road2[0],road2[1]});
                if(m_World({(uint)road2[0],(uint)road2[1]}) != Tile::Room){
                    m_World({(uint)road2[0],(uint)road2[1]}) = Tile::Corridor; // set tile to Corridor   
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

    bool World::nextToGround(uint x, uint y){ // check if the current tile is next to a tile ground
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

    void World::setWalkable(){ // set walkable all the tile that should be
        for(uint i = 0; i < MapSize; ++i){
            for (uint j = 0; j < MapSize; ++j){    
                if( m_World( { i, j } ) == Tile::Corridor || m_World( { i, j} ) == Tile::Stair || m_World( { i, j } ) == Tile::Room){ // room and corridor and stair are walkable
                   m_SquareWorld.setEmpty({(int)i,(int)j});
                }
            }
        }
    }

    void World::getSpawnPoint(Game &game) // set a spawn point and check if all tile around him is a room or a corrdior to make player spawnable on these tile
    {
        int newPosX;
        int newPosY;

        do
        {
            newPosX = rand() % MapSize;
            newPosY = rand() % MapSize;
        } while( m_World( { (uint)newPosX,(uint) newPosY } ) != Tile::Room ); // only putting stair on an empty randon room's tile 

        m_Spawn = std::move(gf::Vector2i(newPosX, newPosY));

        for(int i = -1; i < 2 ; ++i) // check if all the tile around are either a corridor's tile or a room's tile and there is no monster or props on the position
        {
            for(int j = -1; j < 2; ++j)
            {
                if (!m_SquareWorld.isWalkable({m_Spawn[0] + i, m_Spawn[1] + j}))
                {
                    getSpawnPoint(game); // call the method again
                    return;
                }
            }
        } 
    }

    void World::putStair(Game &game) // put a stair somewhere on the map
    {
        int newPosX, newPosY;

        do{
            newPosX = rand() % MapSize;
            newPosY = rand() % MapSize;
        }while(!m_SquareWorld.isWalkable({newPosX,newPosY}) || m_World({(uint)newPosX,(uint)newPosY}) != Tile::Room); // only putting stair on a  randon room's tile

        m_World( { (uint)newPosX, (uint)newPosY } ) = Tile::Stair; // 1 stair for a floor   
        m_StairPosition = {newPosX, newPosY};    
    }

    void World::spawnProps(Prop &prop, Game &game, gf::Vector4u currentRoom)
    {
        int newPosX, newPosY;
        int numberOfTry = 0; // will stop the while loop if we can't place the props in 10 try
        bool free;
        do
        {
            free = true;
            if( numberOfTry == 10 )
            {
                game.m_Props.erase(prop.getEntityID());
                return;
            }

            numberOfTry++;
            newPosX = currentRoom[0] + (rand() % currentRoom[2]); // length of the room
            newPosY = currentRoom[1] + (rand() % currentRoom[3]); // width of the room

            for(int size1 = 0 ; size1 < prop.m_Size[0]; ++size1)
            {
                for(int size2 = 0 ; size2 < prop.m_Size[1]; ++size2)
                {
                    if(!m_SquareWorld.isWalkable({newPosX + size1, newPosY + size2}))
                    {
                        free = false;
                        break;
                    }
                }

                if (!free)
                {
                    break;
                }
            }
        }while(!free);

        prop.m_Pos = std::move(gf::Vector2i(newPosX, newPosY));
        
        setWalkableFromEntity(&prop, false);
        setTransparentFromEntity(&prop, false);
    }

    void World::monsterSpawn(Monster &monster, uint floor) // set to a monster a spawn
    {
        monster.levelUp(floor);

        int newPosX, newPosY;
        do
        {
            newPosX = rand() % MapSize;
            newPosY = rand() % MapSize;
        } while( !m_SquareWorld.isWalkable({newPosX, newPosY}) );

        monster.m_Pos = std::move(gf::Vector2i(newPosX, newPosY));

        setWalkableFromEntity(&monster, false);
        setTransparentFromEntity(&monster, false);
        
        drawRoutine(monster);
    }

     void World::drawRoutine(Monster &monster){
        int x;
        int y;

        do{
            x = rand() % MapSize;
            y = rand() % MapSize;
            gf::Vector2i spawn({x,y});  
            monster.m_Routine = spawn;
        }while(m_World( { (uint)x,(uint) y } ) != Tile::Room && !m_SquareWorld.isWalkable({x,y})); // only putting stair on a  randon room's tile 
    }

    void World::prettyPrint(){ // printing the map on server's console
        std::cout << "\n";
        for(uint i = 0; i < MapSize; ++i){
            for (uint j = 0; j < MapSize; ++j){     
                if (m_World( { j, i } ) == Tile::Room) {
                    std::cout << " ";
                }else if( m_World( { j, i } ) == Tile::Wall){
                    std::cout << "W";
                }else if( m_World( { j, i } ) == Tile::Void){
                    std::cout << " ";
                }else if( m_World( { j, i } ) == Tile::Grid){
                    std::cout << " ";
                }else if( m_World( { j, i } ) == Tile::Test){
                    std::cout << " ";
                }else if( m_World( { j, i } ) == Tile::Corridor){
                    std::cout << "C";
                }else if( m_World( { j, i } ) == Tile::Stair){
                    std::cout << "X";
                }
            }
            std::cout << "\n";
        }
    }

    void World::prettyPrintWalkable(){ // printing the walkable tile on server's console
        std::cout << "\n";
        for(uint i = 0; i < MapSize; ++i){
            for (uint j = 0; j < MapSize; ++j){    
                gf::Vector2i pair({(int)j,(int)i});
                if( m_SquareWorld.isWalkable(pair) ){
                    std::cout << "W";
                }else{
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
    }

    void World::setWalkableFromEntity(redsquare::Entity *entity, bool walkable){
        for(uint i = 0; i < entity->m_Size[0]; ++i)
        {
            for(uint j = 0; j < entity->m_Size[1]; ++j)
            {
                m_SquareWorld.setWalkable({entity->m_Pos[0] + (int)i, entity->m_Pos[1] + (int)j}, walkable);
            }
        }
    }

    void World::setWalkableFromEntity(gf::Vector2i Pos, gf::Vector2i size, bool walkable){
        for(uint i = 0; i < size[0]; ++i)
        {
            for(uint j = 0; j < size[1]; ++j)
            {
                m_SquareWorld.setWalkable({Pos[0] + (int)i, Pos[1] + (int)j}, walkable);
            }
        }
    }

    void World::setTransparentFromEntity(redsquare::Entity *entity, bool transparent)
    {
        for(uint i = 0; i < entity->m_Size[0]; ++i)
        {
            for(uint j = 0; j < entity->m_Size[1]; ++j)
            {
                m_SquareWorld.setTransparent({entity->m_Pos[0] + (int)i, entity->m_Pos[1] + (int)j}, transparent);
            }
        }
    }
}
