#ifndef REDSQUARE_CLIENT_MONSTER_H
#define REDSQUARE_CLIENT_MONSTER_H

#include "ClientEntity.h"

#include <gf/Texture.h>
#include <gf/Font.h>
#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>

namespace redsquare
{
    class Monster: public ClientEntity
    {
    public:
        Monster( gf::Id entityID, EntitySubType entitySubType, gf::Vector2i pos );

        void render(gf::RenderTarget& target, const gf::RenderStates& states);
        void update(gf::Time time);

    private:
        gf::Font &m_Font;
    };
}

#endif