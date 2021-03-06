#include "Entity.h"
#include <gf/VectorOps.h>

namespace redsquare
{

    Entity::Entity(gf::Id entityID, const EntityType entityType, const EntitySubType entitySubType)
    : m_EntitySubType(entitySubType)
    , m_RowCargoSlotNmb(6)
    , m_ColumnCargoSlotNmb(10)
    , m_EntityID(entityID)
    , m_EntityType(entityType)
    {
        switch (m_EntitySubType)
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

    gf::Id Entity::getEntityID() const
    {
        return m_EntityID;
    }

    EntityType Entity::getEntityType() const
    {
        return m_EntityType;
    }
}