#include "Prop.h"
#include "World.h"
#include "../common/Singletons.h"

#include <gf/AnimatedSprite.h>
#include <gf/Sprite.h>  
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

namespace redsquare
{
    Prop::Prop( gf::Id entityID, EntitySubType type, gf::Vector2i pos )
    : redsquare::Entity(entityID,type)
    , m_Animated(false)
    {
        m_Pos = pos;

        loadTexture(type);
    }

    void Prop::loadTexture(const EntitySubType type)
    {
        switch (type)
        {
            case EntitySubType::BrokenPots:
            {
                propTexture = &gResourceManager().getTexture("img/Props/BrokenVase.png");
                break;
            }
            case EntitySubType::Pot:
            {
                propTexture = &gResourceManager().getTexture("img/Props/Vase.png");
                break;
            }
            case EntitySubType::Torch:
            {
                m_Animated = true;
                loadAnimation("img/TileSet3.png", 18, 0, 4, 0.1f);
                break;
            }
            case EntitySubType::ExtinctTorch:
            {
                propTexture = &gResourceManager().getTexture("img/Props/ExtinctTorch.png");
                break;
            }
            case EntitySubType::Stool:
            {
                propTexture = &gResourceManager().getTexture("img/Props/Stool.png");
                break;
            }
            case EntitySubType::PileWood:
            {
                propTexture = &gResourceManager().getTexture("img/Props/WoodPile.png");
                break;
            }
            case EntitySubType::OpenedChest:
            {
                propTexture = &gResourceManager().getTexture("img/Props/OpenChest.png");
                break;
            }
            case EntitySubType::Chest:
            {
                propTexture = &gResourceManager().getTexture("img/Props/Chest.png");
                break;
            }
            case EntitySubType::Rock:
            {
                propTexture = &gResourceManager().getTexture("img/Props/Rock.png");
                break;
            }
            case EntitySubType::BrownRock:
            {
                propTexture = &gResourceManager().getTexture("img/Props/BrownRock.png");
                break;
            }
            case EntitySubType::GreyBrokenPillar:
            {
                propTexture = &gResourceManager().getTexture("img/Props/BrokenPillar.png");
                break;
            }
            case EntitySubType::BrownBrokenPillar:
            {
                propTexture = &gResourceManager().getTexture("img/Props/BrokenPillar2.png");
                break;
            }
            case EntitySubType::BlankShelf:
            {
                propTexture = &gResourceManager().getTexture("img/Props/BlankShelf.png");
                break;
            }
            case EntitySubType::BookShelf:
            {
                propTexture = &gResourceManager().getTexture("img/Props/BookShelf.png");
                break;
            }
            case EntitySubType::BrokenShelf:
            {
                propTexture = &gResourceManager().getTexture("img/Props/BrokenShelf.png");
                break;
            }
            case EntitySubType::PotShelf:
            {
                propTexture = &gResourceManager().getTexture("img/Props/PotShelf.png");
                break;
            }
            case EntitySubType::EmptyStand:
            {
                propTexture = &gResourceManager().getTexture("img/Props/EmptyStand.png");
                break;
            }
            case EntitySubType::WeaponShelf1:
            {
                propTexture = &gResourceManager().getTexture("img/Props/WeaponShelf1.png");
                break;
            }
            case EntitySubType::WeaponShelf2:
            {
                propTexture = &gResourceManager().getTexture("img/Props/WeaponShelf2.png");
                break;
            }
            case EntitySubType::DualBox:
            {
                propTexture = &gResourceManager().getTexture("img/Props/DualBox.png");
                break;
            }
            case EntitySubType::BrownDualRock:
            {
                propTexture = &gResourceManager().getTexture("img/Props/BrownDualRock.png");
                break;
            }
            case EntitySubType::GreyDualRock:
            {
                propTexture = &gResourceManager().getTexture("img/Props/GreyDualRock.png");
                break;
            }
            case EntitySubType::RightChain:
            {
                propTexture = &gResourceManager().getTexture("img/Props/RightChain.png");
                break;
            }
            case EntitySubType::LeftChain:
            {
                propTexture = &gResourceManager().getTexture("img/Props/LeftChain.png");
                break;
            }
            case EntitySubType::Throne:
            {
                propTexture = &gResourceManager().getTexture("img/Props/Throne.png");
                break;
            }
            case EntitySubType::BrownPillar1:
            {
                propTexture = &gResourceManager().getTexture("img/Props/BrownPillar1.png");
                break;
            }
            case EntitySubType::BrownPillar2:
            {
                propTexture = &gResourceManager().getTexture("img/Props/BrownPillar2.png");
                break;
            }
            case EntitySubType::GreyPillar1:
            {
                propTexture = &gResourceManager().getTexture("img/Props/GreyPillar1.png");
                break;
            }
            case EntitySubType::GreyPillar2:
            {
                propTexture = &gResourceManager().getTexture("img/Props/GreyPillar2.png");
                break;
            }
            case EntitySubType::Box1:
            {
                propTexture = &gResourceManager().getTexture("img/Props/Box1.png");
                break;
            }
            case EntitySubType::Box2:
            {
                propTexture = &gResourceManager().getTexture("img/Props/Box2.png");
                break;
            }
            case EntitySubType::GreyQuadRock:
            {
                propTexture = &gResourceManager().getTexture("img/Props/GreyQuadRock.png");
                break;
            }
            case EntitySubType::BrownQuadRock:
            {
                propTexture = &gResourceManager().getTexture("img/Props/BrownQuadRock.png");
                break;
            }
            case EntitySubType::Table:
            {
                propTexture = &gResourceManager().getTexture("img/Props/table.png");
                break;
            }
            default:
            {
                break;
            }
        }
    }

    void Prop::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        if (m_Animated)
        {
            gf::AnimatedSprite animated;
            animated.setAnimation(m_Animation);
            animated.setPosition(m_Pos * World::TileSize);
            animated.setScale(1.0f);
            
            target.draw(animated, states);
        }
        else
        {
            gf::Sprite sprite;

            sprite.setPosition( m_Pos * World::TileSize );
            sprite.setScale( 1 );
            sprite.setTexture( *propTexture );
            target.draw(sprite, states);
        }
    }

    void Prop::update(gf::Time time)
    {
        if (m_Animated)
        {
            m_Animation.update(time);
        }
    }

    void Prop::loadAnimation(gf::Path pathTextureAnimated, int line, int startFramePos, int nmbFrames, float frameDuration)
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