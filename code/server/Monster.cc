#include "Monster.h"

namespace redsquare
{
    Monster::Monster(gf::Id entityID)
    : ServerEntity(entityID,static_cast<EntitySubType>(rand() % static_cast<int>(EntitySubType::EntityClassCount)))
    {
        m_LifePoint = 100;
        m_MaxLifePoint = 100;

        m_AttackPoint = 5;
        m_MaxAttackPoint = 5;

        m_DefensePoint = 0;
        m_MaxDefensePoint = 0;

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

    bool Monster::checkRoutine(){
        if(m_Pos[0] == m_Routine[0] && m_Pos[1] == m_Routine[1]){
            return true;
        }
        return false;
    }

    void Monster::attack(ServerEntity *target)
    {
        target->m_LifePoint -= (m_AttackPoint - target->m_DefensePoint);
    }

}