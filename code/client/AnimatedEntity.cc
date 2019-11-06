#include "AnimatedEntity.h"
#include "World.h"

#include <gf/AnimatedSprite.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

namespace redsquare
{
    AnimatedEntity::AnimatedEntity(gf::Path pathTextureAnimated, int line, int startFramePos, int nmbFrames, gf::Vector2f textureSize, float frameDuration)
    : m_TextureSize(textureSize)
    , m_FrameDuration(frameDuration)
    {
        m_AnimatedTexture.loadFromFile(pathTextureAnimated);
        loadAnimation(line, startFramePos, nmbFrames);
    }

    void AnimatedEntity::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        //std::cout << "AnimatedEntity::render 1" << std::endl;
        gf::AnimatedSprite animated;
        //std::cout << "AnimatedEntity::render m_Animation : " << &m_Animation << " m_Animation.getCurrentTexture().getName() : " << m_Animation.getCurrentTexture().getName() << " m_Animation.getCurrentTexture().getSize()[0] : " << m_Animation.getCurrentTexture().getSize()[0] << std::endl;
        animated.setAnimation(m_Animation);
        animated.setScale(1.0f);
        animated.setPosition(m_Pos * World::TileSize);
        //TODO: draw not working, creating core dumped
        //target.draw(animated, states);
    }

    void AnimatedEntity::update(gf::Time time)
    {
        m_Animation.update(time);
    }

    void AnimatedEntity::loadAnimation(int line, int startFramePos, int nmbFrames)
    {
        static constexpr gf::Vector2f FrameSize = { 16.0f, 16.0f };
        gf::Time FrameDuration = gf::seconds(m_FrameDuration);

        for (int i = startFramePos; i < (startFramePos + nmbFrames); ++i)
        {
            gf::RectF frame(gf::Vector2i(i, line) *  FrameSize / m_TextureSize, FrameSize / m_TextureSize);
            m_Animation.addFrame(m_AnimatedTexture, frame, FrameDuration);
        } 
  }
}