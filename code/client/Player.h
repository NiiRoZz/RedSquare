#ifndef REDSQUARE_CLIENT_PLAYER_H
#define REDSQUARE_CLIENT_PLAYER_H

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace redsquare
{
    class Player: public gf::Entity
    {
    public:
        /*POS*/
        gf::Vector2i m_Pos;
        /*POS*/
        /* Characteristic */
        int m_LifePoint;
        int m_ManaPoint;

        int m_MaxLifePoint;
        int m_MaxManaPoint;


        int m_AttackPoint;
        int m_DefensePoint;
        int m_MovePoint;
        int m_Range;

        int m_XP;
        int m_Max_XP;
        /* Characteristic*/
        
        Player();
        Player( gf::Vector2i pos, uint8_t type );

        void loadTexture(uint8_t type = 0);

        virtual void update(gf::Time time) override;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        bool canAttack(gf::Vector2i m_TargetPos);
        void attack();

    private:
        gf::Texture testTexture;

    };
}

#endif
