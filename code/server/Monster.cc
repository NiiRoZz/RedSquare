#include "Monster.h"


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
        case 5:
            m_EntitySubType = EntitySubType::Demon;
            break;
        case 6:
            m_EntitySubType = EntitySubType::Goblin;
            break;
        case 7:
            m_EntitySubType = EntitySubType::Imp;
            break;
        case 8:
            m_EntitySubType = EntitySubType::LilGob;
            break;
        case 9:
            m_EntitySubType = EntitySubType::LilZombie;
            break;
        case 10:
            m_EntitySubType = EntitySubType::Lizard;
            break;
        case 11:
            m_EntitySubType = EntitySubType::Mask;
            break;
        case 12:
            m_EntitySubType = EntitySubType::Mud;
            break;
        case 13:
            m_EntitySubType = EntitySubType::Orc;
            break;
        case 14:
            m_EntitySubType = EntitySubType::Shaman;
            break;
        case 15:
            m_EntitySubType = EntitySubType::Swamp;
            break;
        case 16:
            m_EntitySubType = EntitySubType::Zombie;
            break;
        default:
            m_EntitySubType = EntitySubType::Bat;
            break;
        }
    
       switch (m_EntitySubType)
       {
       case EntitySubType::Bat:  case EntitySubType::Demon:  case EntitySubType::Zombie: 
            m_LifePoint = 170;
            m_MaxLifePoint = 170;

            m_AttackPoint = 50;
            m_MaxAttackPoint = 50;

            m_DefensePoint = 4;
            m_MaxDefensePoint = 4;

            m_Range = 1;
            m_Level = 1;
           break;

        case EntitySubType::SkeletonKnife:  case EntitySubType::Lizard:  case EntitySubType::Mask: case EntitySubType::Orc:  
            m_LifePoint = 220;
            m_MaxLifePoint = 220;

            m_AttackPoint = 60;
            m_MaxAttackPoint = 60;

            m_DefensePoint = 5;
            m_MaxDefensePoint = 5;

            m_Range = 1;
            m_Level = 1;
           break;

        case EntitySubType::SkeletonMagus: case EntitySubType::Shaman:   
            m_LifePoint = 190;
            m_MaxLifePoint = 190;

            m_AttackPoint = 50;
            m_MaxAttackPoint = 50;

            m_DefensePoint = 5;
            m_MaxDefensePoint = 5;

            m_Range = 2;
            m_Level = 1;
           break;

        case EntitySubType::Slime:  case EntitySubType::Mud: case EntitySubType::Swamp: case EntitySubType::Goblin: 
            m_LifePoint = 160;
            m_MaxLifePoint = 160;

            m_AttackPoint = 45;
            m_MaxAttackPoint = 45;

            m_DefensePoint = 5;
            m_MaxDefensePoint = 5;

            m_Range = 1;
            m_Level = 1;
           break;

        case EntitySubType::Spirit: case EntitySubType::LilZombie: case EntitySubType::LilGob: case EntitySubType::Imp:    
            m_LifePoint = 140;
            m_MaxLifePoint = 140;

            m_AttackPoint = 45;
            m_MaxAttackPoint = 45;

            m_DefensePoint = 5;
            m_MaxDefensePoint = 5;

            m_Range = 1;
            m_Level = 1;
           break;
       default:
            m_LifePoint = 220;
            m_MaxLifePoint = 220;

            m_AttackPoint = 50;
            m_MaxAttackPoint = 50;

            m_DefensePoint = 5;
            m_MaxDefensePoint = 5;

            m_Range = 1;
            m_Level = 1;
           break;
       }
    }

    bool Monster::checkRoutine(){
        if(m_Pos[0] == m_Routine[0] && m_Pos[1] == m_Routine[1]){
            return true;
        }
        return false;
    }

    void Monster::attack(ServerEntity *target)
    {
        int damage = (m_AttackPoint*m_AttackPoint) / (m_AttackPoint + target->m_DefensePoint);
        damage += Variance(-10);
        if(damage < 0){
            damage = -damage;
        }
        if(target->m_LifePoint - damage < 0){
            std::cout << "The monster killed a player " << std::endl;
            /*createSystemMessage("The monster killed a player " , "system","monster");*/

            target->m_LifePoint = 0;
            return;
        }
    
        target->m_LifePoint -= damage;
        std::string messToChat("");
        /*std::cout << "The monster dealed " << damage << " damage " << std::endl;
        messToChat = "The monster dealed " +std::to_string(damage)+" damage ";
        createSystemMessage("The monster killed a player " , "system","monster");*/
    }

    void Monster::levelUp(uint m_Floor){ // method to level up a monster

        int gain;
        if(m_Floor < 5){
            gain = 4;
        }else if(m_Floor >= 5 && m_Floor < 8){
            gain = 5;
        }else{
            gain = 6;
        }
        
        m_MaxLifePoint += gain*m_Floor;
        m_LifePoint += gain*m_Floor;

        m_AttackPoint += gain*m_Floor;
        m_DefensePoint += gain*m_Floor;
        m_Level = (m_Floor+1);
    }

    int Monster::Variance(int range){ // adding some rng to the damage of a spell [range;+range] added to the base damage of the spell
        int randomNum = rand() % (range*2) + (range);
        return randomNum;
    }

    /*void Monster::createSystemMessage(std::string message, std::string to,std::string name){
        std::cout<<message<<std::endl;
            Message packet;
            int length=0;
            std::string myMessage;
            myMessage =  name + " -> "+message;
            std::string sys("system");
            length = myMessage.copy(packet.message, myMessage.length());
            packet.message[length]='\0';
            length = sys.copy(packet.from, sys.length());
            packet.from[length]='\0';
            if(to != "system"){
                length = to.copy(packet.to, to.length());
                packet.to[length]='\0';
            }else{
                length = sys.copy(packet.to, sys.length());
                packet.to[length]='\0';
            }
            Chat::getInstance().sendMessage(packet);  
    }*/

}