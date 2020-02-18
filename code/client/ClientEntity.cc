#include "ClientEntity.h"
#include "../common/Singletons.h"
#include <gf/AnimatedSprite.h>
#include <gf/Sprite.h>  
#include "Player.h"

namespace redsquare
{
    ClientEntity::ClientEntity(gf::Id entityID, const EntityType entityType, const EntitySubType entitySubType)
    : redsquare::Entity(entityID,entityType,entitySubType)
    , m_Inventory(entityID)
    , m_EntityTexture(loadTexture(entitySubType))
    , m_Animated(false)
    { 

        switch (m_EntitySubType)
        {
            case EntitySubType::Warrior:
                m_Animated = true;
                //loadAnimation("img/Character/Warrior_animation.png", 0, 0, 6, 0.2f);
                loadAnimation("img/Character/WarriorNew_animation.png", 0, 0, 3, 0.2f);
                break;

            case EntitySubType::Magus:
                m_Animated = true;
                loadAnimation("img/Character/Magus_animation.png", 0, 0, 4, 0.2f);
                break;

            case EntitySubType::Ranger:
                m_Animated = true;
                loadAnimation("img/Character/Ranger_animation.png", 0, 0, 4, 0.2f);
                break;

            case EntitySubType::Rogue:
                m_Animated = true;
                loadAnimation("img/Character/Rogue_animation.png", 0, 0, 3, 0.3f);
                break;

            case EntitySubType::Shaman:
                m_Animated = true;
                loadAnimation("img/Monster/Shaman_animation.png", 0, 0, 5, 0.2f);
                break;

            case EntitySubType::Orc:
                m_Animated = true;
                loadAnimation("img/Monster/Orc_animation.png", 0, 0, 5, 0.2f);
                break;

            case EntitySubType::Goblin:
                m_Animated = true;
                loadAnimation("img/Monster/Goblin_animation.png", 0, 0, 6, 0.1f);
                break;

            case EntitySubType::Bat:
                m_Animated = true;
                loadAnimation("img/Monster/Bat_animation.png", 0, 0, 4, 0.2f);
                break;
            
            case EntitySubType::Slime:
                m_Animated = true;
                loadAnimation("img/Monster/Slime_animation.png", 0, 0, 6, 0.15f);
                break;

            case EntitySubType::Imp:
                m_Animated = true;
                loadAnimation("img/Monster/Imp_animation.png", 0, 0, 4, 0.15f);
                break;

            case EntitySubType::LilGob:
                m_Animated = true;
                loadAnimation("img/Monster/LilGob_animation.png", 0, 0, 4, 0.15f);
                break;

            case EntitySubType::LilZombie:
                m_Animated = true;
                loadAnimation("img/Monster/LilZombie_animation.png", 0, 0, 4, 0.15f);
                break;

            case EntitySubType::Swamp:
                m_Animated = true;
                loadAnimation("img/Monster/Swamp_animation.png", 0, 0, 4, 0.15f);
                break;

            case EntitySubType::Mud:
                m_Animated = true;
                loadAnimation("img/Monster/Mud_animation.png", 0, 0, 4, 0.15f);
                break;

            case EntitySubType::Zombie:
                m_Animated = true;
                loadAnimation("img/Monster/Zombie_animation.png", 0, 0, 4, 0.15f);
                break;

            case EntitySubType::Mask:
                m_Animated = true;
                loadAnimation("img/Monster/Mask_animation.png", 0, 0, 4, 0.15f);
                break;
             
            default:
                break;
        }
        
        
        if (m_EntitySubType == EntitySubType::Swamp)
        {
            m_Animated = true;
            loadAnimation("img/Monster/Swamp_animation.png", 0, 0, 4, 0.15f);
        }
        if (m_EntitySubType == EntitySubType::LilGob)
        {
            m_Animated = true;
            loadAnimation("img/Monster/LilGob_animation.png", 0, 0, 4, 0.15f);
        }
    }

    Inventory& ClientEntity::getInventory()
    {
        return m_Inventory;
    }

    gf::Texture* ClientEntity::getEntityTexture() const
    {
        return m_EntityTexture;
    }

    gf::Texture* ClientEntity::loadTexture(const EntitySubType entitySubType)
    {
        switch (entitySubType)
        {
            //Player textures
            case EntitySubType::Magus:
            {
                return &gResourceManager().getTexture("img/Character/Magus.png");
            }
            case EntitySubType::Warrior:
            {
                return &gResourceManager().getTexture("img/Character/WarriorNew.png");
                break;
            }
            case EntitySubType::Rogue:
            {
                return &gResourceManager().getTexture("img/Character/Rogue.png");
            }
            case EntitySubType::Ranger:
            {
                return &gResourceManager().getTexture("img/Character/Ranger.png");
            }
            case EntitySubType::Healer:
            {
                return &gResourceManager().getTexture("img/Character/Healer.png");
            }

            //Monster textures
            case EntitySubType::Bat:
            {
                return &gResourceManager().getTexture("img/Monster/Bat.png");
            }
            case EntitySubType::SkeletonKnife:
            {
                return &gResourceManager().getTexture("img/Monster/SkeletonKnife.png");
            }
            case EntitySubType::SkeletonMagus:
            {
                return &gResourceManager().getTexture("img/Monster/SkeletonMagus.png");
            }
            case EntitySubType::Slime:
            {
                return &gResourceManager().getTexture("img/Monster/Slime.png");
            }
            case EntitySubType::Spirit:
            {
                return &gResourceManager().getTexture("img/Monster/Spirit.png");
            }
            case EntitySubType::Mud:
            {
                return &gResourceManager().getTexture("img/Monster/Mud.png");
            }
            case EntitySubType::Demon:
            {
                return &gResourceManager().getTexture("img/Monster/Demon.png");
            }
            case EntitySubType::Imp:
            {
                return &gResourceManager().getTexture("img/Monster/Imp.png");
            }
            case EntitySubType::Goblin:
            {
                return &gResourceManager().getTexture("img/Monster/Goblin.png");
            }
            case EntitySubType::LilGob:
            {
                return &gResourceManager().getTexture("img/Monster/LilGob.png");
            }
            case EntitySubType::LilZombie:
            {
                return &gResourceManager().getTexture("img/Monster/LilZombie.png");
            }
            case EntitySubType::Lizard:
            {
                return &gResourceManager().getTexture("img/Monster/Lizard.png");
            }
            case EntitySubType::Mask:
            {
                return &gResourceManager().getTexture("img/Monster/Mask.png");
            }
            case EntitySubType::Orc:
            {
                return &gResourceManager().getTexture("img/Monster/Orc.png");
            }
            case EntitySubType::Shaman:
            {
                return &gResourceManager().getTexture("img/Monster/Shaman.png");
            }
            case EntitySubType::Swamp:
            {
                return &gResourceManager().getTexture("img/Monster/Swamp.png");
            }
            case EntitySubType::Zombie:
            {
                return &gResourceManager().getTexture("img/Monster/Zombie.png");
            }

            //Prop textures
            case EntitySubType::BrokenPots:
            {
                return &gResourceManager().getTexture("img/Props/BrokenVase.png");
            }
            case EntitySubType::Pot:
            {
                return &gResourceManager().getTexture("img/Props/Vase.png");
            }
            case EntitySubType::ExtinctTorch:
            {
                return &gResourceManager().getTexture("img/Props/ExtinctTorch.png");
            }
            case EntitySubType::Stool:
            {
                return &gResourceManager().getTexture("img/Props/Stool.png");
            }
            case EntitySubType::PileWood:
            {
                return &gResourceManager().getTexture("img/Props/WoodPile.png");
            }
            case EntitySubType::OpenedChest:
            {
                return &gResourceManager().getTexture("img/Props/OpenChest.png");
            }
            case EntitySubType::Chest:
            {
                return &gResourceManager().getTexture("img/Props/Chest.png");
            }
            case EntitySubType::Rock:
            {
                return &gResourceManager().getTexture("img/Props/Rock.png");
            }
            case EntitySubType::BrownRock:
            {
                return &gResourceManager().getTexture("img/Props/BrownRock.png");
            }
            case EntitySubType::GreyBrokenPillar:
            {
                return &gResourceManager().getTexture("img/Props/BrokenPillar.png");
            }
            case EntitySubType::BrownBrokenPillar:
            {
                return &gResourceManager().getTexture("img/Props/BrokenPillar2.png");
            }
            case EntitySubType::BlankShelf:
            {
                return &gResourceManager().getTexture("img/Props/BlankShelf.png");
            }
            case EntitySubType::BookShelf:
            {
                return &gResourceManager().getTexture("img/Props/BookShelf.png");
            }
            case EntitySubType::BrokenShelf:
            {
                return &gResourceManager().getTexture("img/Props/BrokenShelf.png");
            }
            case EntitySubType::PotShelf:
            {
                return &gResourceManager().getTexture("img/Props/PotShelf.png");
            }
            case EntitySubType::EmptyStand:
            {
                return &gResourceManager().getTexture("img/Props/EmptyStand.png");
            }
            case EntitySubType::WeaponShelf1:
            {
                return &gResourceManager().getTexture("img/Props/WeaponShelf1.png");
            }
            case EntitySubType::WeaponShelf2:
            {
                return &gResourceManager().getTexture("img/Props/WeaponShelf2.png");
            }
            case EntitySubType::DualBox:
            {
                return &gResourceManager().getTexture("img/Props/DualBox.png");
            }
            case EntitySubType::BrownDualRock:
            {
                return &gResourceManager().getTexture("img/Props/BrownDualRock.png");
            }
            case EntitySubType::GreyDualRock:
            {
                return &gResourceManager().getTexture("img/Props/GreyDualRock.png");
            }
            case EntitySubType::RightChain:
            {
                return &gResourceManager().getTexture("img/Props/RightChain.png");
            }
            case EntitySubType::LeftChain:
            {
                return &gResourceManager().getTexture("img/Props/LeftChain.png");
            }
            case EntitySubType::Throne:
            {
                return &gResourceManager().getTexture("img/Props/Throne.png");
            }
            case EntitySubType::BrownPillar1:
            {
                return &gResourceManager().getTexture("img/Props/BrownPillar1.png");
            }
            case EntitySubType::BrownPillar2:
            {
                return &gResourceManager().getTexture("img/Props/BrownPillar2.png");
            }
            case EntitySubType::GreyPillar1:
            {
                return &gResourceManager().getTexture("img/Props/GreyPillar1.png");
            }
            case EntitySubType::GreyPillar2:
            {
                return &gResourceManager().getTexture("img/Props/GreyPillar2.png");
            }
            case EntitySubType::Box1:
            {
                return &gResourceManager().getTexture("img/Props/Box1.png");
            }
            case EntitySubType::Box2:
            {
                return &gResourceManager().getTexture("img/Props/Box2.png");
            }
            case EntitySubType::GreyQuadRock:
            {
                return &gResourceManager().getTexture("img/Props/GreyQuadRock.png");
            }
            case EntitySubType::BrownQuadRock:
            {
                return &gResourceManager().getTexture("img/Props/BrownQuadRock.png");
            }
            case EntitySubType::Table:
            {
                return &gResourceManager().getTexture("img/Props/table.png");
            }
            case EntitySubType::LitllePots:
            {
                return &gResourceManager().getTexture("img/Props/pots.png");
            }

            default:
                break;
        }

        std::cout << "ClientEntity::loadTexture Error default : " << static_cast<uint8_t>(entitySubType) * 1.0 << " " << static_cast<uint16_t>(entitySubType) * 1.0 << std::endl;
 
        return &gResourceManager().getTexture("img/Character/Magus.png");
    }

    void ClientEntity::loadCharacteristic(EntityCharacteristicData &characteristics)
    {
        m_LifePoint = characteristics.m_LifePoint;
        m_MaxLifePoint = characteristics.m_MaxLifePoint;

        m_AttackPoint = characteristics.m_AttackPoint;
        m_MaxAttackPoint = characteristics.m_MaxAttackPoint;

        m_DefensePoint = characteristics.m_DefensePoint;
        m_MaxDefensePoint = characteristics.m_MaxDefensePoint;

        m_Range = characteristics.m_Range;
        m_Level = characteristics.m_Level;

        if (getEntityType() == EntityType::Player)
        {
            Player* player = static_cast<Player*>(this);
            if (player != nullptr)
            {
                player->m_ManaPoint = characteristics.m_ManaPoint;
                player->m_MaxManaPoint = characteristics.m_MaxManaPoint;

                player->m_XP = characteristics.m_XP;
                player->m_Max_XP = characteristics.m_MaxXP;
                player->m_SpellTab = characteristics.m_Spells;
            }
        }
    }

    void ClientEntity::loadAnimation(gf::Path pathTextureAnimated, int line, int startFramePos, int nmbFrames, float frameDuration)
    {
        static constexpr gf::Vector2f FrameSize = { 16.0f, 16.0f };
        gf::Time TimeFrameDuration = gf::seconds(frameDuration);
        gf::Texture &textureAnimated = gResourceManager().getTexture(pathTextureAnimated);
        gf::Vector2i textureSize = textureAnimated.getSize();

        for (int i = startFramePos; i < (startFramePos + nmbFrames); ++i)
        {
            gf::RectF frame = gf::RectF::fromPositionSize( gf::Vector2i(i, line) * FrameSize / textureSize, FrameSize / textureSize);
            m_Animation.addFrame(textureAnimated, frame, TimeFrameDuration);
        }
    }
}