#ifndef REDSQUARE_CLIENT_WORLD_H
#define REDSQUARE_CLIENT_WORLD_H

#include "../common/ProtocolData.h"
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
        World();
        
        void generateWorld( gf::Array2D<Tile> world );
        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
        int rigthTile(gf::Vector2i tile);

        void setWalkableFromEntity(redsquare::Entity *entity, bool walkable);
        void setTransparentFromEntity(redsquare::Entity *entity, bool transparent);

        static constexpr int MapSize = 100; // size of the map composed of tile
        static constexpr int TileSize = 16; // size of a tile
        static constexpr int TileSetSize = 16; // size of a tileset

        gf::SquareMap m_SquareMap;
        gf::Array2D<Tile> m_World;

    private:
        gf::TileLayer m_TileSet;
        gf::Texture &m_TileTexture;
    };
}

#endif
