#include "Prop.h"
#include "World.h"
#include "../common/Singletons.h"

#include <gf/AnimatedSprite.h>
#include <gf/Sprite.h>  
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

namespace redsquare
{
    Prop::Prop( gf::Id entityID, EntitySubType entitySubType, gf::Vector2i pos )
    : ClientEntity(entityID, EntityType::Prop, entitySubType)
    , m_Animated(false)
    {
        m_Pos = pos;

        if (m_EntitySubType == EntitySubType::Torch)
        {
            m_Animated = true;
            loadAnimation("img/TileSet3.png", 18, 0, 4, 0.1f);
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
            sprite.setTexture( *m_EntityTexture );
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

    bool Prop::haveInventory() const
    {
        return (m_EntitySubType == EntitySubType::Chest || m_EntitySubType == EntitySubType::OpenedChest);
    }
}