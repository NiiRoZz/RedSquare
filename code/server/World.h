#ifndef REDSQUARE_SERVER_WORLD_H
#define REDSQUARE_SERVER_WORLD_H

namespace redsquare
{
    class World
    {
    public:
        static constexpr int MapSize = 128;
        static constexpr int TileSize = 16;
        World();

    private:
        int m_World[MapSize][MapSize];
    };
}

#endif