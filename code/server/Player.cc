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

        m_MovePoint = 6;
        m_Range = 1;

        m_XP = 0;
        m_MaxXP = 100;
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
        //MouseClic
        if(dirX !=0 && dirY !=0)
        {
            int newPosX =  dirX ;
            int newPosY =  dirY;
            
            
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
        //
        else
        {
            
            int newPosY = m_Pos[1] + dirY;
            int newPosX = m_Pos[0] + dirX;
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

     void Player::levelUp(){

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

    void Player::createCarPacket(Packet &packet)
    {
        packet.type = PacketType::PlayerCar;

        packet.playerCar.playerID = m_PlayerID;

        packet.playerCar.m_LifePoint = m_LifePoint;
        packet.playerCar.m_ManaPoint = m_ManaPoint;

        packet.playerCar.m_MaxLifePoint = m_MaxLifePoint;
        packet.playerCar.m_MaxManaPoint = m_MaxManaPoint;

        packet.playerCar.m_AttackPoint = m_AttackPoint;
        packet.playerCar.m_DefensePoint = m_DefensePoint;
        packet.playerCar.m_MovePoint = m_MovePoint;
        packet.playerCar.m_Range = m_Range;

        packet.playerCar.m_XP = m_XP;
        packet.playerCar.m_MaxXP= m_MaxXP;

        packet.playerCar.m_Level = m_Level;
    }

    void Player::playerSpawn(std::map<gf::Id,Player> &m_Players,World &world){ // set spawn for a player 
        switch (m_Players.size())
        {
        case 1:
            m_Pos = {(world.m_Spawn[0]-1),(world.m_Spawn[1]-1)};
            break;
        case 2:
            m_Pos = {(world.m_Spawn[0]),(world.m_Spawn[1]-1)};
            break;
        case 3:
            m_Pos = {(world.m_Spawn[0]+1),(world.m_Spawn[1]-1)};
            break;
        case 4:
            m_Pos = {(world.m_Spawn[0]-1),(world.m_Spawn[1])};
            break;
        case 5:
            m_Pos = {(world.m_Spawn[0]),(world.m_Spawn[1])};
            break;
        case 6:
            m_Pos = {(world.m_Spawn[0]+1),(world.m_Spawn[1])};
            break;
        case 7:
            m_Pos = {(world.m_Spawn[0]-1),(world.m_Spawn[1]+1)};
            break;
        case 8:
            m_Pos = {(world.m_Spawn[0]),(world.m_Spawn[1]+1)};
            break;
        case 9:
            m_Pos = {(world.m_Spawn[0]+1),(world.m_Spawn[1]+1)};
            break;                        
        default: // can only manage 10 or 9 player
            break;
        }
    }
}
