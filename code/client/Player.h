#ifndef REDSQUARE_CLIENT_PLAYER_H
#define REDSQUARE_CLIENT_PLAYER_H

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace redsquare
{
    class Player: public gf::Entity
    {
    public:
        Player();
        Player( gf::Vector2i pos );

        void setPos( gf::Vector2i pos );
        void loadTexture();
        gf::Vector2i getPos();

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
        gf::Vector2i m_Pos;

        gf::Texture testTexture;
    };
}

#endif