#ifndef REDSQUARE_CLIENT_MONSTER_H
#define REDSQUARE_CLIENT_MONSTER_H

#include "../common/Entity.h"

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace redsquare
{
    class Monster: public gf::Entity, public redsquare::Entity
    {
    public:
        Monster();

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
    };
}

#endif