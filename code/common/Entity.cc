#include "Entity.h"
#include <gf/VectorOps.h>

#define NBMONSTER 5;

namespace redsquare
{
    Entity::Entity()
    : m_TypeOfEntity(EntitySubType::Magus)
    , m_EntityID(0)
    , m_Size({1,1})
    {
        
    }

    Entity::Entity(gf::Id entityID)
    : m_EntityID(entityID)
    , m_Size({1,1})
    {
        EntitySubType typeOfEntity;
        int randomMonster = rand() % NBMONSTER;

        switch (randomMonster)
        {
        case 0:
            m_TypeOfEntity = EntitySubType::Bat;
            break;
        case 1:
            m_TypeOfEntity = EntitySubType::SkeletonKnife;
            break;
        case 2:
            m_TypeOfEntity = EntitySubType::SkeletonMagus;
            break;
        case 3:
            m_TypeOfEntity = EntitySubType::Slime;
            break;
        case 4:
            m_TypeOfEntity = EntitySubType::Spirit;
            break;
        default:
            m_TypeOfEntity = EntitySubType::Bat;
            break;
        }
    }

    Entity::Entity(gf::Id entityID, const EntitySubType typeOfEntity)
    : m_EntityID(entityID)
    , m_TypeOfEntity(typeOfEntity)
    {
        switch (typeOfEntity)
        {
            case EntitySubType::BlankShelf : 
                m_Size = {2,1};
                break;
            case EntitySubType::BookShelf : 
                m_Size = {2,1};
                break;
            case EntitySubType::BrokenShelf : 
                m_Size = {2,1};
                break;
            case EntitySubType::PotShelf : 
                m_Size = {2,1};
                break;
            case EntitySubType::EmptyStand : 
                m_Size = {2,1};
                break;
            case EntitySubType::WeaponShelf1 : 
                m_Size = {2,1};
                break;
            case EntitySubType::WeaponShelf2 : 
                m_Size = {2,1};
                break;
            case EntitySubType::DualBox : 
                m_Size = {2,1};
                break;            
            case EntitySubType::BrownDualRock : 
                m_Size = {2,1};
                break; 
            case EntitySubType::GreyDualRock : 
                m_Size = {2,1};
                break;  
            case EntitySubType::RightChain : 
                m_Size = {1,2};
                break; 
            case EntitySubType::LeftChain : 
                m_Size = {1,2};
                break;   
            case EntitySubType::Throne : 
                m_Size = {1,2};
                break;   
            case EntitySubType::BrownPillar1 : 
                m_Size = {1,2};
                break; 
            case EntitySubType::BrownPillar2 : 
                m_Size = {1,2};
                break; 
            case EntitySubType::GreyPillar1 : 
                m_Size = {1,2};
                break;  
            case EntitySubType::GreyPillar2 : 
                m_Size = {1,2};
                break;   
            case EntitySubType::Box1 : 
                m_Size = {1,2};
                break; 
            case EntitySubType::Box2 : 
                m_Size = {1,2};
                break;      
            case EntitySubType::GreyQuadRock :
                m_Size = {2,2};
                break; 
            case EntitySubType::BrownQuadRock :
                m_Size = {2,2};
                break;
            case EntitySubType::Table : 
                m_Size = {2,3};
                break;   
            default :
                m_Size = {1,1};
                break;     
        }

    }

    bool Entity::isInsideMe(gf::Vector2i pos)
    {
        for(uint i = 0; i < m_Size[0]; ++i)
        {
            for(uint j = 0; j < m_Size[1]; ++j)
            {
                if (pos == gf::Vector2i({m_Pos[0] + (int)i, m_Pos[1] + (int)j}))
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool Entity::isInsideMe(Entity &entity)
    {
        for(uint i = 0; i < entity.m_Size[0]; ++i)
        {
            for(uint j = 0; j < entity.m_Size[1]; ++j)
            {
                if (isInsideMe({entity.m_Pos[0] + (int)i, entity.m_Pos[1] + (int)j}))
                {
                    return true;
                }
            }
        }

        return false;
    }
}