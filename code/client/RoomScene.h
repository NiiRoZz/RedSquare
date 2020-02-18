#ifndef REDSQUARE_ROOM_SCENE_H
#define REDSQUARE_ROOM_SCENE_H

#include <cstdint>
#include <vector>

#include <gf/ResourceManager.h>
#include <gf/Scene.h>
#include <gf/Window.h>
#include <gf/RenderWindow.h>
#include <gf/Widgets.h>
#include <gf/WidgetContainer.h>
#include <gf/Text.h>
#include <gf/StaticString.h>

#include "../common/ProtocolData.h"
#include "ClientNetwork.h"
#include "LobbyChat.h"

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
        void displayNextSubEntityTexture(int offset);

    private:
        Scenes& m_Scenes;
        ClientNetwork& m_Network;

        LobbyChat m_Chat;

        bool m_Ready;
        gf::Font& m_Font;

        std::vector<PlayerData> m_Players;
        std::vector<const char *> m_PlayersView;

        gf::SpriteWidget m_PlayerWidget;
        gf::WidgetContainer m_Container;
        gf::TextWidget m_GaucheButton;
        gf::TextWidget m_DroiteButton;

        EntitySubType m_DisplayEntitySubType;

        gf::Text m_InfoText;
    };

}

#endif // REDSQUARE_ROOM_SCENE_H
