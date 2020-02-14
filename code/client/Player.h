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
    class GameScene;

    class Player: public ClientEntity
    {
    public:
        /* Characteristic */
        int m_ManaPoint;
        int m_MaxManaPoint;
        int m_XP;
        int m_Max_XP;
        /* Characteristic*/

        std::vector<SpellType> m_SpellTab;
        
        Player( gf::Id entityID, EntitySubType entitySubType, gf::Vector2i pos );

        void render(gf::RenderTarget& target, const gf::RenderStates& states);

        bool canAttack( gf::Vector2i targetPos, GameScene &game);
        bool canMove( gf::Vector2i targetPos, gf::SquareMap &map );
        bool canOpenTargetInventory( gf::Vector2i targetPos, GameScene &game);

        std::string getName() const;

    private:
        gf::Font &m_Font;
        std::string m_Name;

    };
}

#endif
 