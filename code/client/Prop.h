#ifndef REDSQUARE_CLIENT_PROP_H
#define REDSQUARE_CLIENT_PROP_H

#include "ClientEntity.h"

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Animation.h>

namespace redsquare
{
    class Prop: public ClientEntity
    {
    public:
        Prop( gf::Id entityID, EntitySubType entitySubType, gf::Vector2i pos );

        void update(gf::Time time);

        void render(gf::RenderTarget& target, const gf::RenderStates& states);

        bool haveInventory() const;

    private:
        gf::Texture *propTexture;
        gf::Animation m_Animation;
        bool m_Animated;

        void loadAnimation( gf::Path pathTextureAnimated, int line, int startFramePos, int nmbFrames, float frameDuration);
    };
}

#endif