#ifndef REDSQUARE_CLIENT_GAME_H
#define REDSQUARE_CLIENT_GAME_H

#include <vector>
#include <map>

#include <gf/Entity.h>
#include <gf/Time.h>
#include <gf/Id.h>
#include <gf/Views.h>
#include <gf/Map.h>
#include <gf/Scene.h>
#include <gf/Cursor.h>

#include "ClientNetwork.h"
#include "Entities.h"
#include "World.h"
#include "Hud.h"

namespace redsquare
{
    struct Scenes;
    struct EntityData;

    class GameScene: public gf::Scene
    {
    public:
        GameScene(Scenes& scenes, ClientNetwork& network);

        Entities& getEntities();

        uint getCurrentFloor() const;

        bool canPlay() const;

        bool playerDead() const;

        void changeSpell(int spell);

        void initialize();

    public:
        World m_World;

        SpellType m_CurrentSpell;

    private:
        void doHandleActions(gf::Window& window) override;
        bool doEarlyProcessEvent(gf::Event &event) override;
        void doProcessEvent(gf::Event& event) override;
        void doUpdate(gf::Time time) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;

        void movePlayer( int dirX, int dirY, bool mouseClic = false );
        void attackPos( gf::Vector2i pos );

        void createEntity(EntityData &entity);

        void doAction();

    private:
        Scenes& m_Scenes;
        ClientNetwork& m_Network;

        Entities m_Entities;

        Hud m_Hud;

        //Value if it's his turn and he can play
        bool m_CanPlay;

        //Value to say if the current player is dead
        bool m_PlayerDead;

        //Get the current floor
        uint m_Floor;

        //Move of player
        std::pair<gf::Vector2i, bool> m_MovePlayer;
        gf::Texture &m_NextPosTexture;
        std::vector<gf::Vector2i> m_TempMove;
        gf::Vector2i m_TempMoveTarget;

        //Attack of player
        gf::Vector2i m_AttackPos;

        //Pass turn
        bool m_PassTurn;

        //Actions
        gf::Action m_FullScreenAction;
        gf::Action m_LeftAction;
        gf::Action m_RightAction;
        gf::Action m_UpAction;
        gf::Action m_DownAction;
        gf::Action m_PassTurnAction;
        gf::Action m_InventoryAction;
        gf::Action m_MapAction;
        gf::Action m_HelpMenuAction;
        gf::Action m_ChatAction;
        gf::Action m_EscapeAction;
        gf::Action m_Spell1Action;
        gf::Action m_Spell2Action;
        gf::Action m_Spell3Action;
        gf::Action m_Spell4Action;
        gf::Action m_Spell5Action;
        gf::Action m_Spell6Action;
        gf::Action m_Spell7Action;
        gf::Action m_Spell8Action;

        //Cursors
        gf::Cursor m_DefaultCursor;
        gf::Image m_AttackCursorImage;
        gf::Cursor m_AttackCursor;
        gf::Image m_MoveCursorImage;
        gf::Cursor m_MoveCursor;
        gf::Image m_ChestCursorImage;
        gf::Cursor m_ChestCursor;

        /*

        GameScene( Scenes &scenes );

        void passTurn();

        void processPackets();

        void doHandleActions(gf::Window& window) override;
        bool doEarlyProcessEvent(gf::Event &event) override;
        void doProcessEvent(gf::Event& event) override;
        void doUpdate(gf::Time time) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;

        void sendPacket(Packet &packet);
        

    private:
        void receiveWorld();

    private:
        Scenes& m_Scenes;

        std::string m_Name;

        gf::Action m_FullScreenAction;
        gf::Action m_LeftAction;
        gf::Action m_RightAction;
        gf::Action m_UpAction;
        gf::Action m_DownAction;
        gf::Action m_PassTurnAction;
        gf::Action m_InventoryAction;
        gf::Action m_MapAction;
        gf::Action m_HelpMenuAction;
        gf::Action m_ChatAction;
        gf::Action m_EscapeAction;
        gf::Action m_Spell1Action;
        gf::Action m_Spell2Action;
        gf::Action m_Spell3Action;
        gf::Action m_Spell4Action;
        gf::Action m_Spell5Action;
        gf::Action m_Spell6Action;
        gf::Action m_Spell7Action;
        gf::Action m_Spell8Action;
        */
    };
}


#endif
