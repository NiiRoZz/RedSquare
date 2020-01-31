#include "ClientEntity.h"
#include "../common/Singletons.h"

namespace redsquare
{
    ClientEntity::ClientEntity(gf::Id entityID, const EntityType entityType, const EntitySubType entitySubType)
    : redsquare::Entity(entityID,entityType,entitySubType)
    , m_Inventory(entityID)
    , m_EntityTexture(loadTexture())
    {

    }

    Inventory& ClientEntity::getInventory()
    {
        return m_Inventory;
    }

    gf::Texture* ClientEntity::getEntityTexture() const
    {
        return m_EntityTexture;
    }

    gf::Texture* ClientEntity::loadTexture() const
    {
        switch (m_EntitySubType)
        {
            //Player textures
            case EntitySubType::Magus:
            {
                return &gResourceManager().getTexture("img/Character/Magus.png");
                break;
            }
            case EntitySubType::Warrior:
            {
                return &gResourceManager().getTexture("img/Character/Warrior.png");
                break;
            }
            case EntitySubType::Rogue:
            {
                return &gResourceManager().getTexture("img/Character/Rogue.png");
                break;
            }
            case EntitySubType::Ranger:
            {
                return &gResourceManager().getTexture("img/Character/Ranger.png");
                break;
            }
            case EntitySubType::Healer:
            {
                return &gResourceManager().getTexture("img/Character/Magus.png");
                break;
            }

            //Monster textures
            case EntitySubType::Bat:
            {
                return &gResourceManager().getTexture("img/Monster/Bat.png");
                break;
            }
            case EntitySubType::SkeletonKnife:
            {
                return &gResourceManager().getTexture("img/Monster/SkeletonKnife.png");
                break;
            }
            case EntitySubType::SkeletonMagus:
            {
                return &gResourceManager().getTexture("img/Monster/SkeletonMagus.png");
                break;
            }
            case EntitySubType::Slime:
            {
                return &gResourceManager().getTexture("img/Monster/Slime.png");
                break;
            }
            case EntitySubType::Spirit:
            {
                return &gResourceManager().getTexture("img/Monster/Spirit.png");
                break;
            }
            case EntitySubType::Mud:
            {
                return &gResourceManager().getTexture("img/Monster/Mud.png");
                break;
            }
            case EntitySubType::Demon:
            {
                return &gResourceManager().getTexture("img/Monster/Demon.png");
                break;
            }
            case EntitySubType::Imp:
            {
                return &gResourceManager().getTexture("img/Monster/Imp.png");
                break;
            }
            case EntitySubType::Goblin:
            {
                return &gResourceManager().getTexture("img/Monster/Goblin.png");
                break;
            }
            case EntitySubType::LilGob:
            {
                return &gResourceManager().getTexture("img/Monster/LilGob.png");
                break;
            }
            case EntitySubType::LilZombie:
            {
                return &gResourceManager().getTexture("img/Monster/LilZombie.png");
                break;
            }
            case EntitySubType::Lizard:
            {
                return &gResourceManager().getTexture("img/Monster/Lizard.png");
                break;
            }
            case EntitySubType::Mask:
            {
                return &gResourceManager().getTexture("img/Monster/Mask.png");
                break;
            }
            case EntitySubType::Orc:
            {
                return &gResourceManager().getTexture("img/Monster/Orc.png");
                break;
            }
            case EntitySubType::Shaman:
            {
                return &gResourceManager().getTexture("img/Monster/Shaman.png");
                break;
            }
            case EntitySubType::Swamp:
            {
                return &gResourceManager().getTexture("img/Monster/Swamp.png");
                break;
            }
            case EntitySubType::Zombie:
            {
                return &gResourceManager().getTexture("img/Monster/Zombie.png");
                break;
            }

            //Prop textures
            case EntitySubType::BrokenPots:
            {
                return &gResourceManager().getTexture("img/Props/BrokenVase.png");
                break;
            }
            case EntitySubType::Pot:
            {
                return &gResourceManager().getTexture("img/Props/Vase.png");
                break;
            }
            case EntitySubType::ExtinctTorch:
            {
                return &gResourceManager().getTexture("img/Props/ExtinctTorch.png");
                break;
            }
            case EntitySubType::Stool:
            {
                return &gResourceManager().getTexture("img/Props/Stool.png");
                break;
            }
            case EntitySubType::PileWood:
            {
                return &gResourceManager().getTexture("img/Props/WoodPile.png");
                break;
            }
            case EntitySubType::OpenedChest:
            {
                return &gResourceManager().getTexture("img/Props/OpenChest.png");
                break;
            }
            case EntitySubType::Chest:
            {
                return &gResourceManager().getTexture("img/Props/Chest.png");
                break;
            }
            case EntitySubType::Rock:
            {
                return &gResourceManager().getTexture("img/Props/Rock.png");
                break;
            }
            case EntitySubType::BrownRock:
            {
                return &gResourceManager().getTexture("img/Props/BrownRock.png");
                break;
            }
            case EntitySubType::GreyBrokenPillar:
            {
                return &gResourceManager().getTexture("img/Props/BrokenPillar.png");
                break;
            }
            case EntitySubType::BrownBrokenPillar:
            {
                return &gResourceManager().getTexture("img/Props/BrokenPillar2.png");
                break;
            }
            case EntitySubType::BlankShelf:
            {
                return &gResourceManager().getTexture("img/Props/BlankShelf.png");
                break;
            }
            case EntitySubType::BookShelf:
            {
                return &gResourceManager().getTexture("img/Props/BookShelf.png");
                break;
            }
            case EntitySubType::BrokenShelf:
            {
                return &gResourceManager().getTexture("img/Props/BrokenShelf.png");
                break;
            }
            case EntitySubType::PotShelf:
            {
                return &gResourceManager().getTexture("img/Props/PotShelf.png");
                break;
            }
            case EntitySubType::EmptyStand:
            {
                return &gResourceManager().getTexture("img/Props/EmptyStand.png");
                break;
            }
            case EntitySubType::WeaponShelf1:
            {
                return &gResourceManager().getTexture("img/Props/WeaponShelf1.png");
                break;
            }
            case EntitySubType::WeaponShelf2:
            {
                return &gResourceManager().getTexture("img/Props/WeaponShelf2.png");
                break;
            }
            case EntitySubType::DualBox:
            {
                return &gResourceManager().getTexture("img/Props/DualBox.png");
                break;
            }
            case EntitySubType::BrownDualRock:
            {
                return &gResourceManager().getTexture("img/Props/BrownDualRock.png");
                break;
            }
            case EntitySubType::GreyDualRock:
            {
                return &gResourceManager().getTexture("img/Props/GreyDualRock.png");
                break;
            }
            case EntitySubType::RightChain:
            {
                return &gResourceManager().getTexture("img/Props/RightChain.png");
                break;
            }
            case EntitySubType::LeftChain:
            {
                return &gResourceManager().getTexture("img/Props/LeftChain.png");
                break;
            }
            case EntitySubType::Throne:
            {
                return &gResourceManager().getTexture("img/Props/Throne.png");
                break;
            }
            case EntitySubType::BrownPillar1:
            {
                return &gResourceManager().getTexture("img/Props/BrownPillar1.png");
                break;
            }
            case EntitySubType::BrownPillar2:
            {
                return &gResourceManager().getTexture("img/Props/BrownPillar2.png");
                break;
            }
            case EntitySubType::GreyPillar1:
            {
                return &gResourceManager().getTexture("img/Props/GreyPillar1.png");
                break;
            }
            case EntitySubType::GreyPillar2:
            {
                return &gResourceManager().getTexture("img/Props/GreyPillar2.png");
                break;
            }
            case EntitySubType::Box1:
            {
                return &gResourceManager().getTexture("img/Props/Box1.png");
                break;
            }
            case EntitySubType::Box2:
            {
                return &gResourceManager().getTexture("img/Props/Box2.png");
                break;
            }
            case EntitySubType::GreyQuadRock:
            {
                return &gResourceManager().getTexture("img/Props/GreyQuadRock.png");
                break;
            }
            case EntitySubType::BrownQuadRock:
            {
                return &gResourceManager().getTexture("img/Props/BrownQuadRock.png");
                break;
            }
            case EntitySubType::Table:
            {
                return &gResourceManager().getTexture("img/Props/table.png");
                break;
            }
            case EntitySubType::LitllePots:
            {
                return &gResourceManager().getTexture("img/Props/pots.png");
                break;
            }
        }

        std::cout << "ClientEntity::loadTexture Error default : " << static_cast<uint8_t>(m_EntityType) * 1.0 << " " << static_cast<uint16_t>(m_EntitySubType) * 1.0 << std::endl;
 
        return &gResourceManager().getTexture("img/Character/Magus.png");
    }
}