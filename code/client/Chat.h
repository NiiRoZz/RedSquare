#ifndef REDSQUARE_CLIENT_CHAT_H
#define REDSQUARE_CLIENT_CHAT_H

#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>

namespace redsquare
{
    class Chat
    {
    public:
        Chat(gf::Font &font);

        void update(gf::Time time);

        void render(gf::RenderTarget& target, const gf::RenderStates& states);

        void processEvent(const gf::Event &event);

        bool m_HoveringChat;

    private:
        gf::Shader m_ChatShader;
        gf::UI m_UI;

    };
}


#endif