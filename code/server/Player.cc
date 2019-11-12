#include "Player.h"
#include <iostream>

#define RATIO 1.1
#define LEVELSPELL 1

namespace redsquare
{
    Player::Player(SocketTcp socket, gf::Id playerID, const EntityClass type)
    : ServerEntity(playerID,type)
    , m_Socket(std::move(socket))
    {
        switch (m_TypeOfEntity){
        case EntityClass::Magus : // ------------------- Magus -------------------
            m_Class = EntityClass::Magus;
            m_LifePoint = 100;
            m_ManaPoint = 20;

            m_MaxLifePoint = 100;
            m_MaxManaPoint = 20;

            m_AttackPoint = 30;
            m_DefensePoint = 0;

            m_MaxAttackPoint = 30;
            m_MaxDefensePoint = 0;

            m_Range = 2;

            m_SpellTab.push_back(SpellType::BasicAttack);
            m_SpellTab.push_back(SpellType::FireBall);
            break;

        case EntityClass::Warrior : // ------------------- Warrior -------------------
            m_Class = EntityClass::Warrior;
            m_LifePoint = 150;
            m_ManaPoint = 5;

            m_MaxLifePoint = 150;
            m_MaxManaPoint = 5;

            m_AttackPoint = 20;
            m_DefensePoint = 0;

            m_MaxAttackPoint = 20;
            m_MaxDefensePoint = 0;

            m_Range = 1;

            m_SpellTab.push_back(SpellType::BasicAttack);
            m_SpellTab.push_back(SpellType::Berserk);
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
            world.m_SquareWorld.setWalkable(m_Pos);
            world.m_SquareWorld.setTransparent(m_Pos);
            m_Pos[1] = newPosY;
            m_Pos[0] = newPosX;

            world.setUnWalkable(m_Pos);

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
        m_LifePoint += 2;

        m_MaxManaPoint += 2;
        m_ManaPoint += 2;

        m_AttackPoint += 2;
        m_DefensePoint += 2;

        m_XP = 0;
        m_MaxXP += 20;

        m_Level++;
        if(m_Level == 2){ // adding spell when level is 2
            if(m_Class == EntityClass::Warrior){ // TODO for all class and with more level or maybe give choice to player
                m_SpellTab.push_back(SpellType::Revenge);
            }else if(m_Class == EntityClass::Magus){
                m_SpellTab.push_back(SpellType::LightningStrike);
            }
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

    void Player::playerSpawn(std::map<gf::Id,Player> &m_Players,World &world){ // set spawn for a player 
        switch (m_Players.size()) // TODO GLOBAL VARIABLE
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

    void Player::attack(ServerEntity *target)
    {
        m_PointInRound -= 1;

        target->m_LifePoint -= (m_AttackPoint - target->m_DefensePoint);
        if(target->m_LifePoint <= 0){
            m_XP += 10;
            if(m_XP >= m_MaxXP){
                levelUp();
            }
        }
    }

    void Player::BasicAttack(ServerEntity *target){
        target->m_LifePoint -= (m_AttackPoint - target->m_DefensePoint);
    }

    void Player::Fireball(ServerEntity *target){
        target->m_LifePoint -= (m_AttackPoint*(RATIO) - target->m_DefensePoint);
        // TODO burn state
    }

    void Player::ArmorUp(){
        m_DefensePoint += 2; // TODO update ration by lvl of source
    }


    void Player::DoubleStrike(ServerEntity *target){
        target->m_LifePoint -= (m_AttackPoint*(RATIO) - target->m_DefensePoint); // TODO update ration by lvl of source or lvl of spell
        target->m_LifePoint -= (m_AttackPoint*(RATIO) - target->m_DefensePoint); // TODO update ration by lvl of source or lvl of spell
    } 

    void Player::Heal(){
        m_LifePoint += 5; // TODO update ration by lvl of spell
    }
    void Player::Assasinate(ServerEntity *target){
        int range = 100 / LEVELSPELL;
        if( rand() % range == 1){
            target->m_LifePoint = 0;
        }else{
            target->m_LifePoint -= 1;
        }
    }

    void Player::DamageUp(){
        m_AttackPoint += 5;
    }


    void Player::Protection(ServerEntity *target){
        target->m_DefensePoint += 5; 
    }

    void Player::Revenge(ServerEntity *target){
        target->m_LifePoint -= (m_MaxLifePoint - m_LifePoint) / 5 ;
    }

    void Player::Lacerate(ServerEntity *target){
        target->m_LifePoint -= (target->m_MaxLifePoint - target->m_LifePoint) / 5 ;
    }

    void Player::Incinerate(ServerEntity *target){
        target->m_LifePoint -= (m_AttackPoint*(RATIO) - target->m_DefensePoint);
        // TODO burn state
    }

    void Player::Devastate(ServerEntity *target, int zone){
        // TODO get all foes in an area and attack them
    }


    void Player::Massacre(ServerEntity *target){
        target->m_LifePoint -= (m_AttackPoint*(RATIO) - target->m_DefensePoint);
        // TODO bleed status
    }


    void Player::Impact(ServerEntity *target,gf::SquareMap m_SquareWorld){
    // dash attack
        gf::Vector2i start = m_Pos;
        gf::Vector2i end = target->m_Pos;

        target->m_LifePoint -= (m_AttackPoint*(RATIO) - target->m_DefensePoint);
        std::vector<gf::Vector2i> dash = m_SquareWorld.computeRoute(start, end, 0.0); // first set of tile for the corridor

        if(dash.size() > 1){ // techniccaly we will check if the field of vision is clear from the source to target so there wont be anyone on the path 
            m_Pos = dash[1]; // TODO check if correct
        }
    }

    void Player::LightningStrike(ServerEntity *target, int zone){
        // TODO get all foes in an area and attack them
    }


    void Player::Scorch(ServerEntity *target){
        target->m_LifePoint -= (m_AttackPoint*(RATIO) - target->m_DefensePoint);
        // TODO draw if bleed
    }

    void Player::Berserk(){
        m_AttackPoint += 5;
        m_DefensePoint -= 5;
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

    void Reincarnate(ServerEntity *target){
        // TODO kick
    }

    void Player::Shoot(ServerEntity *target){
        target->m_LifePoint -= (m_AttackPoint*(RATIO) - target->m_DefensePoint);
    }

    void Player::Backstab(ServerEntity *target){
        target->m_LifePoint -= (m_AttackPoint*(RATIO) - target->m_DefensePoint);
    }


    void Player::Energize(){
        // same as Damage uup but with bigger bonuse for a limited time
    }

    void Player::Torpedo(ServerEntity *target){
        target->m_LifePoint -= (m_AttackPoint*(RATIO) - target->m_DefensePoint);
    }

    void Player::SoulLink(ServerEntity *target){
        // TODO
    }
}
