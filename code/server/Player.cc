#include "Player.h"
#include <iostream>

namespace redsquare
{
    Player::Player(SocketTcp socket, gf::Id playerID)
    : m_Socket(std::move(socket))
    , m_PlayerID(playerID)
    , m_TypeOfPlayer((uint8_t)(rand() % 4))
    {
        m_LifePoint = 100;
        m_ManaPoint = 100;

        m_MaxLifePoint = 100;
        m_MaxManaPoint = 100;


        m_AttackPoint = 50;
        m_DefensePoint = 50;

        m_MaxAttackPoint = 50;
        m_MaxDefensePoint = 50;

        m_Range = 1;

        m_XP = 0;
        m_MaxXP = 100;

        m_Level = 100;
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

    bool Player::applyMove(int dirX, int dirY, gf::SquareMap &map )
    {
        //used mouse clic
        if( dirX !=0 && dirY != 0 )
        {
            int newPosX = dirX;
            int newPosY = dirY;
            
            if ( newPosY >= 0 && newPosY < World::MapSize-1 && newPosX >= 0 && newPosX < World::MapSize-1 && map.isWalkable( {newPosX, newPosY}))
            {
                m_Pos[1] = newPosY;
                m_Pos[0] = newPosX;
                
                return true;
            }
            else
            {
                return false;
            }
        }
        // used arrow key
        else
        {
            int newPosX = m_Pos[0] + dirX;
            int newPosY = m_Pos[1] + dirY;

            if ( newPosY >= 0 && newPosY < World::MapSize-1 && newPosX >= 0 && newPosX < World::MapSize-1 && map.isWalkable( {newPosX, newPosY} ) )
            {
                m_Pos[1] = newPosY;
                m_Pos[0] = newPosX;

                return true;
            }
            else
            {
                return false;
            }
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
        m_MaxXP += 100;

        m_Level++;
    }

    void Player::createCarPacket(Packet &packet) // create the packet of the caracteristic who will be send to player
    {
        packet.type = PacketType::EntityCar;
        packet.entityCar.entityType = EntityType::Player;
        packet.entityCar.entityID = m_PlayerID;

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
        switch (m_Players.size())
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
}
