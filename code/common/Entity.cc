#include "Entity.h"

#include <gf/VectorOps.h>

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
    , m_TypeOfEntity(EntitySubType::Magus)
    , m_Size({1,1})
    {
        
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
}