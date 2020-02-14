#ifndef REDSQUARE_ROOM_SCENE_H
#define REDSQUARE_ROOM_SCENE_H

#include <cstdint>
#include <vector>

#include <gf/ResourceManager.h>
#include <gf/Scene.h>

#include "../common/ProtocolData.h"

#include "ClientNetwork.h"

namespace redsquare 
{
    struct Scenes;

    class RoomScene : public gf::Scene
    {
    public:
        RoomScene(Scenes& scenes, ClientNetwork& network);

        void startRoom();

    private:
        void doProcessEvent(gf::Event& event) override;
        void doUpdate(gf::Time time) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;

    private:
        Scenes& m_Scenes;
        ClientNetwork& m_Network;

        bool m_Ready;

        std::vector<PlayerData> m_Players;
    };

}

#endif // REDSQUARE_ROOM_SCENE_H
