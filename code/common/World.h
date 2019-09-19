#ifndef REDSQUARE_COMMON_WORLD_H
#define REDSQUARE_COMMON_WORLD_H

#include <gf/Entity.h>

namespace redsquare
{
    class World: public gf::Entity
    {
    public:
        static constexpr int MapSize = 128;
        static constexpr int TileSize = 1;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:

        int m_World[MapSize][MapSize];
    };
}

#endif