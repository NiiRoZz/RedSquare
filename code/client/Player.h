#ifndef REDSQUARE_CLIENT_PLAYER_H
#define REDSQUARE_CLIENT_PLAYER_H

#include "ClientEntity.h"
#include "Monster.h"
#include "Prop.h"

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Id.h>
#include <gf/Map.h>
#include <gf/Font.h>

namespace redsquare
{
    // forward class
    class Game;

    class Player: public ClientEntity
    {
    public:
        /* Characteristic */
        int m_ManaPoint;
        int m_MaxManaPoint;
        int m_XP;
        int m_Max_XP;
        /* Characteristic*/

        std::array<SpellType, MAX_SPELL_PER_PLAYER> m_SpellTab;
        
        Player( gf::Id entityID, EntitySubType entitySubType, gf::Vector2i pos );

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        bool canAttack( gf::Vector2i targetPos, Game &game);
        bool canMove( gf::Vector2i targetPos, std::map<gf::Id, Player> &players, std::map<gf::Id, Monster> &monsters, std::map<gf::Id, Prop> &props, gf::SquareMap &map );
        bool canOpenTargetInventory( gf::Vector2i targetPos, Game &game);

    private:
        gf::Font &m_Font;

    };
}

#endif
 