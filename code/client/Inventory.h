#ifndef REDSQUARE_CLIENT_INVENTORY_H
#define REDSQUARE_CLIENT_INVENTORY_H

#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>

namespace redsquare
{
    class Inventory
    {
    public:
        Inventory(gf::Font &font);

        void update(gf::Time time);

        void render(gf::RenderTarget& target, const gf::RenderStates& states);

        void processEvent(const gf::Event &event);

    private:

        gf::UI m_UI;
    };
}


#endif