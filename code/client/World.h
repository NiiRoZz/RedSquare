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
        static constexpr int MapSize = 16; // size of the map composed of tile
        static constexpr int TileSize = 16; // size of a tile
        static constexpr int TileSetSize = 16; // size of a tileset

        World();

        void generateWorld( gf::Array2D<Tile> world );

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
        gf::Array2D<Tile> m_World;

        gf::Texture tileTexture;
        gf::TileLayer m_TileSet; //
    };
}

#endif
