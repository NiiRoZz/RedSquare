#ifndef REDSQUARE_CLIENT_WORLD_H
#define REDSQUARE_CLIENT_WORLD_H

#include "../common/Packet.h"

#include <gf/Entity.h>
#include <gf/TileLayer.h>
#include <gf/Texture.h>
#include <gf/Array2D.h>

namespace redsquare
{
    class World: public gf::Entity
    {
    public:
        static constexpr int MapSize = 16;
        static constexpr int TileSize = 16;

        World();

        void generateWorld( gf::Array2D<Tile> world );

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
        gf::Array2D<Tile> m_World;

        gf::Texture tileTexture;
        gf::TileLayer m_GroundLayer;
        gf::TileLayer m_WallLayer;
    };
}

#endif
