#ifndef REDSQUARE_CLIENT_ANIMATEDENTITY_H
#define REDSQUARE_CLIENT_ANIMATEDENTITY_H

#include <gf/Entity.h>
#include <gf/Animation.h>
#include <gf/Texture.h>
#include <gf/Path.h>

#include "../common/Entity.h"

namespace redsquare
{
    class AnimatedEntity: public gf::Entity, public redsquare::Entity
    {
    public:
        AnimatedEntity(gf::Id entityID, gf::Vector2i pos, gf::Path pathTextureAnimated, int line, int startFramePos, int nmbFrames, float frameDuration);

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
        virtual void update(gf::Time time) override;

        float m_FrameDuration;
        gf::Animation m_Animation;
        gf::Texture m_AnimatedTexture;
        gf::Vector2f m_TextureSize;
    private:
        

        void loadAnimation(int line, int startFramePos, int nmbFrames);
    };
}

#endif