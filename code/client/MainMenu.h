#ifndef REDSQUARE_CLIENT_MAINMENU_H
#define REDSQUARE_CLIENT_MAINMENU_H

#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>

namespace redsquare
{
    class MainMenu
    {
    public:
        MainMenu(gf::Font &font);

        void update(gf::Time time);

        void render(gf::RenderTarget& target, const gf::RenderStates& states);

        void processEvent(const gf::Event &event);

        bool m_ShowMainMenu;

    private:
        gf::UI m_UI;

    };
}


#endif