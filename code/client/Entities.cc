#include "Entities.h"

namespace redsquare
{
    void Entities::initialize()
    {
        m_Players.clear();
        m_Props.clear();
        m_Monsters.clear();
        m_ItemHolders.clear();
    }

    Player& Entities::addNewPlayer(gf::Id id, Player &&player)
    {
        auto it = m_Players.insert( std::make_pair( id, std::move(player) ) );
        assert(it.second);

        return it.first->second;
    }

    const std::map<gf::Id, Player>& Entities::getPlayers() const
    {
        return m_Players;
    }

    void Entities::removePlayer(gf::Id id)
    {
        assert( m_Players.erase(id) > 0u );
    }

    Monster& Entities::addNewMonster(gf::Id id, Monster &&monster)
    {
        auto it = m_Monsters.insert( std::make_pair( id, std::move(monster) ) );
        assert(it.second);

        return it.first->second;
    }

    void Entities::removeMonster(gf::Id id)
    {
        assert( m_Monsters.erase(id) > 0u );
    }

    Prop& Entities::addNewProp(gf::Id id, Prop &&prop)
    {
        auto it = m_Props.insert( std::make_pair( id, std::move(prop) ) );
        assert(it.second);

        return it.first->second;
    }

    void Entities::removeProp(gf::Id id)
    {
        assert( m_Props.erase(id) > 0u );
    }

    ItemHolder& Entities::addNewItemHolder(gf::Id id, ItemHolder &&itemHolder)
    {
        auto it = m_ItemHolders.insert( std::make_pair( id, std::move(itemHolder) ) );
        assert(it.second);

        return it.first->second;
    }

    void Entities::removeItemHolder(gf::Id id)
    {
        assert( m_ItemHolders.erase(id) > 0u );
    }

    Player* Entities::getPlayer( gf::Id playerID )
    {
        auto player = m_Players.find( playerID );

        if ( player != m_Players.end() )
        {
            return &player->second;
        }

        return nullptr;
    }

    Player* Entities::getPlayer( gf::Vector2i pos )
    {
        for(auto &player: m_Players)
        {
            if (player.second.isInsideMe(pos))
            {
                return &player.second;
            }
        }

        return nullptr;
    }

    Monster* Entities::getMonster( gf::Id monsterID )
    {
        auto monster = m_Monsters.find( monsterID );

        if ( monster != m_Monsters.end() )
        {
            return &monster->second;
        }

        return nullptr;
    }

    Monster* Entities::getMonster( gf::Vector2i pos )
    {
        for(auto &monster: m_Monsters)
        {
            if (monster.second.isInsideMe(pos))
            {
                return &monster.second;
            }
        }

        return nullptr;
    }

    Prop* Entities::getProp( gf::Id propID )
    {
        auto prop = m_Props.find( propID );

        if ( prop != m_Props.end() )
        {
            return &prop->second;
        }

        return nullptr;
    }

    Prop* Entities::getProp( gf::Vector2i pos )
    {
        for(auto &prop: m_Props)
        {
            if (prop.second.isInsideMe(pos))
            {
                return &prop.second;
            }
        }

        return nullptr;
    }

    Prop* Entities::getProp( gf::Vector2i pos, bool withInventory )
    {
        for(auto &prop: m_Props)
        {
            if (prop.second.isInsideMe(pos) && ((!withInventory && !(prop.second.haveInventory())) || (withInventory && prop.second.haveInventory())))
            {
                return &prop.second;
            }
        }

        return nullptr;
    }

    ItemHolder* Entities::getItemHolder( gf::Id itemHolderID )
    {
        auto itemHolder = m_ItemHolders.find( itemHolderID );

        if ( itemHolder != m_ItemHolders.end() )
        {
            return &itemHolder->second;
        }

        return nullptr;
    }

    bool Entities::monsterInRange(gf::Vector2i pos, int range) const
    {
        gf::Distance2<int> distFn = gf::manhattanDistance<int, 2>;

        for(auto &monster: m_Monsters)
        {
            if ( distFn(pos, monster.second.m_Pos) <= range )
            {
                return true;
            }
        }

        return false;
    }

    void Entities::update(gf::Time time)
    {
        for(auto &prop: m_Props)
        {
            prop.second.update(time);
        }
    }

    void Entities::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        for(auto &itemHolder: m_ItemHolders)
        {
            itemHolder.second.render(target,states);
        }

        for(auto &prop: m_Props)
        {
            prop.second.render(target,states);
        }

        for(auto &monster: m_Monsters)
        {
            monster.second.render(target,states);
        }

        for(auto &player: m_Players)
        {
            player.second.render(target,states);
        }
    }
}