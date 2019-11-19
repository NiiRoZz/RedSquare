#ifndef REDSQUARE_CLIENT_PROP_H
#define REDSQUARE_CLIENT_PROP_H

#include "../common/Entity.h"

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Animation.h>

namespace redsquare
{
    class Prop: public gf::Entity, public redsquare::Entity
    {
    public:
        Prop( gf::Id entityID, EntitySubType type, gf::Vector2i pos );

        void loadTexture(const EntitySubType type);

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
        gf::Texture *propTexture;
        gf::Animation m_Animation;
        bool m_Animated;

        void loadAnimation( gf::Path pathTextureAnimated, int line, int startFramePos, int nmbFrames, float frameDuration);
    };
}

#endif