#include "Monster.h"

#define NBMONSTER 5

namespace redsquare
{
    Monster::Monster(gf::Id entityID)
    : ServerEntity(entityID, EntityType::Monster, EntitySubType::Slime)
    {
        int randomMonster = rand() % NBMONSTER;

        switch (randomMonster)
        {
        case 0:
            m_EntitySubType = EntitySubType::Bat;
            break;
        case 1:
            m_EntitySubType = EntitySubType::SkeletonKnife;
            break;
        case 2:
            m_EntitySubType = EntitySubType::SkeletonMagus;
            break;
        case 3:
            m_EntitySubType = EntitySubType::Slime;
            break;
        case 4:
            m_EntitySubType = EntitySubType::Spirit;
            break;
        default:
            m_EntitySubType = EntitySubType::Bat;
            break;
        }
    
       switch (m_EntitySubType)
       {
       case EntitySubType::Bat:    
            m_LifePoint = 150;
            m_MaxLifePoint = 150;

            m_AttackPoint = 8;
            m_MaxAttackPoint = 8;

            m_DefensePoint = 4;
            m_MaxDefensePoint = 4;

            m_Range = 1;
            m_Level = 1;
           break;

        case EntitySubType::SkeletonKnife:    
            m_LifePoint = 180;
            m_MaxLifePoint = 180;

            m_AttackPoint = 15;
            m_MaxAttackPoint = 15;

            m_DefensePoint = 5;
            m_MaxDefensePoint = 5;

            m_Range = 1;
            m_Level = 1;
           break;

        case EntitySubType::SkeletonMagus:    
            m_LifePoint = 160;
            m_MaxLifePoint = 160;

            m_AttackPoint = 10;
            m_MaxAttackPoint = 10;

            m_DefensePoint = 5;
            m_MaxDefensePoint = 5;

            m_Range = 2;
            m_Level = 1;
           break;

        case EntitySubType::Slime:    
            m_LifePoint = 140;
            m_MaxLifePoint = 140;

            m_AttackPoint = 11;
            m_MaxAttackPoint = 11;

            m_DefensePoint = 3;
            m_MaxDefensePoint = 3;

            m_Range = 1;
            m_Level = 1;
           break;

        case EntitySubType::Spirit:    
            m_LifePoint = 100;
            m_MaxLifePoint = 100;

            m_AttackPoint = 12;
            m_MaxAttackPoint = 12;

            m_DefensePoint = 2;
            m_MaxDefensePoint = 2;

            m_Range = 1;
            m_Level = 1;
           break;
       default:
            m_LifePoint = 200;
            m_MaxLifePoint = 200;

            m_AttackPoint = 10;
            m_MaxAttackPoint = 10;

            m_DefensePoint = 5;
            m_MaxDefensePoint = 5;

            m_Range = 1;
            m_Level = 1;
           break;
       }
    }

    void Monster::createCarPacket(Packet &packet)
    {
        packet.type = PacketType::EntityCar;
        packet.entityCar.entityType = EntityType::Monster;
        packet.entityCar.entityID = getEntityID();

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
        int damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
        damage += Variance(-10);
        if(damage < 0){
            damage = -damage;
        }
        if(target->m_LifePoint - damage < 0){
            std::cout << "The monster killed a player " << std::endl;
            target->m_LifePoint = 0;
            return;
        }
    
        target->m_LifePoint -= damage;

        std::cout << "The monster dealed " << damage << " damage " << std::endl;
    }

    void Monster::levelUp(uint m_Floor){ // method to level up a player

        m_MaxLifePoint += 2*m_Floor;
        m_LifePoint += 2*m_Floor;

        m_AttackPoint += 2*m_Floor;
        m_DefensePoint += 2*m_Floor;
        
        m_Level = (m_Floor+1);
    }

    int Monster::Variance(int range){ // adding some rng to the damage of a spell [range;+range] added to the base damage of the spell
        int randomNum = rand() % (range*2) + (range);
        return randomNum;
    }

}