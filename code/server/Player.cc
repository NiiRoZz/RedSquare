#include "Player.h"
#include <iostream>

namespace redsquare
{
    Player::Player(SocketTcp socket, gf::Id playerID, gf::Vector2i pos)
    : m_Socket(std::move(socket))
    , m_PlayerID(playerID)
    , m_Pos(pos)
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

        m_MovePoint = 50;
        m_Range = 50;
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
            printf("actual position x: %d\nactual position y: %d\n",m_Pos[0] ,m_Pos[1]);
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

        m_AttackPoint += 50;
        m_DefensePoint += 50;
        m_Range = 1; // MELEE IF 1
    }

    void Player::createCarPacket(Packet &packet){
        packet.type = PacketType::PlayerCar;
        packet.playerCar.m_LifePoint = m_LifePoint;
        packet.playerCar.m_ManaPoint = m_ManaPoint;

        packet.playerCar.m_MaxLifePoint = m_MaxLifePoint;
        packet.playerCar.m_MaxManaPoint = m_MaxManaPoint;

        packet.playerCar.m_AttackPoint = m_AttackPoint;
        packet.playerCar.m_DefensePoint = m_DefensePoint;
        packet.playerCar.m_MovePoint = m_MovePoint;
        packet.playerCar.m_Range = m_Range;

    }
}
