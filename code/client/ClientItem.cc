#include "ClientItem.h"
#include "../common/Singletons.h"

namespace redsquare
{
    gf::Texture& ClientItem::getTexture( ItemType type )
    {
        switch (type)
        {
            case ItemType::Sword1: 
                return gResourceManager().getTexture("img/Item/Sword/Sword1.png");
                break;
            case ItemType::Sword2: 
                return gResourceManager().getTexture("img/Item/Sword/Sword2.png");
                break;
            case ItemType::Sword3: 
                return gResourceManager().getTexture("img/Item/Sword/Sword2.png");
                break;
            case ItemType::Staff1: 
                return gResourceManager().getTexture("img/Item/Staff/Staff1.png");
                break;
            case ItemType::Bow1: 
                return gResourceManager().getTexture("img/Item/Bow/Bow1.png");
                break;
        }

        return gResourceManager().getTexture("img/attackCursor.png");
    }

    ClientItem::ClientItem(ItemType type, uint8_t slotMask)
    : Item(type, slotMask)
    , m_Icon(ClientItem::getTexture(type))
    {
        
    }

    gf::Texture& ClientItem::getIcon() const
    {
        return m_Icon;
    }
}