#include "AnimatedEntity.h"
#include "World.h"

#include <gf/AnimatedSprite.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

namespace redsquare
{
    AnimatedEntity::AnimatedEntity(gf::Id entityID, gf::Vector2i pos, gf::Path pathTextureAnimated, int line, int startFramePos, int nmbFrames, float frameDuration)
    : redsquare::Entity(entityID)
    , m_FrameDuration(frameDuration)
    {
        m_Pos = pos;
        m_AnimatedTexture.loadFromFile(pathTextureAnimated);
        m_TextureSize = m_AnimatedTexture.getSize();
        
        loadAnimation(line, startFramePos, nmbFrames);

        //std::cout << "AnimatedEntity::render m_Animation : " << &m_Animation << " m_Animation.getCurrentBounds().getTopLeft()[0] : " << m_Animation.getCurrentBounds().getTopLeft()[0] << " m_Animation.getCurrentTexture().getSize()[0] : " << m_Animation.getCurrentTexture().getSize()[0] << std::endl;
    }

    void AnimatedEntity::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::AnimatedSprite animated;
        //std::cout << "AnimatedEntity::render m_Animation : " << &m_Animation << " m_Animation.getCurrentBounds().getTopLeft()[0] : " << m_Animation.getCurrentBounds().getTopLeft()[0] << " m_Animation.getCurrentTexture().getSize()[0] : " << m_Animation.getCurrentTexture().getSize()[0] << std::endl;
        animated.setAnimation(m_Animation);
        animated.setScale(1.0f);
        animated.setPosition(m_Pos * World::TileSize);
        //TODO: draw not working, creating core dumped
        //target.draw(animated, states);
    }

    void AnimatedEntity::update(gf::Time time)
    {
        bool changed = m_Animation.update(time);
        //std::cout << "changed : " << changed << std::endl;
    }

    void AnimatedEntity::loadAnimation(int line, int startFramePos, int nmbFrames)
    {
        static constexpr gf::Vector2f FrameSize = { 16.0f, 16.0f };
        gf::Time FrameDuration = gf::seconds(m_FrameDuration);

        for (int i = startFramePos; i < (startFramePos + nmbFrames); ++i)
        {
            gf::RectF frame(gf::Vector2i(i, line) * FrameSize / m_TextureSize, FrameSize / m_TextureSize);
            m_Animation.addFrame(m_AnimatedTexture, frame, FrameDuration);
        }
    }
}