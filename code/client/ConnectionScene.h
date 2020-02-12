#ifndef REDSQUARE_CLIENT_CONNECTION_SCENE_H
#define REDSQUARE_CLIENT_CONNECTION_SCENE_H

#include <gf/Window.h>
#include <gf/RenderWindow.h>
#include <gf/Scene.h>
#include <gf/Widgets.h>
#include <gf/WidgetContainer.h>
#include <gf/StaticString.h>

namespace redsquare
{
    struct Scenes;

    class ConnectionScene: public gf::Scene
    {
    public:
        ConnectionScene(Scenes& scenes);

    private:
        void doHandleActions(gf::Window& window) override;
        bool doEarlyProcessEvent(gf::Event &event) override;
        void doProcessEvent(gf::Event& event) override;
        void doUpdate(gf::Time time) override;
        void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
        Scenes& m_Scenes;

        gf::Font& m_Font;

        bool m_ConnectionAsked;
        gf::StaticString<255> m_HostNameBuffer;
        gf::StaticString<255> m_PortBuffer;
        gf::StaticString<255> m_NameBuffer;
        gf::SpriteWidget m_PlayerWidget;
        gf::WidgetContainer m_Container;
        gf::TextWidget m_GaucheButton;
        gf::TextWidget m_DroiteButton;

    };
}


#endif //REDSQUARE_CLIENT_CONNECTION_SCENE_H