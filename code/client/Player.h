#ifndef REDSQUARE_CLIENT_PLAYER_H
#define REDSQUARE_CLIENT_PLAYER_H

#include "../common/Entity.h"
#include "Monster.h"

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Id.h>
#include <gf/Map.h>

namespace redsquare
{
    class Player: public gf::Entity, public redsquare::Entity
    {
    public:
        /* Characteristic */
        int m_ManaPoint;
        int m_MaxManaPoint;
        int m_XP;
        int m_Max_XP;
        /* Characteristic*/
        
        Player();
        Player( gf::Vector2i pos, uint8_t type );

        void loadTexture(uint8_t type = 0);

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        bool canAttack( gf::Vector2i targetPos, std::map<gf::Id, Player> &players, std::map<gf::Id, Monster> &monsters );
        bool canMove( gf::Vector2i targetPos, std::map<gf::Id, Player> &players, std::map<gf::Id, Monster> &monsters, gf::SquareMap &map );

    private:
        gf::Texture playerTexture;

    };
}

#endif
