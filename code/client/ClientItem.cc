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
            case ItemType::Helmet1: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet1.png");
                break;
            case ItemType::Shield1: 
                return gResourceManager().getTexture("img/Item/Shield/Shield1.png");
                break;
            case ItemType::Chesplate1: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate1.png");
                break;
            case ItemType::SpellBook1: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook1.png");
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