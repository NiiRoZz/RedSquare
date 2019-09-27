#ifndef REDSQUARE_CLIENT_PLAYER_H
#define REDSQUARE_CLIENT_PLAYER_H

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace redsquare
{
    class Player: public gf::Entity
    {
    public:
        gf::Vector2i m_Pos;
        
        Player();
        Player( gf::Vector2i pos );

        void loadTexture();

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
        gf::Texture testTexture;
    };
}

#endif