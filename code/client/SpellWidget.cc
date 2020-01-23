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
            m_Description = "Throw a fireball";
            break;
        case SpellType::Devastate :
            texturePath = "img/SpellIcon/Named/Devastate1.png";
            m_Description = "Deal damage the more target has of life";
            break;
        case SpellType::Lacerate :
            texturePath = "img/SpellIcon/Named/Lacerate1.png";
            m_Description = "Deal damage the more target has of life";
            break;
        case SpellType::ArmorUp :
            texturePath = "img/SpellIcon/Named/ArmorUP1.png";
            m_Description = "Gain a bonus of ArmorPoint";
            break;
        case SpellType::Heal :
            texturePath = "img/SpellIcon/Named/Heal1.png";
            m_Description = "Heal an ally or self";
            break;
        case SpellType::Berserk :
            texturePath = "img/SpellIcon/Named/Berserk1.png";
            m_Description = "Gain Attack and Defense points";
            break;
        case SpellType::DamageUp :
            texturePath = "img/SpellIcon/Named/DamageUP1.png";
            m_Description = "Gain AttackPoint";
            break;
        case SpellType::Revenge :
            texturePath = "img/SpellIcon/Named/Revenge1.png";
            m_Description = "Deal damage based on missing health";
            break;
        case SpellType::Incinerate :
            texturePath = "img/SpellIcon/Named/Incinerate1.png";
            m_Description = "Deal damage the more target has of life";
            break;
        case SpellType::Scorch :
            texturePath = "img/SpellIcon/Named/Scorch1.png";
            m_Description = "Deal damage";
            break;
        case SpellType::Shoot :
            texturePath = "img/SpellIcon/Named/Shoot1.png";
            m_Description = "Deal damage from distance";
            break;
        case SpellType::Torpedo :
            texturePath = "img/SpellIcon/Named/Torpedo1.png";
            m_Description = "Deal damage based on missing health";
            break;
        case SpellType::Massacre :
            texturePath = "img/SpellIcon/Named/Massacre1.png";
            m_Description = "Life steal attack";
            break;
        case SpellType::DoubleStrike :
            texturePath = "img/SpellIcon/Named/DoubleStrike1.png";
            m_Description = "Hit the target twice";
            break;
        case SpellType::Protection :
            texturePath = "img/SpellIcon/Named/Protection1.png";
            m_Description = "Give ally ArmorPoint";
            break;
        case SpellType::LightningStrike :
            texturePath = "img/SpellIcon/Named/LightningStrike1.png";
            m_Description = "Deal damage around you";
            break;
        case SpellType::Reaper :
            texturePath = "img/SpellIcon/Named/Reaper1.png";
            m_Description = "Deal damage in front of you";
            break;
        case SpellType::BasicAttack :
            texturePath = "img/SpellIcon/Named/Basic1.png";
            m_Description = "Basic Attack";
            break;
        default:
            texturePath = "img/SpellIcon/Named/Basic1.png";
            m_Description = "";
            break;
        }

        gf::Texture &texture = gResourceManager().getTexture(texturePath);
        setDefaultSprite(texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        setDisabledSprite(texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
        setSelectedSprite(texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
    }
}