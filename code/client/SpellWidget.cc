#include "SpellWidget.h"

#include "../common/Singletons.h"

namespace redsquare
{
    SpellWidget::SpellWidget(SpellType type)
    : m_SpellType(type)
    {
        std::string texturePath;

        switch (type)
        {
        case SpellType::FireBall :
            texturePath = "img/SpellIcon/Named/Fireball1.png";
            m_SpellName="Fireball";
            m_Description = "Throw a fireball (click on enemy)";
            m_ManaCost= "5";
            break;
        case SpellType::Lacerate :
            texturePath = "img/SpellIcon/Named/Lacerate1.png";
            m_SpellName="Lacerate";
            m_Description = "Deal damage the more target has of life (click on enemy)";
            m_ManaCost= "5";
            break;
        case SpellType::ArmorUp :
            texturePath = "img/SpellIcon/Named/ArmorUP1.png";
            m_SpellName="ArmorUp";
            m_Description = "Gain a bonus of ArmorPoint (click on yourself)";
            m_ManaCost= "5";
            break;
        case SpellType::Heal :
            texturePath = "img/SpellIcon/Named/Heal1.png";
            m_SpellName="Heal";
            m_Description = "Heal an ally or self (click on yourself or an ally)";
            m_ManaCost= "5";
            break;
        case SpellType::Berserk :
            texturePath = "img/SpellIcon/Named/Berserk1.png";
            m_SpellName="Berserk";
            m_Description = "Gain Attack and Defense points (click on yourself)";
            m_ManaCost= "7";
            break;
        case SpellType::DamageUp :
            texturePath = "img/SpellIcon/Named/DamageUP1.png";
            m_SpellName="Damage up";
            m_Description = "Gain AttackPoint (click on yourself)";
            m_ManaCost= "5";
            break;
        case SpellType::Revenge :
            texturePath = "img/SpellIcon/Named/Revenge1.png";
            m_SpellName="Revenge";
            m_Description = "Deal damage based on missing health (click on enemy)";
            m_ManaCost= "5";
            break;
        case SpellType::Incinerate :
            texturePath = "img/SpellIcon/Named/Incinerate1.png";
            m_SpellName="Incinerate";
            m_Description = "Deal damage the more target has of life (click on enemy)";
            m_ManaCost= "5";
            break;
        case SpellType::Scorch :
            texturePath = "img/SpellIcon/Named/Scorch1.png";
            m_SpellName="Scorch";
            m_Description = "Deal damage (click on enemy)";
            m_ManaCost= "5";
            break;
        case SpellType::Shoot :
            texturePath = "img/SpellIcon/Named/Shoot1.png";
            m_SpellName="Shoot";
            m_Description = "Deal damage from distance (click on enemy)";
            m_ManaCost= "5";
            break;
        case SpellType::Torpedo :
            texturePath = "img/SpellIcon/Named/Torpedo1.png";
            m_SpellName="Torpedo";
            m_Description = "Deal damage based on missing health (click on enemy)";
            m_ManaCost= "5";
            break;
        case SpellType::Massacre :
            texturePath = "img/SpellIcon/Named/Massacre1.png";
            m_SpellName="Massacre";
            m_Description = "Life steal attack (click on enemy)";
            m_ManaCost= "5";
            break;
        case SpellType::DoubleStrike :
            texturePath = "img/SpellIcon/Named/DoubleStrike1.png";
            m_SpellName="DoubleStrike";
            m_Description = "Hit the target twice (click on enemy)";
            m_ManaCost= "5";
            break;
        case SpellType::Protection :
            texturePath = "img/SpellIcon/Named/Protection1.png";
            m_SpellName="Protection";
            m_Description = "Give ally ArmorPoint (click on an ally)";
            m_ManaCost= "5";
            break;
        case SpellType::LightningStrike :
            texturePath = "img/SpellIcon/Named/LightningStrike1.png";
            m_SpellName="LightningStrike";
            m_Description = "Deal damage around you (click on enemy)";
            m_ManaCost= "7";
            break;
        case SpellType::Reaper :
            texturePath = "img/SpellIcon/Named/Reaper1.png";
            m_SpellName="Reaper";
            m_Description = "Deal damage in front of you (click on enemy)";
            m_ManaCost= "7";
            break;
        case SpellType::BasicAttack :
            texturePath = "img/SpellIcon/Named/Basic1.png";
            m_SpellName="BasicAttack";
            m_Description = "Click on enemy";
            m_ManaCost= "5";
            break;
        default:
            texturePath = "img/SpellIcon/Named/Basic1.png";
            m_SpellName="";
            m_Description = "";
            m_ManaCost= "0";
            break;
        }

        gf::Texture &texture = gResourceManager().getTexture(texturePath);
        setDefaultSprite(texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        setDisabledSprite(texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        setSelectedSprite(texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
    }
}