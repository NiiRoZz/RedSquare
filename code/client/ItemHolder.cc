#include "ItemHolder.h"

#include "World.h"
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>

namespace redsquare
{
    ItemHolder::ItemHolder( gf::Id entityID, ItemType itemHoldingType, gf::Vector2i pos )
    : ClientEntity(entityID, EntityType::ItemHolder, EntitySubType::Unknow)
    {
        m_Pos = pos;
        m_EntityTexture = &(ClientItem::getTexture(itemHoldingType));
    }

    void ItemHolder::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Sprite sprite;
        sprite.setPosition( m_Pos * World::TileSize );
        sprite.setScale( 1 );
        sprite.setTexture( *m_EntityTexture );
        target.draw(sprite, states);
    }
}