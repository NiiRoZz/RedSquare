#ifndef REDSQUARE_CLIENT_MAINMENU_H
#define REDSQUARE_CLIENT_MAINMENU_H

#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/StaticString.h>
#include <imgui.h>
#include <imgui_impl_gf.h>
#include <gf/Text.h>
#include "InventoryWidget.h"

namespace redsquare
{
    constexpr ImGuiWindowFlags DefaultWindowFlagsTest = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;

    class MainMenu
    {
    public:
        MainMenu(gf::Font &font);

        void update(gf::Time time);

        void render(gf::RenderTarget& target, const gf::RenderStates& states);

        void processEvent(const gf::Event &event);

        bool m_ShowMainMenu;

        

    private:
        gf::StaticString<15> m_LineBufferName;
        gf::StaticString<20> m_LineBufferIp;
        InventoryWidget m_PlayerWidget;

    };
}


#endif