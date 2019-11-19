#ifndef REDSQUARE_CLIENT_WORLD_H
#define REDSQUARE_CLIENT_WORLD_H

#include "../common/Packet.h"
#include "../common/Entity.h"

#include <gf/Entity.h>
#include <gf/TileLayer.h>
#include <gf/Texture.h>
#include <gf/Array2D.h>
#include <gf/Map.h>

namespace redsquare
{
    class World: public gf::Entity
    {
    public:
        static constexpr int MapSize = 100; // size of the map composed of tile
        static constexpr int TileSize = 16; // size of a tile
        static constexpr int TileSetSize = 16; // size of a tileset

        gf::SquareMap m_SquareMap;

        void generateWorld( gf::Array2D<Tile> world );
        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
        int rigthTile(gf::Vector2i tile);

        void setWalkableFromEntity(redsquare::Entity *entity, bool walkable);

        World();

    private:
        gf::Array2D<Tile> m_World;

        gf::Texture &m_TileTexture;
        gf::TileLayer m_TileSet;

        /*gf::Texture tileTexture2;
        gf::TileLayer m_TileSet2; TODO */
    };
}

#endif
