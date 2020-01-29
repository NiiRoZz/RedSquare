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
                return gResourceManager().getTexture("img/Item/Sword/Sword3.png");
                break;
            case ItemType::Sword4: 
                return gResourceManager().getTexture("img/Item/Sword/Sword4.png");
                break;
            case ItemType::Sword5: 
                return gResourceManager().getTexture("img/Item/Sword/Sword5.png");
                break;
            case ItemType::Sword6: 
                return gResourceManager().getTexture("img/Item/Sword/Sword6.png");
                break;
            case ItemType::Sword7: 
                return gResourceManager().getTexture("img/Item/Sword/Sword7.png");
                break;
            case ItemType::Sword8: 
                return gResourceManager().getTexture("img/Item/Sword/Sword8.png");
                break;
            case ItemType::Sword9: 
                return gResourceManager().getTexture("img/Item/Sword/Sword9.png");
                break;
            case ItemType::Sword10: 
                return gResourceManager().getTexture("img/Item/Sword/Sword10.png");
                break;
            case ItemType::Sword11: 
                return gResourceManager().getTexture("img/Item/Sword/Sword11.png");
                break;
            case ItemType::Sword12: 
                return gResourceManager().getTexture("img/Item/Sword/Sword12.png");
                break;
            case ItemType::Sword13: 
                return gResourceManager().getTexture("img/Item/Sword/Sword13.png");
                break;
            case ItemType::Sword14: 
                return gResourceManager().getTexture("img/Item/Sword/Sword14.png");
                break;
            case ItemType::Sword15: 
                return gResourceManager().getTexture("img/Item/Sword/Sword15.png");
                break;

            case ItemType::Staff1: 
                return gResourceManager().getTexture("img/Item/Staff/Staff1.png");
                break;
            case ItemType::Staff2: 
                return gResourceManager().getTexture("img/Item/Staff/Staff2.png");
                break;
            case ItemType::Staff3: 
                return gResourceManager().getTexture("img/Item/Staff/Staff3.png");
                break;
            case ItemType::Staff4: 
                return gResourceManager().getTexture("img/Item/Staff/Staff4.png");
                break;
            case ItemType::Staff5: 
                return gResourceManager().getTexture("img/Item/Staff/Staff5.png");
                break;
            case ItemType::Staff6: 
                return gResourceManager().getTexture("img/Item/Staff/Staff6.png");
                break;
            case ItemType::Staff7: 
                return gResourceManager().getTexture("img/Item/Staff/Staff7.png");
                break;
            case ItemType::Staff8: 
                return gResourceManager().getTexture("img/Item/Staff/Staff8.png");
                break;
            case ItemType::Staff9: 
                return gResourceManager().getTexture("img/Item/Staff/Staff9.png");
                break;
            case ItemType::Staff10: 
                return gResourceManager().getTexture("img/Item/Staff/Staff10.png");
                break;
            case ItemType::Staff11: 
                return gResourceManager().getTexture("img/Item/Staff/Staff11.png");
                break;
            case ItemType::Staff12: 
                return gResourceManager().getTexture("img/Item/Staff/Staff12.png");
                break;
            case ItemType::Staff13: 
                return gResourceManager().getTexture("img/Item/Staff/Staff13.png");
                break;
            case ItemType::Staff14: 
                return gResourceManager().getTexture("img/Item/Staff/Staff14.png");
                break;
            case ItemType::Staff15: 
                return gResourceManager().getTexture("img/Item/Staff/Staff15.png");
                break;

            case ItemType::Bow1: 
                return gResourceManager().getTexture("img/Item/Bow/Bow1.png");
                break;
            case ItemType::Bow2: 
                return gResourceManager().getTexture("img/Item/Bow/Bow2.png");
                break;
            case ItemType::Bow3: 
                return gResourceManager().getTexture("img/Item/Bow/Bow3.png");
                break;
            case ItemType::Bow4: 
                return gResourceManager().getTexture("img/Item/Bow/Bow4.png");
                break;
            case ItemType::Bow5: 
                return gResourceManager().getTexture("img/Item/Bow/Bow5.png");
                break;
            case ItemType::Bow6: 
                return gResourceManager().getTexture("img/Item/Bow/Bow6.png");
                break;
            case ItemType::Bow7: 
                return gResourceManager().getTexture("img/Item/Bow/Bow7.png");
                break;
            case ItemType::Bow8: 
                return gResourceManager().getTexture("img/Item/Bow/Bow8.png");
                break;
            case ItemType::Bow9: 
                return gResourceManager().getTexture("img/Item/Bow/Bow9.png");
                break;
            case ItemType::Bow10: 
                return gResourceManager().getTexture("img/Item/Bow/Bow10.png");
                break;
            case ItemType::Bow11: 
                return gResourceManager().getTexture("img/Item/Bow/Bow11.png");
                break;
            case ItemType::Bow12: 
                return gResourceManager().getTexture("img/Item/Bow/Bow12.png");
                break;
            case ItemType::Bow13: 
                return gResourceManager().getTexture("img/Item/Bow/Bow13.png");
                break;
            case ItemType::Bow14: 
                return gResourceManager().getTexture("img/Item/Bow/Bow14.png");
                break;
            case ItemType::Bow15: 
                return gResourceManager().getTexture("img/Item/Bow/Bow15.png");
                break;

            case ItemType::SpellBook1: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook1.png");
                break;
            case ItemType::SpellBook2: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook2.png");
                break;
            case ItemType::SpellBook3: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook3.png");
                break;
            case ItemType::SpellBook4: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook4.png");
                break;
            case ItemType::SpellBook5: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook5.png");
                break;
            case ItemType::SpellBook6: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook6.png");
                break;
            case ItemType::SpellBook7: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook7.png");
                break;
            case ItemType::SpellBook8: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook8.png");
                break;
            case ItemType::SpellBook9: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook9.png");
                break;
            case ItemType::SpellBook10: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook10.png");
                break;
            case ItemType::SpellBook11: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook11.png");
                break;
            case ItemType::SpellBook12: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook12.png");
                break;
            case ItemType::SpellBook13: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook13.png");
                break;
            case ItemType::SpellBook14: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook14.png");
                break;
            case ItemType::SpellBook15: 
                return gResourceManager().getTexture("img/Item/SpellBook/SpellBook15.png");
                break;

            case ItemType::Shield1: 
                return gResourceManager().getTexture("img/Item/Shield/Shield1.png");
                break;
            case ItemType::Shield2: 
                return gResourceManager().getTexture("img/Item/Shield/Shield2.png");
                break;
            case ItemType::Shield3: 
                return gResourceManager().getTexture("img/Item/Shield/Shield3.png");
                break;
            case ItemType::Shield4: 
                return gResourceManager().getTexture("img/Item/Shield/Shield4.png");
                break;
            case ItemType::Shield5: 
                return gResourceManager().getTexture("img/Item/Shield/Shield5.png");
                break;
            case ItemType::Shield6: 
                return gResourceManager().getTexture("img/Item/Shield/Shield6.png");
                break;
            case ItemType::Shield7: 
                return gResourceManager().getTexture("img/Item/Shield/Shield7.png");
                break;
            case ItemType::Shield8: 
                return gResourceManager().getTexture("img/Item/Shield/Shield8.png");
                break;
            case ItemType::Shield9: 
                return gResourceManager().getTexture("img/Item/Shield/Shield9.png");
                break;
            case ItemType::Shield10: 
                return gResourceManager().getTexture("img/Item/Shield/Shield10.png");
                break;
            case ItemType::Shield11: 
                return gResourceManager().getTexture("img/Item/Shield/Shield11.png");
                break;
            case ItemType::Shield12: 
                return gResourceManager().getTexture("img/Item/Shield/Shield12.png");
                break;
            case ItemType::Shield13: 
                return gResourceManager().getTexture("img/Item/Shield/Shield13.png");
                break;
            case ItemType::Shield14: 
                return gResourceManager().getTexture("img/Item/Shield/Shield14.png");
                break;
            case ItemType::Shield15: 
                return gResourceManager().getTexture("img/Item/Shield/Shield15.png");
                break;

            case ItemType::Helmet1: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet1.png");
                break;
            case ItemType::Helmet2: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet2.png");
                break;
            case ItemType::Helmet3: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet3.png");
                break;
            case ItemType::Helmet4: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet4.png");
                break;
            case ItemType::Helmet5: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet5.png");
                break;
            case ItemType::Helmet6: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet6.png");
                break;
            case ItemType::Helmet7: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet7.png");
                break;
            case ItemType::Helmet8: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet8.png");
                break;
            case ItemType::Helmet9: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet9.png");
                break;
            case ItemType::Helmet10: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet10.png");
                break;
            case ItemType::Helmet11: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet11.png");
                break;
            case ItemType::Helmet12: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet12.png");
                break;
            case ItemType::Helmet13: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet13.png");
                break;
            case ItemType::Helmet14: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet14.png");
                break;
            case ItemType::Helmet15: 
                return gResourceManager().getTexture("img/Item/Helmet/Helmet15.png");
                break;

            case ItemType::Chesplate1: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate1.png");
                break;
            case ItemType::Chesplate2: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate2.png");
                break;
            case ItemType::Chesplate3: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate3.png");
                break;
            case ItemType::Chesplate4: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate4.png");
                break;
            case ItemType::Chesplate5: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate5.png");
                break;
            case ItemType::Chesplate6: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate6.png");
                break;
            case ItemType::Chesplate7: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate7.png");
                break;
            case ItemType::Chesplate8: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate8.png");
                break;
            case ItemType::Chesplate9: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate9.png");
                break;
            case ItemType::Chesplate10: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate10.png");
                break;
            case ItemType::Chesplate11: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate11.png");
                break;
            case ItemType::Chesplate12: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate12.png");
                break;
            case ItemType::Chesplate13: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate13.png");
                break;
            case ItemType::Chesplate14: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate14.png");
                break;
            case ItemType::Chesplate15: 
                return gResourceManager().getTexture("img/Item/Chesplate/Chesplate15.png");
                break;

            case ItemType::Legging1: 
                return gResourceManager().getTexture("img/Item/Legging/Legging1.png");
                break;
            case ItemType::Legging2: 
                return gResourceManager().getTexture("img/Item/Legging/Legging2.png");
                break;
            case ItemType::Legging3: 
                return gResourceManager().getTexture("img/Item/Legging/Legging3.png");
                break;
            case ItemType::Legging4: 
                return gResourceManager().getTexture("img/Item/Legging/Legging4.png");
                break;
            case ItemType::Legging5: 
                return gResourceManager().getTexture("img/Item/Legging/Legging5.png");
                break;
            case ItemType::Legging6: 
                return gResourceManager().getTexture("img/Item/Legging/Legging6.png");
                break;
            case ItemType::Legging7: 
                return gResourceManager().getTexture("img/Item/Legging/Legging7.png");
                break;
            case ItemType::Legging8: 
                return gResourceManager().getTexture("img/Item/Legging/Legging8.png");
                break;
            case ItemType::Legging9: 
                return gResourceManager().getTexture("img/Item/Legging/Legging9.png");
                break;
            case ItemType::Legging11: 
                return gResourceManager().getTexture("img/Item/Legging/Legging11.png");
                break;
            case ItemType::Legging12: 
                return gResourceManager().getTexture("img/Item/Legging/Legging12.png");
                break;
            case ItemType::Legging13: 
                return gResourceManager().getTexture("img/Item/Legging/Legging13.png");
                break;
            case ItemType::Legging14: 
                return gResourceManager().getTexture("img/Item/Legging/Legging14.png");
                break;
            case ItemType::Legging15: 
                return gResourceManager().getTexture("img/Item/Legging/Legging15.png");
                break;

            case ItemType::Boot1: 
                return gResourceManager().getTexture("img/Item/Boot/Boot1.png");
                break;
            case ItemType::Boot2: 
                return gResourceManager().getTexture("img/Item/Boot/Boot2.png");
                break;
            case ItemType::Boot3: 
                return gResourceManager().getTexture("img/Item/Boot/Boot3.png");
                break;
            case ItemType::Boot4: 
                return gResourceManager().getTexture("img/Item/Boot/Boot4.png");
                break;
            case ItemType::Boot5: 
                return gResourceManager().getTexture("img/Item/Boot/Boot5.png");
                break;
            case ItemType::Boot6: 
                return gResourceManager().getTexture("img/Item/Boot/Boot6.png");
                break;
            case ItemType::Boot7: 
                return gResourceManager().getTexture("img/Item/Boot/Boot7.png");
                break;
            case ItemType::Boot8: 
                return gResourceManager().getTexture("img/Item/Boot/Boot8.png");
                break;
            case ItemType::Boot9: 
                return gResourceManager().getTexture("img/Item/Boot/Boot9.png");
                break;
            case ItemType::Boot10: 
                return gResourceManager().getTexture("img/Item/Boot/Boot10.png");
                break;
            case ItemType::Boot11: 
                return gResourceManager().getTexture("img/Item/Boot/Boot11.png");
                break;
            case ItemType::Boot12: 
                return gResourceManager().getTexture("img/Item/Boot/Boot12.png");
                break;
            case ItemType::Boot13: 
                return gResourceManager().getTexture("img/Item/Boot/Boot13.png");
                break;
            case ItemType::Boot14: 
                return gResourceManager().getTexture("img/Item/Boot/Boot14.png");
                break;
            case ItemType::Boot15: 
                return gResourceManager().getTexture("img/Item/Boot/Boot15.png");
                break;

            case ItemType::ManaPot1: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/ManaPot/ManaPot1.png");
                break;
            case ItemType::ManaPot2: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/ManaPot/ManaPot2.png");
                break;
            case ItemType::ManaPot3: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/ManaPot/ManaPot3.png");
                break;


            case ItemType::HealthPot1: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/HealthPot/HealthPot1.png");
                break;
            case ItemType::HealthPot2: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/HealthPot/HealthPot2.png");
                break;
            case ItemType::HealthPot3: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/HealthPot/HealthPot3.png");
                break;


            case ItemType::EnergyPot1: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/EnergyPot/EnergyPot1.png");
                break;
            case ItemType::EnergyPot2: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/EnergyPot/EnergyPot2.png");
                break;
            case ItemType::EnergyPot3: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/EnergyPot/EnergyPot3.png");
                break;
                

            case ItemType::BoostAttack1: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostAttack/BoostAttack1.png");
                break;
            case ItemType::BoostAttack2: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostAttack/BoostAttack2.png");
                break;
            case ItemType::BoostAttack3: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostAttack/BoostAttack3.png");
                break;
                

            case ItemType::BoostDefense1: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostDefense/BoostDefense1.png");
                break;
            case ItemType::BoostDefense2: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostDefense/BoostDefense2.png");
                break;
            case ItemType::BoostDefense3: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostDefense/BoostDefense3.png");
                break;
                

            case ItemType::BoostHP1: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostHealth/BoostHealth1.png");
                break;
            case ItemType::BoostHP2: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostHealth/BoostHealth2.png");
                break;
            case ItemType::BoostHP3: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostHealth/BoostHealth3.png");
                break;
                

            case ItemType::BoostMana1: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostMana/BoostMana1.png");
                break;
            case ItemType::BoostMana2: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostMana/BoostMana2.png");
                break;
            case ItemType::BoostMana3: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostMana/BoostMana3.png");
                break;
                

            case ItemType::BoostXP1: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostXP/BoostXP1.png");
                break;
            case ItemType::BoostXP2: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostXP/BoostXP2.png");
                break;
            case ItemType::BoostXP3: 
                return gResourceManager().getTexture("img/Item/Cunsomables/Potion/BoostXP/BoostXP3.png");
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