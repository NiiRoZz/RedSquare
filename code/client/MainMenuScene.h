#ifndef REDSQUARE_CLIENT_MAINMENU_SCENE_H
#define REDSQUARE_CLIENT_MAINMENU_SCENE_H

#include <gf/Window.h>
#include <gf/RenderWindow.h>
#include <gf/Scene.h>
#include <gf/Widgets.h>
#include <gf/WidgetContainer.h>

namespace redsquare
{
    struct Scenes;

    enum class MenuChoice {
        None,
        Start,
        Quit,
    };

    class MainMenuScene: public gf::Scene
    {
    public:
        MainMenuScene(Scenes& scenes);

    private:
        void doHandleActions(gf::Window& window) override;
        void doProcessEvent(gf::Event& event) override;
        void doUpdate(gf::Time time) override;
        void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
        Scenes& m_Scenes;

        gf::Font& m_Font;
        MenuChoice m_MenuChoice;
        gf::WidgetContainer m_Container;
        gf::TextWidget m_StartWidget;
        gf::TextWidget m_QuitWidget;

    };
}


#endif //REDSQUARE_CLIENT_MAINMENU_SCENE_H