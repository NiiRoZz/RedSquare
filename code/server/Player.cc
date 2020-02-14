#include "Player.h"
#include "RedsquareInstance.h"
#include "RedsquareInstance.h"

#include <iostream>

#include "../common/Constants.h"

namespace redsquare
{
    Player::Player(gf::Id playerID, const EntitySubType entitySubType, std::string name, RedsquareInstance &instance)
    : ServerEntity(playerID, EntityType::Player, entitySubType)
    , m_PlayerTurn(false)
    , m_RedsquareInstance(instance)
    , m_Name(name)
    {
        switch (m_EntitySubType){
        case EntitySubType::Magus : // ------------------- Magus -------------------
            m_Class = EntitySubType::Magus;
            m_LifePoint = MAGUS_LIFEPOINT;
            m_ManaPoint = MAGUS_MANAPOINT;

            m_MaxLifePoint = MAGUS_MAXLIFEPOINT;
            m_MaxManaPoint = MAGUS_MAXMANAPOINT;

            m_AttackPoint = MAGUS_ATTACKPOINT;
            m_DefensePoint = MAGUS_DEFENSEPOINT;

            m_MaxAttackPoint = MAGUS_MAXATTACKPOINT;
            m_MaxDefensePoint = MAGUS_MAXDEFENSEPOINT;

            m_Range = MAGUS_RANGE;
            break;

        case EntitySubType::Warrior : // ------------------- Warrior -------------------
            m_Class = EntitySubType::Warrior;
            m_LifePoint = WARRIOR_LIFEPOINT;
            m_ManaPoint = WARRIOR_MANAPOINT;

            m_MaxLifePoint = WARRIOR_MAXLIFEPOINT;
            m_MaxManaPoint = WARRIOR_MAXMANAPOINT;

            m_AttackPoint = WARRIOR_ATTACKPOINT;
            m_DefensePoint = WARRIOR_DEFENSEPOINT;

            m_MaxAttackPoint = WARRIOR_MAXATTACKPOINT;
            m_MaxDefensePoint = WARRIOR_MAXDEFENSEPOINT;

            m_Range = WARRIOR_RANGE;
            break;

        case EntitySubType::Rogue : // ------------------- Rogue -------------------
            m_Class = EntitySubType::Rogue;
            m_LifePoint = ROGUE_LIFEPOINT;
            m_ManaPoint = ROGUE_MANAPOINT;

            m_MaxLifePoint = ROGUE_MAXLIFEPOINT;
            m_MaxManaPoint = ROGUE_MAXMANAPOINT;

            m_AttackPoint = ROGUE_ATTACKPOINT;
            m_DefensePoint = ROGUE_DEFENSEPOINT;

            m_MaxAttackPoint = ROGUE_MAXATTACKPOINT;
            m_MaxDefensePoint = ROGUE_MAXDEFENSEPOINT;

            m_Range = ROGUE_RANGE;
            break;

        case EntitySubType::Ranger : // ------------------- Rogue -------------------
            m_Class = EntitySubType::Ranger;
            m_LifePoint = RANGER_LIFEPOINT;
            m_ManaPoint = RANGER_MANAPOINT;

            m_MaxLifePoint = RANGER_MAXLIFEPOINT;
            m_MaxManaPoint = RANGER_MAXMANAPOINT;

            m_AttackPoint = RANGER_ATTACKPOINT;
            m_DefensePoint = RANGER_DEFENSEPOINT;

            m_MaxAttackPoint = RANGER_MAXATTACKPOINT;
            m_MaxDefensePoint = RANGER_MAXDEFENSEPOINT;

            m_Range = RANGER_RANGE;
            break;

        case EntitySubType::Healer : // ------------------- Healer -------------------
            m_Class = EntitySubType::Healer;
            m_LifePoint = HEALER_LIFEPOINT;
            m_ManaPoint = HEALER_MANAPOINT;

            m_MaxLifePoint = HEALER_MAXLIFEPOINT;
            m_MaxManaPoint = HEALER_MAXMANAPOINT;

            m_AttackPoint = HEALER_ATTACKPOINT;
            m_DefensePoint = HEALER_DEFENSEPOINT;

            m_MaxAttackPoint = HEALER_MAXATTACKPOINT;
            m_MaxDefensePoint = HEALER_MAXDEFENSEPOINT;

            m_Range = HEALER_RANGE;
            break;

        default:
            break;
        }
        m_XP = 0;
        m_MaxXP = 15;

        m_Level = 1;
        m_MovedInRound = false;
        m_SpellTab.push_back(SpellType::BasicAttack);
    }

    bool Player::applyMove(gf::Vector2i dir, World &world)
    {
        int newPosX = m_Pos[0] + dir[0];
        int newPosY = m_Pos[1] + dir[1];

        if ( newPosY >= 0 && newPosY < World::MapSize-1 && newPosX >= 0 && newPosX < World::MapSize-1 && world.m_SquareWorld.isWalkable( {newPosX, newPosY} ) )
        {
            world.setWalkableFromEntity(static_cast<redsquare::Entity*>(this), true);
            world.setTransparentFromEntity( static_cast<redsquare::Entity*>(this), true );

            m_Pos += dir;

            world.setWalkableFromEntity(static_cast<redsquare::Entity*>(this), false);
            world.setTransparentFromEntity( static_cast<redsquare::Entity*>(this), false );

            return true;
        }
        else
        {
            return false;
        }

        return false;
    }
    
    void Player::defaultInventoryStuff()
    {
        switch (m_EntitySubType)
        {
            case EntitySubType::Magus:{
                //Example how to spawn item in weapon slot
                ServerItem item1(ItemType::Staff1, 1u);
                ssize_t pos = m_Inventory.addItem(InventorySlotType::Weapon, std::move(item1));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Weapon, false, pos));
                }

                //Example how to spawn item in cargo slot
                ServerItem item2(ItemType::HealthPot1, 1u);
                pos = m_Inventory.addItem(InventorySlotType::Cargo, std::move(item2));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                }
                //Example how to spawn item in cargo slot
                ServerItem item3(ItemType::HealthPot2, 1u);
                pos = m_Inventory.addItem(InventorySlotType::Cargo, std::move(item3));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                }

                //Example how to spawn item in cargo slot
                ServerItem item4(ItemType::HealthPot3, 1u);
                pos = m_Inventory.addItem(InventorySlotType::Cargo, std::move(item4));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                }

                //Example how to spawn item in cargo slot
                ServerItem item5(ItemType::ManaPot1, 1u);
                pos = m_Inventory.addItem(InventorySlotType::Cargo, std::move(item5));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                }
                //Example how to spawn item in cargo slot
                ServerItem item6(ItemType::ManaPot2, 1u);
                pos = m_Inventory.addItem(InventorySlotType::Cargo, std::move(item6));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                }

                //Example how to spawn item in cargo slot
                ServerItem item7(ItemType::ManaPot3, 1u);
                pos = m_Inventory.addItem(InventorySlotType::Cargo, std::move(item7));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                }
                
                //Example how to spawn item in cargo slot
                ServerItem item8(ItemType::BoostDefense1, 1u);
                pos = m_Inventory.addItem(InventorySlotType::Cargo, std::move(item8));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                }
                ServerItem item9(ItemType::BoostDefense2, 1u);
                pos = m_Inventory.addItem(InventorySlotType::Cargo, std::move(item9));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                }
                ServerItem item10(ItemType::BoostDefense3, 1u);
                pos = m_Inventory.addItem(InventorySlotType::Cargo, std::move(item10));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                }
                break;
            }
            case EntitySubType::Warrior:  case EntitySubType::Rogue:{
                ServerItem item1(ItemType::Sword1, 1u);
                ssize_t pos = m_Inventory.addItem(InventorySlotType::Weapon, std::move(item1));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Weapon, false, pos));
                }
                break;
            }
            case EntitySubType::Ranger:{
                ServerItem item1(ItemType::Bow1, 1u);
                ssize_t pos = m_Inventory.addItem(InventorySlotType::Weapon, std::move(item1));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Weapon, false, pos));
                }
                break;
            }
            case EntitySubType::Healer:{
                ServerItem item1(ItemType::SpellBook1, 1u);
                ssize_t pos = m_Inventory.addItem(InventorySlotType::Weapon, std::move(item1));
                if (pos != -1)
                {
                    m_RedsquareInstance.send(m_EntityID, createUpdateItemPacket(InventorySlotType::Weapon, false, pos));
                }
                break;
            }
                
        }
        
    }

    void Player::levelUp(){ // method to level up a player

        m_MaxLifePoint += 2;
        m_LifePoint = m_MaxLifePoint;

        m_MaxManaPoint += 2;
        m_ManaPoint = m_MaxManaPoint;

        m_AttackPoint += 2;
        m_DefensePoint += 2;

        m_XP = 0;
        m_MaxXP += 30;

        m_Level++;

    
        switch (m_Class)
        {
            case EntitySubType::Warrior:  
                if(m_Level == 2){
                    m_SpellTab.push_back(SpellType::DamageUp); // gain damage 
                }else if(m_Level == 3){
                    m_SpellTab.push_back(SpellType::Massacre); // lifesteal attack
                }else if(m_Level == 4){
                    m_SpellTab.push_back(SpellType::Berserk); // gain damage and defense
                }else if(m_Level == 5){
                    m_SpellTab.push_back(SpellType::Revenge); // more damage the less life you have
                }else if(m_Level == 6){
                    m_SpellTab.push_back(SpellType::Reaper); // damage in front of you 
                }else if(m_Level == 7){
                    m_SpellTab.push_back(SpellType::Protection); // give armor to target
                }else if(m_Level == 8){
                   m_SpellTab.push_back(SpellType::ArmorUp); // gain armor
                }
                break;

            case EntitySubType::Magus:
                if(m_Level == 2){
                    m_SpellTab.push_back(SpellType::FireBall); // damage
                }else if(m_Level == 3){
                   m_SpellTab.push_back(SpellType::Incinerate); // more damage the more life target has
                }else if(m_Level == 4){
                    m_SpellTab.push_back(SpellType::LightningStrike); // AOE
                }else if(m_Level == 5){
                    m_SpellTab.push_back(SpellType::Heal); // heal
                }else if(m_Level == 6){
                    m_SpellTab.push_back(SpellType::Scorch); // damage
                }else if(m_Level == 7){
                    m_SpellTab.push_back(SpellType::DamageUp); // gain damage 
                }else if(m_Level == 8){
                    m_SpellTab.push_back(SpellType::Revenge); // more damage the less life you have
                }
                break;

            case EntitySubType::Rogue:  
                if(m_Level == 2){
                    m_SpellTab.push_back(SpellType::Lacerate); // the more life the target has the more damage it deal
                }else if(m_Level == 4){
                    m_SpellTab.push_back(SpellType::Berserk); // gain damage and defense
                }else if(m_Level == 5){
                    m_SpellTab.push_back(SpellType::Scorch); // damage
                }else if(m_Level == 6){
                    m_SpellTab.push_back(SpellType::DoubleStrike); // double attack 
                }else if(m_Level == 7){
                    m_SpellTab.push_back(SpellType::DamageUp); // gain damage
                }else if(m_Level == 8){
                    m_SpellTab.push_back(SpellType::Massacre); // lifesteal attack
                }
                break;

            case EntitySubType::Ranger:  
                if(m_Level == 2){
                    m_SpellTab.push_back(SpellType::Shoot);
                }else if(m_Level == 3){
                    //m_SpellTab.push_back(SpellType::RangeUp); too op for now
                    m_SpellTab.push_back(SpellType::Berserk); // gain damage and defense
                }else if(m_Level == 4){
                    m_SpellTab.push_back(SpellType::Torpedo);
                }else if(m_Level == 5){
                    m_SpellTab.push_back(SpellType::Scorch);
                }else if(m_Level == 6){
                    m_SpellTab.push_back(SpellType::Revenge); // more damage the less life you have
                }else if(m_Level == 7){
                    m_SpellTab.push_back(SpellType::DamageUp); // gain damage
                }else if(m_Level == 8){
                    m_SpellTab.push_back(SpellType::LightningStrike); // AOE
                }
                break;

            case EntitySubType::Healer:  
                if(m_Level == 2){
                    m_SpellTab.push_back(SpellType::Heal);
                }else if(m_Level == 3){
                    m_SpellTab.push_back(SpellType::FireBall);
                }else if(m_Level == 4){
                    m_SpellTab.push_back(SpellType::Protection);
                }else if(m_Level == 5){
                    m_SpellTab.push_back(SpellType::DamageUp);
                }else if(m_Level == 6){
                   m_SpellTab.push_back(SpellType::ArmorUp);
                }else if(m_Level == 7){
                   m_SpellTab.push_back(SpellType::Incinerate);
                }
                break;

            default: 
                break;
        }
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

        m_Inventory.setOwner(this);
    }

    void Player::attack(SpellType spellType, ServerEntity *target){
        m_PointInRound = 0;

        switch (spellType)
        {
            case SpellType::BasicAttack:
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
            case SpellType::Heal:
                Heal(target);
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

    std::vector<Monster*> Player::attack(SpellType spellType, ServerEntity *target,std::map<gf::Id, Monster> &monsters)
    {
        m_PointInRound = 0;

        switch (spellType){
            case SpellType::Reaper:
                return Reaper(target,monsters);
                break;
            case SpellType::LightningStrike:
                return LightningStrike(target,monsters);
                break;
        }
        std::vector<Monster*> empty;
        return empty;
    }

    int Player::Variance(int range){ // adding some rng to the damage of a spell [range;+range] added to the base damage of the spell
        if(range == 0){
            return 0;
        }
        int randomNum = rand() % (range*2) + (range);
        return randomNum;
    }

    void Player::BasicAttack(ServerEntity *target){ // DONE

        int damage;
        int critical = rand() % 100;
        if(critical > 95){ // critical hit
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage *= 2; // double the damage 
            //unique
            std::cout << " CRITICAL !!! " << std::endl;
            createSystemMessage(" CRITICAL !!! ",m_Name);

            

        }else{
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
        }

        damage += Variance(-(damage/10)); // -10% to +10% dmg 
        
        if(target->m_LifePoint - damage <= 0){
            target->m_LifePoint = 0;
            //multiple
            std::cout << " BasicAttack dealed : " << damage << std::endl;
            createSystemMessage(" BasicAttack dealed : ","system");
            std::cout << " The target is dead" << std::endl;
            createSystemMessage( " The target is dead","system");
            return;
        }else{
            target->m_LifePoint -= damage;
        }
        //multiple
        std::cout << " BasicAttack dealed : " << damage << std::endl;
        createSystemMessage(  "BasicAttack dealed : ","system");
    }

    void Player::Fireball(ServerEntity *target){

        if(m_ManaPoint < 5){
            //unique
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int damage;
        int critical = rand() % 100;
        
        if(critical > 90){
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage *= 2; // double the damage 
            //unique
            std::cout << " CRITICAL !!! " << std::endl;
        }else{
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
        }

        damage += Variance(-(damage/10)); // -10% to +10% dmg 

        m_ManaPoint -= 5;
        //multiple
        std::cout << " Fireball dealed : " << damage << std::endl;

        if(target->m_LifePoint - damage <= 0){
            target->m_LifePoint = 0;
            //multiple
            std::cout << " The target is dead" << std::endl;
            return;
        }else{
            target->m_LifePoint -= damage;
        }
    }

    void Player::RangeUp(){ // Reset the boost x turn after the cast
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }
        m_ManaPoint -= 5;
        m_Range++;
        //unique
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
            damage1 = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) / 1.5;
            damage1 += Variance(-(damage1/10)); // -10% to +10% dmg 
            target->m_LifePoint -= damage1;

            damage2 = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) / 1.5;
            damage2 += Variance(-(damage2/10)); // -10% to +10% dmg 

        }else{
            damage1 = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) / 1.7 ;
            damage1 += Variance(-(damage1/10)); // -10% to +10% dmg 
            target->m_LifePoint -= damage1;

            damage2 = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint) / 1.7;
            damage2 += Variance(-(damage2/10)); // -10% to +10% dmg 
        }


        m_ManaPoint -= 5;
        //multiple
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

        if(heal < 0 ){
            heal = -heal;
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
            m_MaxAttackPoint += attack;
        }else{
            attack = m_AttackPoint / 10;
            attack += Variance(-(m_AttackPoint / 15));
            m_AttackPoint += attack;
            m_MaxAttackPoint += attack;
        }

        m_ManaPoint -= 5;
        
        std::cout << " Gained " << attack << " AttackPoints"<< std::endl;
    }


    void Player::Protection(ServerEntity *target){ // WARRIOR
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

    void Player::Revenge(ServerEntity *target){ // WARRIOR ROGUE
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int critical = rand() % 100;
        int damage;
        int missingHealth = (m_LifePoint*2)/ m_MaxLifePoint;
        //std::cout << "missingHealth " << missingHealth << std::endl;
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
        std::cout << "Revenge dealed " << damage << std::endl;
    }

    void Player::Lacerate(ServerEntity *target){ // DONE
        if(m_ManaPoint < 5){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }

        int critical = rand() % 100;
        int damage;
        double currentHealth = (target->m_LifePoint / target->m_MaxLifePoint) * 2;
        if(currentHealth == 0){
            currentHealth = 0.5;
        }
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
        double currentHealth = (target->m_LifePoint / target->m_MaxLifePoint) * 1.5;
        if(currentHealth == 0){
            currentHealth = 0.5;
        }
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

    std::vector<Monster*> Player::LightningStrike(ServerEntity *target,std::map<gf::Id, Monster> &monsters){
        /* example of use:
             X
            XOX
             X

            O: Player
            X: Ennemy     60% of a basic attack damage
        */

        std::vector<Monster*> allPacket;
        if(m_ManaPoint < 7){
            std::cout << "NOT ENOUGH MANA" << std::endl;

            return allPacket;
        }

        gf::Vector2i mainPos = target->m_Pos; // pos of the main target
        //std::cout << mainPos[0] << ":" << mainPos[1] << std::endl;
        /* adjacent tile */
        gf::Vector2i Pos1{ mainPos[0]+1 , mainPos[1]}; 
        gf::Vector2i Pos2{ mainPos[0]-1 , mainPos[1]};
        gf::Vector2i Pos3{ mainPos[0] , mainPos[1]+1};
        gf::Vector2i Pos4{ mainPos[0] , mainPos[1]-1};
        /* adjacent tile */

        int damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint)*0.7;

        auto it = monsters.begin();
        while(it != monsters.end()){
            if(it->second.m_Pos == Pos1){
                damage += Variance(-(damage / 10));
                if(it->second.m_LifePoint - damage < 0){
                    
                    it->second.m_LifePoint = 0;
                    m_XP += target->m_Level*10;
                    if(m_XP >= m_MaxXP){
                        levelUp();
                    }
                }else{
                    it->second.m_LifePoint -= damage;
                }

                allPacket.push_back(&it->second);
                std::cout << "LightningStrike dealed " << damage << " to the target" << std::endl;
            }else if(it->second.m_Pos == Pos2){
                damage += Variance(-(damage / 10));
                if(it->second.m_LifePoint - damage < 0){
                    
                    it->second.m_LifePoint = 0;
                    m_XP += target->m_Level*10;
                    if(m_XP >= m_MaxXP){
                        levelUp();
                    }
                }else{
                    it->second.m_LifePoint -= damage;
                }

                allPacket.push_back(&it->second);
                std::cout << "LightningStrike dealed " << damage << " to the target" << std::endl;
            }else if(it->second.m_Pos == Pos3){
                damage += Variance(-(damage / 10));
                if(it->second.m_LifePoint - damage < 0){
                    
                    it->second.m_LifePoint = 0;
                    m_XP += target->m_Level*10;
                    if(m_XP >= m_MaxXP){
                        levelUp();
                    }
                }else{
                    it->second.m_LifePoint -= damage;
                }

                allPacket.push_back(&it->second);
                std::cout << "LightningStrike dealed " << damage << " to the target" << std::endl;
            }else if(it->second.m_Pos == Pos4){
                damage += Variance(-(damage / 10));
                if(it->second.m_LifePoint - damage < 0){
                    
                    it->second.m_LifePoint = 0;
                    m_XP += target->m_Level*10;
                    if(m_XP >= m_MaxXP){
                        levelUp();
                    }
                }else{
                    it->second.m_LifePoint -= damage;
                }

                allPacket.push_back(&it->second);
                std::cout << "LightningStrike dealed " << damage << " to the target" << std::endl;
            
            }
            it++;
        }
        m_ManaPoint -= 7;
        std::cout << allPacket.size() << std::endl;
        return allPacket;
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
        if(m_ManaPoint < 7){
            std::cout << "NOT ENOUGH MANA" << std::endl;
            return;
        }
        int critical = rand() % 100;
        if(critical > 90){
            m_AttackPoint += 7;
            m_MaxAttackPoint += 7;
            m_DefensePoint += 7;
            m_MaxDefensePoint += 7;
            std::cout << "+7 attack and defense point" <<  std::endl;
        }else{
            m_AttackPoint += 5;
            m_MaxAttackPoint += 5;
            m_DefensePoint += 5;
            m_MaxDefensePoint += 5;
            std::cout << "+5 attack and defense point" <<  std::endl;
        }

        m_ManaPoint -= 7;
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
            m_MaxAttackPoint += m_MaxAttackPoint / 10;
        }else{  
            damage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint);
            damage += Variance(-(damage / 10));
            m_AttackPoint += m_MaxAttackPoint / 5;
            m_MaxAttackPoint += m_MaxAttackPoint / 5;
        }

        if(target->m_LifePoint - damage < 0){
            target->m_LifePoint = 0;
        }else{
            target->m_LifePoint -= damage;
        }
        m_ManaPoint -= 5;
        std::cout << "Backstab dealed " << damage << std::endl;

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

    std::vector<Monster*> Player::Reaper(ServerEntity *target,std::map<gf::Id, Monster> &monsters){ // deal damage to the 3 tile in front of user 

        /* Example of use:

            +O+      +       +      X
             X       OX     XO     +O+
                     +       + 

             X: Player
             O: Main target      70% of a basic attack damage
             +: Other monster   60% of a basic attack damage
        */

        std::vector<Monster*> allPacket;
        if(m_ManaPoint < 7){
            std::cout << "NOT ENOUGH MANA" << std::endl;

            return allPacket;
        }

        gf::Vector2i mainPos = target->m_Pos; // pos of the main target

        /* adjacent tile */
        gf::Vector2i Pos1{ mainPos[0]+1 , mainPos[1]}; 
        gf::Vector2i Pos2{ mainPos[0]-1 , mainPos[1]};
        gf::Vector2i Pos3{ mainPos[0] , mainPos[1]+1};
        gf::Vector2i Pos4{ mainPos[0] , mainPos[1]-1};
        /* adjacent tile */

        int mainDamage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + target->m_DefensePoint)*0.7;
        mainDamage += Variance(-(mainDamage / 10));
        int sideDamage;

        std::cout << "Reaper dealed " << mainDamage << " to the main target" << std::endl;

        auto it = monsters.begin();
        while(it != monsters.end()){
            if(it->second.m_Pos == Pos1){
                if(m_Pos != Pos2){ // avoid dealing damage to the tile behind the main target
                    sideDamage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + it->second.m_DefensePoint) * 0.6;
                    sideDamage += Variance(-(sideDamage / 10));

                    if(it->second.m_LifePoint - sideDamage < 0){
                        
                        it->second.m_LifePoint = 0;
                        m_XP += target->m_Level*10;
                        if(m_XP >= m_MaxXP){
                            levelUp();
                        }
                    }else{
                        it->second.m_LifePoint -= sideDamage;
                    }

                    allPacket.push_back(&it->second);
                    std::cout << "Reaper dealed " << sideDamage << " to the side target" << std::endl;
                }
            }else if(it->second.m_Pos == Pos2){
                if(m_Pos != Pos1){ // avoid dealing damage to the tile behind the main target
                    sideDamage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + it->second.m_DefensePoint) * 0.6;
                    sideDamage += Variance(-(sideDamage / 10));

                    if(it->second.m_LifePoint - sideDamage < 0){
                        it->second.m_LifePoint = 0;
                        m_XP += target->m_Level*10;
                        if(m_XP >= m_MaxXP){
                            levelUp();
                        }
                    }else{
                        it->second.m_LifePoint -= sideDamage;
                    }

                    allPacket.push_back(&it->second);
                    std::cout << "Reaper dealed " << sideDamage << " to the side target" << std::endl;
                }
            }else if(it->second.m_Pos == Pos3){
                if(m_Pos != Pos4){ // avoid dealing damage to the tile behind the main target
                    sideDamage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + it->second.m_DefensePoint) * 0.6;
                    sideDamage += Variance(-(sideDamage / 10));

                    if(it->second.m_LifePoint - sideDamage < 0){
                        it->second.m_LifePoint = 0;
                        m_XP += target->m_Level*10;
                        if(m_XP >= m_MaxXP){
                            levelUp();
                        }
                    }else{
                        it->second.m_LifePoint -= sideDamage;
                    }
                    allPacket.push_back(&it->second);
                    std::cout << "Reaper dealed " << sideDamage << " to the side target" << std::endl;
                }
            }else if(it->second.m_Pos == Pos4){
                if(m_Pos != Pos3){ // avoid dealing damage to the tile behind the main target
                    sideDamage = (m_AttackPoint*m_AttackPoint / m_AttackPoint + it->second.m_DefensePoint) * 0.6;
                    sideDamage += Variance(-(sideDamage / 10));

                    if(it->second.m_LifePoint - sideDamage < 0){
                        it->second.m_LifePoint = 0;
                        m_XP += target->m_Level*10;
                        if(m_XP >= m_MaxXP){
                            levelUp();
                        }
                    }else{
                        it->second.m_LifePoint -= sideDamage;
                    }
                    allPacket.push_back(&it->second);
                    std::cout << "Reaper dealed " << sideDamage << " to the side target" << std::endl;
                }
            }
            /* main target */ // need to be done here to obtain targetMonster value
            if(it->second.m_Pos == target->m_Pos){
                Monster targetMonster(it->second);

                if(target->m_LifePoint - mainDamage < 0){
                    target->m_LifePoint = 0;
                    m_XP += target->m_Level*10;
                    if(m_XP >= m_MaxXP){
                        levelUp();
                    }
                }else{
                    target->m_LifePoint -= mainDamage;
                }
                allPacket.push_back(&it->second);
            }
            /* main target */
            it++;
        }


        m_ManaPoint -= 7;

        return allPacket;
    }

    void Player::UseItem(ItemType type){
        switch (type)
        {
        case ItemType::HealthPot1:
            HealthPot(POTRATIO1);
            break;
        case ItemType::HealthPot2:
            HealthPot(POTRATIO2);
            break;
        case ItemType::HealthPot3:
            HealthPot(POTRATIO3);
            break;

        case ItemType::ManaPot1:
            ManaPot(POTRATIO1);
            break;
        case ItemType::ManaPot2:
            ManaPot(POTRATIO2);
            break;
        case ItemType::ManaPot3:
            ManaPot(POTRATIO3);
            break;

        case ItemType::EnergyPot1:
            EnergyPot(POTRATIO1);
            break;
        case ItemType::EnergyPot2:
            EnergyPot(POTRATIO2);
            break;
        case ItemType::EnergyPot3:
            EnergyPot(POTRATIO3);
            break;

        case ItemType::BoostAttack1:
            BoostAttack(BOOSTRATIO1);
            break;
        case ItemType::BoostAttack2:
            BoostAttack(BOOSTRATIO2);
            break;
        case ItemType::BoostAttack3:
            BoostAttack(BOOSTRATIO3);
            break;

        case ItemType::BoostDefense1:
            BoostDefense(BOOSTRATIO1);
            break;
        case ItemType::BoostDefense2:
            BoostDefense(BOOSTRATIO2);
            break;
        case ItemType::BoostDefense3:
            BoostDefense(BOOSTRATIO3);
            break;

        case ItemType::BoostHP1:
            BoostHealth(BOOSTRATIO1);
            break;
        case ItemType::BoostHP2:
            BoostHealth(BOOSTRATIO2);
            break;
        case ItemType::BoostHP3:
            BoostHealth(BOOSTRATIO3);
            break;

        case ItemType::BoostMana1:
            BoostMana(BOOSTRATIO1);
            break;
        case ItemType::BoostMana2:
            BoostMana(BOOSTRATIO2);
            break;
        case ItemType::BoostMana3:
            BoostMana(BOOSTRATIO3);
            break;

        case ItemType::BoostXP1:
            BoostXP(BOOSTRATIO1);
            break;
        case ItemType::BoostXP2:
            BoostXP(BOOSTRATIO2);
            break;
        case ItemType::BoostXP3:
            BoostXP(BOOSTRATIO3);
            break;

        default:
            break;
        }
    }


    void Player::ManaPot(float ratio){
        int mana = (m_MaxManaPoint*ratio);
        if(m_ManaPoint + mana > m_MaxManaPoint){
            m_ManaPoint = m_MaxManaPoint;
        }else{
            m_ManaPoint += mana;
        }
        std::cout << " m_ManaPoint +" << mana << std::endl;
    }
    void Player::HealthPot(float ratio){
        int health = (m_MaxLifePoint*ratio);
        if(m_LifePoint + health > m_MaxLifePoint){
            m_LifePoint = m_MaxLifePoint;
        }else{
            m_LifePoint += health;
        }
        std::cout << " m_LifePoint +" << health << std::endl;
    }
    void Player::EnergyPot(float ratio){
        ManaPot(ratio);
        HealthPot(ratio);
    }
    void Player::BoostDefense(float ratio){
        int defense = (m_MaxDefensePoint*ratio);
        m_MaxDefensePoint += defense; 
        m_DefensePoint = m_MaxDefensePoint;
        std::cout << " m_MaxDefensePoint +" << defense << std::endl;
    }
    void Player::BoostAttack(float ratio){
        int attack = (m_MaxAttackPoint*ratio);
        m_MaxAttackPoint += attack;
        m_AttackPoint = m_MaxAttackPoint;
        std::cout << " m_MaxAttackPoint +" << attack << std::endl;
    }
    void Player::BoostXP(float ratio){
        int XP = (m_MaxXP*ratio);
        m_XP += XP;
        if(m_XP > m_MaxXP){
            levelUp();
        }
        std::cout << " m_XP +" << XP << std::endl;
    }
    void Player::BoostMana(float ratio){
        int mana = (m_MaxManaPoint*ratio);
        m_MaxManaPoint += mana; 
        m_ManaPoint = m_MaxManaPoint;
        std::cout << " m_MaxManaPoint +" << mana << std::endl;
    }
    void Player::BoostHealth(float ratio){
        int health = (m_MaxLifePoint*ratio);
        m_MaxLifePoint += health; 
        m_LifePoint = m_MaxLifePoint;
        std::cout << " m_MaxLifePoint +" << health << std::endl;
    }

    /*void Player::sendMessageToChat(std::string str){
        Message mess;
        int length = str.copy(mess.message,str.length());
        mess.message[length]='\0';
        m_Socket.send(mess);
    }*/

    void Player::createSystemMessage(std::string message, std::string to){
            Message packet;
            int length=0;
            std::string sys("system");
            length = message.copy(packet.message, message.length());
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
            //Chat::getInstance().sendMessage(packet);  
    }
}
