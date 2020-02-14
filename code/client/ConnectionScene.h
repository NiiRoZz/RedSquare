#ifndef REDSQUARE_CLIENT_CONNECTION_SCENE_H
#define REDSQUARE_CLIENT_CONNECTION_SCENE_H

#include <gf/Window.h>
#include <gf/RenderWindow.h>
#include <gf/Scene.h>
#include <gf/Widgets.h>
#include <gf/WidgetContainer.h>
#include <gf/StaticString.h>

#include "../common/Protocol.h"
#include "ClientNetwork.h"

namespace redsquare
{
    struct Scenes;

    class ConnectionScene: public gf::Scene
    {
    public:
        ConnectionScene(Scenes& scenes, ClientNetwork& network);

    private:
        void doHandleActions(gf::Window& window) override;
        bool doEarlyProcessEvent(gf::Event &event) override;
        void doProcessEvent(gf::Event& event) override;
        void doUpdate(gf::Time time) override;
        void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
        Scenes& m_Scenes;
        ClientNetwork& m_Network;
        
        gf::Font& m_Font;

        bool m_ConnectionAsked;
        
        gf::StaticString<255> m_HostNameBuffer;
        gf::StaticString<255> m_NameBuffer;
        gf::WidgetContainer m_Container;


    };
}


#endif //REDSQUARE_CLIENT_CONNECTION_SCENE_H