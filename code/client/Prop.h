#ifndef REDSQUARE_CLIENT_PROP_H
#define REDSQUARE_CLIENT_PROP_H

#include "../common/Entity.h"

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace redsquare
{
    class Prop: public gf::Entity, public redsquare::Entity
    {
    public:
        Prop( gf::Id entityID, EntityClass type, gf::Vector2i pos );

        void loadTexture(const EntityClass type = EntityClass::Chair);

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
        gf::Texture propTexture;
    };
}

#endif