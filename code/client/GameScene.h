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

#include "Monster.h"
#include "Player.h"
#include "Prop.h"
#include "ItemHolder.h"
#include "ThreadCom.h"
#include "../common/Packet.h"
#include "World.h"
#include "Hud.h"

namespace redsquare
{
    struct Scenes;

    class GameScene: public gf::Scene
    {
    public:
        //ID of current player
        gf::Id m_PlayerID;

        //All players
        std::map<gf::Id, Player> m_Players;
        //All monsters
        std::map<gf::Id, Monster> m_Monsters;
        //All props
        std::map<gf::Id, Prop> m_Props;
        //All itemHolders
        std::map<gf::Id, ItemHolder> m_ItemHolders;

        //Value if it's his turn and he can play
        bool m_CanPlay;

        //Value to say we can play the game
        bool m_PlayerDead;

        World m_World;

        uint m_Floor;

        std::vector<gf::Vector2i> m_TempMove;

        SpellType m_CurrentSpell;

        GameScene( Scenes &scenes );

        Player* getMyPlayer();
        Player* getPlayer( gf::Id playerID );
        Player* getPlayer( gf::Vector2i pos );

        Monster* getMonster( gf::Id monsterID );
        Monster* getMonster( gf::Vector2i pos );

        Prop* getProp( gf::Id propID );
        Prop* getProp( gf::Vector2i pos );

        ItemHolder* getItemHolder( gf::Id itemHolderID );

        bool monsterInRange();

        bool connect(const char *ip, const char *port, const char *name, const EntitySubType entitySubType);

        void movePlayer( int dirX, int dirY, bool mouseClic = false );

        void attackPos( int posX, int posY );

        void passTurn();

        void processPackets();

        void doHandleActions(gf::Window& window) override;
        bool doEarlyProcessEvent(gf::Event &event) override;
        void doProcessEvent(gf::Event& event) override;
        void doUpdate(gf::Time time) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;

        void changeSpell(int spell);

        void sendPacket(Packet &packet);

    private:
        void sendInfoConnection(EntitySubType type, const char *name);
        void receiveWorld();

    private:
        Scenes& m_Scenes;

        Hud m_Hud;

        const char *m_Name;
        //Thread for communication
        ThreadCom<Packet> m_ThreadCom;
        //Queue for message
        gf::Queue<Packet> m_ComQueue;

        gf::Texture &m_NextPosTexture;

        //Move of player
        std::pair<gf::Vector2i, bool> m_MovePlayer;
        gf::Vector2i m_TempMoveTarget;

        //Attack of player
        int m_AttackX;
        int m_AttackY;
        std::vector<SpellType> m_Spell;

        //Pass turn
        bool m_PassTurn;

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
        gf::Action m_Spell1Action;
        gf::Action m_Spell2Action;
        gf::Action m_Spell3Action;
        gf::Action m_Spell4Action;
        gf::Action m_Spell5Action;
        gf::Action m_Spell6Action;
        gf::Action m_Spell7Action;
        gf::Action m_Spell8Action;

        gf::Cursor m_DefaultCursor;
        gf::Cursor m_AttackCursor;
        gf::Cursor m_MoveCursor;
        gf::Cursor m_ChestCursor;

        void doAction();
    };
}


#endif
