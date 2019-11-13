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
            case EntitySubType::Box:
            {
                propTexture.loadFromFile("data/redsquare/img/spirit.png");
                break;
            }

            case EntitySubType::Pot:
            {
                propTexture.loadFromFile("data/redsquare/img/spirit.png");
                break;
            }

            case EntitySubType::Torch:
            {
                m_Animated = true;
                loadAnimation("img/TileSet3.png", 18, 0, 4, 0.1f);
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
            sprite.setTexture( propTexture );
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
            gf::RectF frame(gf::Vector2i(i, line) * FrameSize / textureSize, FrameSize / textureSize);
            m_Animation.addFrame(textureAnimated, frame, TimeFrameDuration);
        }
    }
}