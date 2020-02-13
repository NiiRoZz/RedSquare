#include "Player.h"
#include "World.h"
#include "../common/Singletons.h"

#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Color.h>
#include <gf/Text.h>

#include "GameScene.h"

namespace redsquare
{
    Player::Player( gf::Id entityID, EntitySubType entitySubType, gf::Vector2i pos )
    : ClientEntity(entityID, EntityType::Player, entitySubType)
    , m_Font(gResourceManager().getFont("font/arial.ttf"))
    {
        m_Pos = pos;
        m_Max_XP = 0;
    }

    void Player::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        static constexpr gf::Vector2f BarSize = { 20.0f, 3.0f } ;
        static constexpr gf::Vector2f BarSize2 = { 20.0f, 1.5f } ;
        static constexpr gf::Vector2f BarOffset = { 2.0f, 6.0f };
        static constexpr gf::Vector2f BarOffset2 = { 2.0f, 6.0f };
        static constexpr gf::Vector2f BarOffsetMana1 = { 2.0f, 3.0f };
        static constexpr gf::Vector2f BarOffsetMana2 = { 2.0f, 3.0f };
        static constexpr gf::Vector2f BarOffsetXp = { 4.0f, 4.0f };

        gf::Sprite sprite;

        sprite.setPosition( m_Pos * World::TileSize );
        sprite.setScale( 1 );
        sprite.setTexture( *m_EntityTexture );
        target.draw(sprite, states);
        
        gf::Color4f color(255,0,0,174);
        gf::RectangleShape bar;

        bar.setSize({BarSize.width * 1 , BarSize.height});
        bar.setColor(color);
        bar.setOutlineColor(gf::Color::darker(color));
        bar.setPosition(m_Pos * World::TileSize-BarOffset);
        bar.setAnchor(gf::Anchor::TopLeft);
        target.draw(bar, states);   

        gf::RectangleShape bar2;
        color = gf::Color::Green;
        
        bar2.setSize({BarSize.width * (static_cast<float>(m_LifePoint)/static_cast<float>(m_MaxLifePoint)), BarSize.height});
        bar2.setColor(color);
        bar2.setPosition(m_Pos * World::TileSize-BarOffset2);
        bar2.setAnchor(gf::Anchor::TopLeft);
        target.draw(bar2, states); 

        gf::RectangleShape barMana2;
        color = gf::Color::Cyan;
     
        bar2.setSize({BarSize2.width * 1 , BarSize2.height});
        bar2.setColor(color);
        bar2.setPosition(m_Pos * World::TileSize-BarOffsetMana2);
        bar2.setAnchor(gf::Anchor::TopLeft);
        target.draw(bar2, states); 

        gf::RectangleShape barMana1;
        color = gf::Color::Blue;
        
        bar2.setSize({BarSize2.width * (static_cast<float>(m_ManaPoint)/static_cast<float>(m_MaxManaPoint)) , BarSize2.height});
        bar2.setColor(color);
        bar2.setPosition(m_Pos * World::TileSize-BarOffsetMana1);
        bar2.setAnchor(gf::Anchor::TopLeft);
        target.draw(bar2, states);


        gf::Text m_lvl;
        m_lvl.setCharacterSize(8);
        m_lvl.setColor(gf::Color::Black);
        m_lvl.setFont(m_Font);
        m_lvl.setString("" + std::to_string(m_Level));
        m_lvl.setPosition(m_Pos * World::TileSize-BarOffsetXp);
        m_lvl.setAnchor(gf::Anchor::CenterRight);
        target.draw(m_lvl, states);
    }

    bool Player::canAttack(gf::Vector2i targetPos, GameScene &game)
    {
        gf::Distance2<int> distFn = gf::manhattanDistance<int, 2>;

        float distance = distFn(m_Pos, targetPos);

        if ( distance > m_Range )
        {
            return false;
        }

        switch (game.m_CurrentSpell)
        {
            case SpellType::Heal :
                if ( isInsideMe(targetPos) ){
                    return true;
                }
                if( game.getEntities().getPlayer(targetPos) != nullptr){
                    return true;
                }
                break;
            case SpellType::Protection :
                if ( isInsideMe(targetPos) ){
                    return true;
                }
                if( game.getEntities().getPlayer(targetPos) != nullptr){
                    return true;
                }
                break;
            case SpellType::RangeUp :
                if ( isInsideMe(targetPos) ){
                    return true;
                }
                if( game.getEntities().getPlayer(targetPos) != nullptr){
                    return true;
                }
                break;
            case SpellType::DamageUp :
                if ( isInsideMe(targetPos) ){
                    return true;
                }
                if( game.getEntities().getPlayer(targetPos) != nullptr){
                    return true;
                }
                break;
            case SpellType::ArmorUp :
                if ( isInsideMe(targetPos) ){
                    return true;
                }
                if( game.getEntities().getPlayer(targetPos) != nullptr){
                    return true;
                }
                break;
            case SpellType::LightningStrike :
                if ( isInsideMe(targetPos) ){
                    return true;
                }
                break;
            case SpellType::Berserk :
                if ( isInsideMe(targetPos) ){
                    return true;
                }else{
                    return false;
                }
                break;
            default:
                break;
        }

        if ( isInsideMe(targetPos) )
        {
            return false;
        }

        if (game.getEntities().getMonster(targetPos) != nullptr)
        {
            return true;
        }

        if (game.getEntities().getProp(targetPos, false) != nullptr)
        {
            return true;
        }

        return false;
    }

    bool Player::canMove(gf::Vector2i targetPos, gf::SquareMap &map)
    {
        if ( m_Pos == targetPos || targetPos[0] < 0 || targetPos[1] < 0 )
        {
            return false;
        }

        return map.isWalkable( targetPos );
    }

    bool Player::canOpenTargetInventory(gf::Vector2i targetPos, GameScene &game)
    {
        gf::Distance2<int> distFn = gf::manhattanDistance<int, 2>;

        float distance = distFn(m_Pos, targetPos);

        return (distance <= m_Range && !(isInsideMe(targetPos)) && game.getEntities().getProp(targetPos, true) != nullptr);
    }
}
