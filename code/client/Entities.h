#ifndef REDSQUARE_CLIENT_ENTITIES_H
#define REDSQUARE_CLIENT_ENTITIES_H

#include <map>

#include <gf/Id.h>
#include <gf/Entity.h>

#include "Player.h"
#include "Monster.h"
#include "Prop.h"
#include "ItemHolder.h"

namespace redsquare
{
    class Entities: public gf::Entity
    {
    public:
        Player& addNewPlayer(gf::Id id, Player &&player);
        void removePlayer(gf::Id id);

        Monster& addNewMonster(gf::Id id, Monster &&monster);
        void removeMonster(gf::Id id);

        Prop& addNewProp(gf::Id id, Prop &&prop);
        void removeProp(gf::Id id);

        ItemHolder& addNewItemHolder(gf::Id id, ItemHolder &&itemHolder);
        void removeItemHolder(gf::Id id);

        Player* getPlayer( gf::Id playerID );
        Player* getPlayer( gf::Vector2i pos );

        Monster* getMonster( gf::Id monsterID );
        Monster* getMonster( gf::Vector2i pos );

        Prop* getProp( gf::Id propID );
        Prop* getProp( gf::Vector2i pos );
        Prop* getProp( gf::Vector2i pos, bool withInventory );

        ItemHolder* getItemHolder( gf::Id itemHolderID );

        bool monsterInRange(gf::Vector2i pos, int range) const;

        void update(gf::Time time) override;
        void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
        //All players
        std::map<gf::Id, Player> m_Players;
        //All monsters
        std::map<gf::Id, Monster> m_Monsters;
        //All props
        std::map<gf::Id, Prop> m_Props;
        //All itemHolders
        std::map<gf::Id, ItemHolder> m_ItemHolders;
    };
}

#endif //REDSQUARE_CLIENT_ENTITIES_H