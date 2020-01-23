#ifndef REDSQUARE_CLIENT_MONSTER_H
#define REDSQUARE_CLIENT_MONSTER_H

#include "ClientEntity.h"

#include <gf/Texture.h>
#include <gf/Font.h>

namespace redsquare
{
    class Monster: public ClientEntity
    {
    public:
        Monster( gf::Id entityID, EntitySubType entitySubType, gf::Vector2i pos );

        void loadTexture(const EntitySubType type = EntitySubType::Magus);

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
        gf::Texture *monsterTexture;
        gf::Font &m_Font;
    };
}

#endif