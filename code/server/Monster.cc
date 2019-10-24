#include "Monster.h"

namespace redsquare
{
    Monster::Monster(gf::Id entityID)
    : ServerEntity(entityID,static_cast<EntityClass>(rand() % static_cast<int>(EntityClass::EntityClassCount)))
    {
        m_LifePoint = 100;
        m_MaxLifePoint = 100;

        m_AttackPoint = 20;
        m_MaxAttackPoint = 20;

        m_DefensePoint = 20;
        m_MaxDefensePoint = 20;

        m_Range = 1;

        m_Level = 1;
    }

    void Monster::createCarPacket(Packet &packet)
    {
        packet.type = PacketType::EntityCar;
        packet.entityCar.entityType = EntityType::Monster;
        packet.entityCar.entityID = m_EntityID;

        packet.entityCar.m_LifePoint = m_LifePoint;
        packet.entityCar.m_MaxLifePoint = m_MaxLifePoint;

        packet.entityCar.m_AttackPoint = m_AttackPoint;
        packet.entityCar.m_DefensePoint = m_DefensePoint;

        packet.entityCar.m_MaxAttackPoint = m_MaxAttackPoint;
        packet.entityCar.m_MaxDefensePoint = m_MaxDefensePoint;

        packet.entityCar.m_Range = m_Range;

        packet.entityCar.m_Level = m_Level;
    }

    void Monster::monsterSpawn(std::map<gf::Id,Monster> &m_Monsters,World &world){ // set to a monster a spawn
        int x;
        int y;

        do{
            x = rand() % World::MapSize;
            y = rand() % World::MapSize;
            gf::Vector2i spawn({x,y});
            m_Pos = spawn;

        }while(world.m_World( { (uint)x,(uint) y } ) != Tile::Room); // only putting stair on a  randon room's tile 

        auto it = m_Monsters.begin();

        while ( it != m_Monsters.end() )
        {
            if( it->first != m_EntityID )
            {
                if (it->second.m_Pos[0] == m_Pos[0] && it->second.m_Pos[1] == m_Pos[1])
                {
                    monsterSpawn(m_Monsters,world);
                }
            }
            ++it;
        }
        drawRoutine(world);
    }

    void Monster::drawRoutine(World &world){
        int x;
        int y;

        do{
            x = rand() % World::MapSize;
            y = rand() % World::MapSize;
            gf::Vector2i spawn({x,y});
            m_Routine = spawn;
        }while(world.m_World( { (uint)x,(uint) y } ) != Tile::Room); // only putting stair on a  randon room's tile 
    }

    bool Monster::checkRoutine(){
        if(m_Pos[0] == m_Routine[0] && m_Pos[1] == m_Routine[1]){
            return true;
        }
        return false;
    }
}