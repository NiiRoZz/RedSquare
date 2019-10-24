#ifndef REDSQUARE_CLIENT_MONSTER_H
#define REDSQUARE_CLIENT_MONSTER_H

#include "../common/Entity.h"

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Font.h>

namespace redsquare
{
    class Monster: public gf::Entity, public redsquare::Entity
    {
    public:
        Monster( gf::Id entityID );
        Monster( gf::Id entityID, EntityClass type, gf::Vector2i pos );

        void loadTexture(const EntityClass type = EntityClass::Magus);

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
        gf::Texture monsterTexture;
        gf::Font m_Font;
    };
}

#endif