#include "Player.h"
#include <iostream>

#define RATIO 1.3

namespace redsquare
{
    Player::Player(SocketTcp socket, gf::Id playerID, const EntitySubType type)
    : ServerEntity(playerID,type)
    , m_Socket(std::move(socket))
    {
        switch (m_TypeOfEntity){
        case EntitySubType::Magus : // ------------------- Magus -------------------
            m_Class = EntitySubType::Magus;
            m_LifePoint = 100;
            m_ManaPoint = 30;

            m_MaxLifePoint = 100;
            m_MaxManaPoint = 30;

            m_AttackPoint = 30;
            m_DefensePoint = 0;

            m_MaxAttackPoint = 30;
            m_MaxDefensePoint = 0;

            m_Range = 2;

            m_SpellTab.push_back(SpellType::BasicAttack);
            break;

        case EntitySubType::Warrior : // ------------------- Warrior -------------------
            m_Class = EntitySubType::Warrior;
            m_LifePoint = 150;
            m_ManaPoint = 20;

            m_MaxLifePoint = 150;
            m_MaxManaPoint = 20;

            m_AttackPoint = 20;
            m_DefensePoint = 0;

            m_MaxAttackPoint = 20;
            m_MaxDefensePoint = 0;

            m_Range = 1;

            m_SpellTab.push_back(SpellType::BasicAttack);
            break;

        case EntitySubType::Rogue : // ------------------- Rogue -------------------
            m_Class = EntitySubType::Rogue;
            m_LifePoint = 125;
            m_ManaPoint = 25;

            m_MaxLifePoint = 125;
            m_MaxManaPoint = 25;

            m_AttackPoint = 25;
            m_DefensePoint = 0;

            m_MaxAttackPoint = 25;
            m_MaxDefensePoint = 0;

            m_Range = 1;

            m_SpellTab.push_back(SpellType::BasicAttack);
            break;

        case EntitySubType::Ranger : // ------------------- Rogue -------------------
            m_Class = EntitySubType::Ranger;
            m_LifePoint = 80;
            m_ManaPoint = 20;

            m_MaxLifePoint = 80;
            m_MaxManaPoint = 20;

            m_AttackPoint = 12;
            m_DefensePoint = 0;

            m_MaxAttackPoint = 12;
            m_MaxDefensePoint = 0;

            m_Range = 3;
            m_SpellTab.push_back(SpellType::BasicAttack);
            break;
        
        default:
            break;
        }
       
        m_XP = 0;
        m_MaxXP = 10;

        m_Level = 1;
    }

    void Player::sendPacket(Packet &packet)
    {
        m_Socket.send(packet);
    }

    void Player::receivePacket(Packet &packet)
    {
        m_Socket.receive(packet);
    }

    void Player::sendPacket( NewPlayer &packet )
    {
        m_Socket.send(packet);
    }

    bool Player::applyMove(int dirX, int dirY, World &world)
    {
        int newPosX = m_Pos[0] + dirX;
        int newPosY = m_Pos[1] + dirY;

        if ( newPosY >= 0 && newPosY < World::MapSize-1 && newPosX >= 0 && newPosX < World::MapSize-1 && world.m_SquareWorld.isWalkable( {newPosX, newPosY} ) )
        {
            world.setWalkableFromEntity(static_cast<redsquare::Entity*>(this), true);
            world.m_SquareWorld.setTransparent(m_Pos, true);

            m_Pos[1] = newPosY;
            m_Pos[0] = newPosX;

            world.setWalkableFromEntity(static_cast<redsquare::Entity*>(this), false);
            world.m_SquareWorld.setTransparent(m_Pos, false);

            return true;
        }
        else
        {
            return false;
        }

        return false;
    }

    bool Player::playerDisconnected() const
    {
        return (m_Socket.getState() == SocketState::Disconnected);
    }

     void Player::levelUp(){ // method to level up a player

        m_MaxLifePoint += 2;
        m_LifePoint = m_MaxLifePoint;

        m_MaxManaPoint += 2;
        m_ManaPoint = m_MaxManaPoint;

        m_AttackPoint += 2;
        m_DefensePoint += 2;

        m_XP = 0;
        m_MaxXP += 20;

        m_Level++;

    
        switch (m_Class)
        {
            case EntitySubType::Warrior:  
                if(m_Level == 2){
                    m_SpellTab.push_back(SpellType::DamageUp);
                }else if(m_Level == 3){
                   m_SpellTab.push_back(SpellType::ArmorUp);
                }else if(m_Level == 4){
                    m_SpellTab.push_back(SpellType::Berserk);
                }else if(m_Level == 5){
                    m_SpellTab.push_back(SpellType::Revenge);
                }else if(m_Level == 6){
                    m_SpellTab.push_back(SpellType::Massacre);
                }else if(m_Level == 7){
                    m_SpellTab.push_back(SpellType::Protection);
                }else if(m_Level == 8){
                }
                sendUpdateOfSpells();
                break;

            case EntitySubType::Magus:
                if(m_Level == 2){
                    m_SpellTab.push_back(SpellType::Heal);
                    m_SpellTab.push_back(SpellType::FireBall);
                    
                    m_SpellTab.push_back(SpellType::Incinerate);
                    m_SpellTab.push_back(SpellType::Energize);
                    m_SpellTab.push_back(SpellType::LightningStrike);
                }else if(m_Level == 3){
                   m_SpellTab.push_back(SpellType::Incinerate);
                }else if(m_Level == 4){
                    m_SpellTab.push_back(SpellType::Energize);
                }else if(m_Level == 5){
                    m_SpellTab.push_back(SpellType::LightningStrike);
                }else if(m_Level == 6){
                   // m_SpellTab.push_back(SpellType::Massacre);
                }else if(m_Level == 7){
                   // m_SpellTab.push_back(SpellType::Massacre);
                }
                sendUpdateOfSpells();
                break;

            case EntitySubType::Rogue:  
                if(m_Level == 2){
                    m_SpellTab.push_back(SpellType::Lacerate);
                }else if(m_Level == 3){
                    m_SpellTab.push_back(SpellType::Massacre);
                }else if(m_Level == 4){
                    m_SpellTab.push_back(SpellType::Berserk);
                }else if(m_Level == 5){
                    m_SpellTab.push_back(SpellType::Scorch);
                }else if(m_Level == 6){
                    m_SpellTab.push_back(SpellType::DoubleStrike);
                }else if(m_Level == 7){
                    m_SpellTab.push_back(SpellType::DamageUp);
                }
                sendUpdateOfSpells();
                break;

            case EntitySubType::Ranger:  
                if(m_Level == 2){
                    m_SpellTab.push_back(SpellType::Shoot);
                }else if(m_Level == 3){
                    m_SpellTab.push_back(SpellType::RangeUp);
                }else if(m_Level == 4){
                    m_SpellTab.push_back(SpellType::Torpedo);
                }else if(m_Level == 5){
                    m_SpellTab.push_back(SpellType::Scorch);
                }else if(m_Level == 6){
                   // m_SpellTab.push_back(SpellType::DoubleStrike);
                }else if(m_Level == 7){
                   // m_SpellTab.push_back(SpellType::Massacre);
                }
                sendUpdateOfSpells();
                break;

            case EntitySubType::Healer:  
                if(m_Level == 2){
                    m_SpellTab.push_back(SpellType::Heal);
                }else if(m_Level == 3){
                    // m_SpellTab.push_back(SpellType::Massacre);
                }else if(m_Level == 4){
                    // m_SpellTab.push_back(SpellType::Berserk);
                }else if(m_Level == 5){
                    // m_SpellTab.push_back(SpellType::Scorch);
                }else if(m_Level == 6){
                   // m_SpellTab.push_back(SpellType::DoubleStrike);
                }else if(m_Level == 7){
                   // m_SpellTab.push_back(SpellType::Massacre);
                }
                sendUpdateOfSpells();
                break;

            default: 
                break;
        }
    }

    void Player::createCarPacket(Packet &packet) // create the packet of the caracteristic who will be send to player
    {
        packet.type = PacketType::EntityCar;
        packet.entityCar.entityType = EntityType::Player;
        packet.entityCar.entityID = m_EntityID;

        packet.entityCar.m_LifePoint = m_LifePoint;
        packet.entityCar.m_ManaPoint = m_ManaPoint;

        packet.entityCar.m_MaxLifePoint = m_MaxLifePoint;
        packet.entityCar.m_MaxManaPoint = m_MaxManaPoint;

        packet.entityCar.m_AttackPoint = m_AttackPoint;
        packet.entityCar.m_DefensePoint = m_DefensePoint;

        packet.entityCar.m_MaxAttackPoint = m_MaxAttackPoint;
        packet.entityCar.m_MaxDefensePoint = m_MaxDefensePoint;

        packet.entityCar.m_Range = m_Range;

        packet.entityCar.m_XP = m_XP;
        packet.entityCar.m_MaxXP= m_MaxXP;

        packet.entityCar.m_Level = m_Level;
    }

    void Player::sendUpdateOfSpells()
    {
        Packet packet;
        packet.type = PacketType::UpdateSpells;
        std::fill(packet.updateSpells.spells, packet.updateSpells.spells + MAX_SPELL_PER_PLAYER, static_cast<SpellType>(0));

        if (m_SpellTab.size() > MAX_SPELL_PER_PLAYER)
        {
            std::copy_n(m_SpellTab.begin(), MAX_SPELL_PER_PLAYER, packet.updateSpells.spells);
        }
        else
        {
            std::copy(m_SpellTab.begin(), m_SpellTab.end(), packet.updateSpells.spells);
        }
        sendPacket(packet);
    }

    void Player::playerSpawn(World &world, int playerSpawned){ // set spawn for a player 
        switch (playerSpawned)
        {
        case 1:
            m_Pos = {(world.m_Spawn[0]-1),(world.m_Spawn[1]-1)};// the first player to connect to the game spawn on this case
            break;
        case 2:
            m_Pos = {(world.m_Spawn[0]),(world.m_Spawn[1]-1)}; // the second player to connect to the game spawn on this case
            break;
        case 3:
            m_Pos = {(world.m_Spawn[0]+1),(world.m_Spawn[1]-1)}; // the third player to connect to the game spawn on this case
            break;
        case 4:
            m_Pos = {(world.m_Spawn[0]-1),(world.m_Spawn[1])}; // the fourth player to connect to the game spawn on this case
            break;
        case 5: 
            m_Pos = {(world.m_Spawn[0]),(world.m_Spawn[1])}; // the fifth player to connect to the game spawn on this case
            break;
        case 6:
            m_Pos = {(world.m_Spawn[0]+1),(world.m_Spawn[1])};// the sixth player to connect to the game spawn on this case
            break;
        case 7:
            m_Pos = {(world.m_Spawn[0]-1),(world.m_Spawn[1]+1)}; // the seventh player to connect to the game spawn on this case
            break;
        case 8:
            m_Pos = {(world.m_Spawn[0]),(world.m_Spawn[1]+1)}; // the eigth player to connect to the game spawn on this case
            break;
        case 9:
            m_Pos = {(world.m_Spawn[0]+1),(world.m_Spawn[1]+1)}; // the ninth player to connect to the game spawn on this case
            break;                        
        default: // cant manage more than 9 player
            break;
        }
    }

    void Player::attack(SpellType spellType, ServerEntity *target)
    {
        std::cout << "XD" << std::endl; 
        m_PointInRound -= m_PointInRound;

        switch (spellType)
        {
            case SpellType::BasicAttack:

                std::cout << "XD2" << std::endl; 
                BasicAttack(target);
                break;
            case SpellType::DamageUp:
                DamageUp();
                break;
            case SpellType::FireBall:
                Fireball(target);
                break;
            case SpellType::ArmorUp:
                ArmorUp();
                break;
            case SpellType::Berserk:
                Berserk();
                break;
            case SpellType::Scorch:
                Scorch(target);
                break;
            case SpellType::Shoot:
                Shoot(target);
                break;
            case SpellType::RangeUp:
                RangeUp();
                break;
            case SpellType::Revenge:
                Revenge(target);
                break;
            case SpellType::Torpedo:
                Torpedo(target);
                break;
            case SpellType::DoubleStrike:
                DoubleStrike(target);
                break;
            case SpellType::Lacerate:
                Lacerate(target);
                break;
            case SpellType::Massacre:
                Massacre(target);
                break;
            case SpellType::Protection:
                Protection(target);
                break;
            case SpellType::Incinerate:
                Incinerate(target);
                break;
            case SpellType::Reaper:
                Incinerate(target);
                break;
            default:
                BasicAttack(target);
                break;
        }

        if( target->m_LifePoint <= 0 )
        {
            m_XP += target->m_Level*10;
            if(m_XP >= m_MaxXP)
            {
                levelUp();
            }
        }
    }

    int Player::Variance(int range){ // adding some rng to the damage of a spell [range;+range] added to the base damage of the spell
        if(range == 0){
            return 0;
        }
        int randomNum = rand() % (range*2) + (range);
        return randomNum;
    }

    void Player::BasicAttack(ServerEntity *target){ // DONE

        std::cout << target->m_Level << std::endl;
        int damage;
        int critical = rand() % 100;
        if(critical > 95){ // critical hit
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage *= 2; // double the damage 
            std::cout << " CRITICAL !!! " << std::endl;
        }else{
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
        }

        damage += Variance(-(damage/10)); // -10% to +10% dmg 
        
        if(target->m_LifePoint - damage <= 0){
            target->m_LifePoint = 0;
            std::cout << " The target is dead" << std::endl;
            return;
        }else{
            target->m_LifePoint -= damage;
        }

        std::cout << " BasicAttack dealed : " << damage << std::endl;
    }

    void Player::Fireball(ServerEntity *target){ // TODO : burning status

        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int damage;
        int critical = rand() % 100;
        
        if(critical > 90){
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) * RATIO;
            damage *= 2; // double the damage 
            std::cout << " CRITICAL !!! " << std::endl;
            //target is burned
        }else{
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) * RATIO;
        }

        damage += Variance(-(damage/10)); // -10% to +10% dmg 

        m_ManaPoint -= 5;
        std::cout << " Fireball dealed : " << damage << std::endl;

        if(target->m_LifePoint - damage <= 0){
            target->m_LifePoint = 0;
            std::cout << " The target is dead" << std::endl;
            return;
        }else{
            target->m_LifePoint -= damage;
        }

        critical = rand() % 100;
        if(critical > 70){
            // target burned
            std::cout << " The target is burned !!!" << std::endl;
        }
    }

    void Player::RangeUp(){ // Reset the boost x turn after the cast
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }
        m_ManaPoint -= 5;
        m_Range++;
        std::cout << " Range boosted by 1 point" << std::endl;
    }

    void Player::ArmorUp(){ // TODO : ending the boost after x turns
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int critical = rand() % 100;
        int defense;

        if(critical > 90){
            defense = m_DefensePoint / 2;
            m_DefensePoint += defense;
            std::cout << " CRITICAL !!! " << std::endl;
        }else{
            defense = m_DefensePoint / 4;
            m_DefensePoint += defense;
        }

        m_ManaPoint -= 5;
        std::cout << " ArmorUp boosted your defense by " << defense << " points" << std::endl;
    }


    void Player::DoubleStrike(ServerEntity *target){ // DONE
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int critical = rand() % 100;
        int damage1;
        int damage2;
        if(critical > 90){
            damage1 = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) / 1,5;
            damage1 += Variance(-(damage1/10)); // -10% to +10% dmg 
            target->m_LifePoint -= damage1;

            damage2 = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) / 1,5;
            damage2 += Variance(-(damage2/10)); // -10% to +10% dmg 

        }else{
            damage1 = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) / 1,7 ;
            damage1 += Variance(-(damage1/10)); // -10% to +10% dmg 
            target->m_LifePoint -= damage1;

            damage2 = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) / 1,7;
            damage2 += Variance(-(damage2/10)); // -10% to +10% dmg 
        }


        m_ManaPoint -= 5;
        std::cout << " DoubleStrike dealed " << damage1 << " then " << damage2 << std::endl;

        if(target->m_LifePoint - damage1 <= 0){
            target->m_LifePoint = 0;
            std::cout << " The target is dead" << std::endl;
            return;
        }else{
            target->m_LifePoint -= damage1;
        }

        if(target->m_LifePoint - damage2 <= 0){
            target->m_LifePoint = 0;
            std::cout << " The target is dead" << std::endl;
            return;
        }else{
            target->m_LifePoint -= damage2;
        }
    } 

    void Player::Heal(ServerEntity *target){ // DONE
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int critical = rand() % 100;
        int heal;
        if(critical > 90){
            heal = m_LifePoint / 5;
            heal += Variance(-(heal / 10));
            std::cout << " CRITICAL !!! " << std::endl;
        }else{
            heal = m_LifePoint / 10;
            heal += Variance(-(heal / 15));
        }

        if(target->m_LifePoint + heal > target->m_MaxLifePoint){
            target->m_LifePoint = target->m_MaxLifePoint;
        }else{
            target->m_LifePoint += heal;
        }

        m_ManaPoint -= 5;
        std::cout << " Healed by " << heal << "Hp" << std::endl;
    }

    void Player::Assasinate(ServerEntity *target){ // DONE
         if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int critical = rand() % 100;
        if(critical > 95){
            target->m_LifePoint = 0;
            std::cout << "The target has been assassinated" << std::endl;
        }else{
            target->m_LifePoint -= 10;
            if(target->m_LifePoint -10 < 0){
                target->m_LifePoint = 0 ;
            }else{
                target->m_LifePoint -= 10 ;
            }
            std::cout << "Assasination missed but still done some damage (10Hp)" << std::endl;
        }

        m_ManaPoint -= 5;
    }

    void Player::DamageUp(){  // TODO : turn 
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int critical = rand() % 100;
        int attack;
        if(critical > 90){
            attack = m_AttackPoint / 5;
            attack += Variance(-(m_AttackPoint / 15));
            m_AttackPoint += attack;
        }else{
            attack = m_AttackPoint / 10;
            attack += Variance(-(m_AttackPoint / 15));
            m_AttackPoint += attack;
        }

        m_ManaPoint -= 5;
        
        std::cout << " Gained " << attack << " AttackPoints"<< std::endl;
    }


    void Player::Protection(ServerEntity *target){ // segfault ?????
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }
        int critical = rand() % 100;
        int defense;
        if(critical > 90){
            defense = target->m_DefensePoint / 5;
            defense += Variance(-(target->m_DefensePoint / 15));
            target->m_DefensePoint += defense;
        }else{
            defense = target->m_DefensePoint / 10;
            defense += Variance(-(target->m_DefensePoint / 15));
            target->m_DefensePoint += defense;
        }

        m_ManaPoint -= 5;
        
        std::cout << "The target Gained " << defense << " DefensePoints"<< std::endl;
    }

    void Player::Revenge(ServerEntity *target){ // Need formula on missing health
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int critical = rand() % 100;
        int damage;
        int missingHealth = m_LifePoint * 100 / m_MaxLifePoint;
        std::cout << "missingHealth " << missingHealth << std::endl;
        if(critical > 90){
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) * (100 - (missingHealth));
            damage *= 2;
            damage += Variance(-(damage / 10));
        }else{ 
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) * missingHealth;
            damage += Variance(-(damage / 10));
        }
        if(target->m_LifePoint - damage < 0){
            target->m_LifePoint = 0;
        }else{
            target->m_LifePoint -= damage;
        }

        m_ManaPoint -= 5;
        std::cout << "Revenge dealed " << damage << std::endl;
    }

    void Player::Lacerate(ServerEntity *target){ // DONE
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int critical = rand() % 100;
        int damage;
        int currentHealth = (target->m_LifePoint / target->m_MaxLifePoint) * 2;
        if(critical > 90){
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) * currentHealth;
            damage *= 2;
            damage += Variance(-(damage / 10));
        }else{ 
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) * currentHealth;
            damage += Variance(-(damage / 10));
        }
        if(target->m_LifePoint - damage < 0){
            target->m_LifePoint = 0;
        }else{
            target->m_LifePoint -= damage;
        }

        m_ManaPoint -= 5;
        std::cout << "Lacerate dealed " << damage << std::endl;
    }

    void Player::Incinerate(ServerEntity *target){
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int critical = rand() % 100;
        int damage;
        int currentHealth = (target->m_LifePoint / target->m_MaxLifePoint) * 1.5;
        if(critical > 90){
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) * currentHealth;
            damage *= 2;
            damage += Variance(-(damage / 10));
            if(target->m_DefensePoint - 10 < 0){
                target->m_LifePoint = 0;
            }else{
                target->m_DefensePoint -= 10;
            }
        }else{ 
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) * currentHealth;
            damage += Variance(-(damage / 10));
            if(target->m_DefensePoint - 5 < 0){
                target->m_DefensePoint = 0;
            }else{
                target->m_DefensePoint -= 5;
            }
        }
        if(target->m_LifePoint - damage < 0){
            target->m_LifePoint = 0;
        }else{
            target->m_LifePoint -= damage;
        }

        m_ManaPoint -= 5;
        std::cout << "Incinerate dealed " << damage << std::endl;
    }

    void Player::Devastate(ServerEntity *target, int zone){
        // TODO get all foes in an area and attack them
    }

    void Player::Massacre(ServerEntity *target){ // WARRIOR ROGUE

        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }
        int critical = rand() % 100;
        int damage;
        int health;
        if(critical > 90){
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage *= 1.5;
            damage += Variance(-(damage / 10));
            health = damage / 2;
        }else{
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage += Variance(-(damage / 10));
            health = damage / 3;
        }

        if(target->m_LifePoint - damage < 0){
            target->m_LifePoint = 0;
        }else{
            target->m_LifePoint -= damage;
        }

        if(m_LifePoint + health > m_MaxLifePoint){
            m_LifePoint = m_MaxLifePoint;
        }else{
            m_LifePoint += health;
        }

        m_ManaPoint -= 5;

        std::cout << "Massacre dealed " << damage << " and healed you for " << health << std::endl;
    }


    void Player::Impact(ServerEntity *target,gf::SquareMap m_SquareWorld){ // WARRIOR
        // dash attack
        gf::Vector2i start = m_Pos;
        gf::Vector2i end = target->m_Pos;

        target->m_LifePoint -= (m_AttackPoint*(RATIO) - target->m_DefensePoint);
        std::vector<gf::Vector2i> dash = m_SquareWorld.computeRoute(start, end, 0.0); // first set of tile for the corridor

        if(dash.size() > 1){ // techniccaly we have already check if the field of vision is clear from the source to target so there won't be anyone on the path 
            m_Pos = dash[1]; // TODO check if correct
        }
    }

    void Player::LightningStrike(ServerEntity *target, int zone){
        // TODO get all foes in an area and attack them
    }


    void Player::Scorch(ServerEntity *target){ // TODO : bleeding

        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int damage;
        int critical = rand() % 100;
        if(critical > 95){ // critical hit
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage *= 2; // double the damage 
            damage += Variance(-(damage / 10));
            std::cout << " CRITICAL !!! " << std::endl;
        }else{
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage += Variance(-(damage / 10));
        }
        
        if(target->m_LifePoint - damage <= 0){
            target->m_LifePoint = 0;
            std::cout << " The target is dead" << std::endl;
            return;
        }else{
            target->m_LifePoint -= damage;
        }
        m_ManaPoint -= 5;

         std::cout << " Scorch dealed : " << damage << std::endl;
    }

    void Player::Berserk(){ // WARRIOR
        int critical = rand() % 100;
        if(critical > 90){
            m_AttackPoint += 5;
        }else{
            m_AttackPoint += 5;
            if(m_DefensePoint - 10 < 0){
                m_DefensePoint = 0;
            }else{
                m_DefensePoint -= 10;
            }
        }
    }

    void Player::Cleanse(){
        // TODO cleanse
    }

    void Player::Mirror(){
        // TODO Mirror
    }

    void Player::Stun(ServerEntity *target){
        // TODO stun for x turn
    }

    void Player::Warp(gf::Vector2i dash){
        m_Pos = dash;
    }

    void Player::Shoot(ServerEntity *target){
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }
        int damage;
        int critical = rand() % 100;
        if(critical > 90){   
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage *= 2;
            damage += Variance(-(damage / 10));
            target->m_DefensePoint -= 5;
        }else{  
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage += Variance(-(damage / 10));
        }

        if(target->m_LifePoint - damage < 0){
            target->m_LifePoint = 0;
        }else{
            target->m_LifePoint -= damage;
        }

        m_ManaPoint -=5;

        std::cout << "Shoot dealed " << damage << std::endl;
    }

    void Player::Backstab(ServerEntity *target){
        
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }
        int damage;
        int critical = rand() % 100;
        if(critical > 90){   
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage *= 2;
            damage += Variance(-(damage / 10));
            m_AttackPoint += m_MaxAttackPoint / 10;
        }else{  
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage += Variance(-(damage / 10));
            m_AttackPoint += m_MaxAttackPoint / 5;
        }

        if(target->m_LifePoint - damage < 0){
            target->m_LifePoint = 0;
        }else{
            target->m_LifePoint -= damage;
        }
        m_ManaPoint -= 5;
        std::cout << "Backstab dealed " << damage << std::endl;

    }


    void Player::Energize(){
    }

    void Player::Torpedo(ServerEntity *target){ // RANGER DONE

        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }
        int damage;
        int critical = rand() % 100;
        int missingHealth = m_LifePoint * 100 / m_MaxLifePoint;
        if(critical > 90){   
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) * missingHealth;
            damage *= 2;
            damage += Variance(-(damage / 10));
        }else{  
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) * missingHealth;
            damage += Variance(-(damage / 10));
        }

        if(target->m_LifePoint - damage < 0){
            target->m_LifePoint = 0;
        }else{
            target->m_LifePoint -= damage;
        }

        m_ManaPoint -= 5;
        std::cout << "Torpedo dealed " << damage << std::endl;
    }

    void Player::SoulLink(ServerEntity *target){
        // TODO
    }


    void Player::Reaper(ServerEntity *target){

    }
}
