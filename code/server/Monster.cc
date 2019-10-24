#include "Monster.h"

namespace redsquare
{
    Monster::Monster(gf::Id monsterID)
    : m_MonsterID(monsterID)
    , m_TypeOfMonster((uint8_t)(rand() % 4))
    {
    }

    void Monster::playTurn()
    {
        // do AI here
    }

    void Monster::createCarPacket(Packet &packet)
    {
        packet.type = PacketType::EntityCar;
        packet.entityCar.entityType = EntityType::Monster;
        packet.entityCar.entityID = m_MonsterID;

        packet.entityCar.m_LifePoint = m_LifePoint;
        packet.entityCar.m_MaxLifePoint = m_MaxLifePoint;

        packet.entityCar.m_AttackPoint = m_AttackPoint;
        packet.entityCar.m_DefensePoint = m_DefensePoint;
        packet.entityCar.m_MovePoint = m_MovePoint;
        packet.entityCar.m_Range = m_Range;

        packet.entityCar.m_Level = m_Level;
    }
}