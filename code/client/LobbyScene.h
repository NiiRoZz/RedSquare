#ifndef REDSQUARE_LOBBY_SCENE_H
#define REDSQUARE_LOBBY_SCENE_H

#include <gf/Scene.h>
#include <gf/StaticString.h>

#include "../common/ProtocolData.h"
#include "ClientNetwork.h"
#include "LobbyChat.h"

namespace redsquare
{
    struct Scenes;

    class LobbyScene : public gf::Scene
    {
    public:
        LobbyScene(Scenes& scenes, ClientNetwork& network);

    private:
        void doProcessEvent(gf::Event& event) override;
        void doUpdate(gf::Time time) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;

    private:
        Scenes& m_Scenes;
        ClientNetwork& m_Network;
        LobbyChat m_Chat;

        gf::StaticString<255> m_RoomBuffer;
        gf::StaticString<255> m_NameBuffer;

        std::vector<PlayerData> m_Players;
        std::vector<const char *> m_PlayersView;
        std::vector<RoomData> m_Rooms;
        std::vector<const char *> m_RoomsView;
        int m_SelectedRoom;
        RoomData m_SelectedRoomData;
        int m_SelectedNmbPlayers;
    };
}

#endif // REDSQUARE_LOBBY_SCENE_H
