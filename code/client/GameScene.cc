#include "GameScene.h"
#include "Scenes.h"
#include "../common/RedsquareProtocol.h"
#include "../common/Singletons.h"

#include <iostream>
#include <utility>

#include <gf/VectorOps.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Unused.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

namespace redsquare
{
    GameScene::GameScene(Scenes& scenes, ClientNetwork& network)
    : gf::Scene(InitialSize)
    , m_CurrentSpell(SpellType::BasicAttack)
    , m_Scenes(scenes)
    , m_Network(network)
    , m_Hud(scenes, *this, scenes.resources.getFont("font/arial.ttf"), network)
    , m_CanPlay(false)
    , m_PlayerDead(false)
    , m_Floor(0u)
    , m_MovePlayer({0,0}, false)
    , m_NextPosTexture(scenes.resources.getTexture("img/Cursor/case_selected.png"))
    , m_TempMoveTarget({0,0})
    , m_AttackPos({0,0})
    , m_FullScreenAction("FullScreen")
    , m_LeftAction("Left")
    , m_RightAction("Right")
    , m_UpAction("Up")
    , m_DownAction("Down")
    , m_PassTurnAction("Pass Turn")
    , m_InventoryAction("Inventory")
    , m_MapAction("Map")
    , m_HelpMenuAction("HelpMenu")
    , m_ChatAction("Chat")
    , m_EscapeAction("Escape")
    , m_Spell1Action("Spell")
    , m_Spell2Action("Spell")
    , m_Spell3Action("Spell")
    , m_Spell4Action("Spell")
    , m_Spell5Action("Spell")
    , m_Spell6Action("Spell")
    , m_Spell7Action("Spell")
    , m_Spell8Action("Spell")
    , m_DefaultCursor(gf::Cursor::Type::Arrow)
    , m_AttackCursorImage(scenes.resources.getTexture("img/Cursor/attackCursor.png").copyToImage())
    , m_AttackCursor(m_AttackCursorImage, {8u, 8u})
    , m_MoveCursorImage(scenes.resources.getTexture("img/Cursor/moveCursor.png").copyToImage())
    , m_MoveCursor(m_MoveCursorImage, {8u, 8u})
    , m_ChestCursorImage(scenes.resources.getTexture("img/Cursor/chestCursor.png").copyToImage())
    , m_ChestCursor(m_ChestCursorImage, {8u, 8u})
    {
        setClearColor(gf::Color::Black);

        getWorldView().setCenter({0.0f, 0.0f});
        getWorldView().setSize({300.0f, 300.0f});

        m_World.setPriority(-1);
        addWorldEntity(m_World);
        addWorldEntity(m_Entities);

        addHudEntity(m_Hud);

        m_FullScreenAction.addKeycodeKeyControl(gf::Keycode::F);
        addAction(m_FullScreenAction);

        m_LeftAction.addScancodeKeyControl(gf::Scancode::A);
        m_LeftAction.addKeycodeKeyControl(gf::Keycode::Left);
        addAction(m_LeftAction);

        m_RightAction.addScancodeKeyControl(gf::Scancode::D);
        m_RightAction.addKeycodeKeyControl(gf::Keycode::Right);
        addAction(m_RightAction);

        m_UpAction.addScancodeKeyControl(gf::Scancode::W);
        m_UpAction.addKeycodeKeyControl(gf::Keycode::Up);
        addAction(m_UpAction);

        m_DownAction.addScancodeKeyControl(gf::Scancode::S);
        m_DownAction.addKeycodeKeyControl(gf::Keycode::Down);
        addAction(m_DownAction);

        m_PassTurnAction.addKeycodeKeyControl(gf::Keycode::Space);
        addAction(m_PassTurnAction);

        m_InventoryAction.addKeycodeKeyControl(gf::Keycode::I);
        addAction(m_InventoryAction);

        m_MapAction.addKeycodeKeyControl(gf::Keycode::M);
        addAction(m_MapAction);

        m_HelpMenuAction.addKeycodeKeyControl(gf::Keycode::H);
        addAction(m_HelpMenuAction);

        m_ChatAction.addKeycodeKeyControl(gf::Keycode::C);
        addAction(m_ChatAction);

        m_EscapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
        addAction(m_EscapeAction);

        m_Spell1Action.addKeycodeKeyControl(gf::Keycode::Num1);
        m_Spell1Action.addKeycodeKeyControl(gf::Keycode::Numpad1);
        addAction(m_Spell1Action);

        m_Spell2Action.addKeycodeKeyControl(gf::Keycode::Num2);
        m_Spell2Action.addKeycodeKeyControl(gf::Keycode::Numpad2);
        addAction(m_Spell2Action);

        m_Spell3Action.addKeycodeKeyControl(gf::Keycode::Num3);
        m_Spell3Action.addKeycodeKeyControl(gf::Keycode::Numpad3);
        addAction(m_Spell3Action);

        m_Spell4Action.addKeycodeKeyControl(gf::Keycode::Num4);
        m_Spell4Action.addKeycodeKeyControl(gf::Keycode::Numpad4);
        addAction(m_Spell4Action);

        m_Spell5Action.addKeycodeKeyControl(gf::Keycode::Num5);
        m_Spell5Action.addKeycodeKeyControl(gf::Keycode::Numpad5);
        addAction(m_Spell5Action);

        m_Spell6Action.addKeycodeKeyControl(gf::Keycode::Num6);
        m_Spell6Action.addKeycodeKeyControl(gf::Keycode::Numpad6);
        addAction(m_Spell6Action);

        m_Spell7Action.addKeycodeKeyControl(gf::Keycode::Num7);
        m_Spell7Action.addKeycodeKeyControl(gf::Keycode::Numpad7);
        addAction(m_Spell7Action);

        m_Spell8Action.addKeycodeKeyControl(gf::Keycode::Num8);
        m_Spell8Action.addKeycodeKeyControl(gf::Keycode::Numpad8);
        addAction(m_Spell8Action);
    }

    Entities& GameScene::getEntities()
    {
        return m_Entities;
    }

    uint GameScene::getCurrentFloor() const
    {
        return m_Floor;
    }

    bool GameScene::canPlay() const
    {
        return m_CanPlay;
    }

    bool GameScene::playerDead() const
    {
        return m_PlayerDead;
    }

    void GameScene::doHandleActions(gf::Window &window)
    {
        if (m_FullScreenAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard))
        {
            m_Scenes.getWindow().toggleFullscreen();
        }

        if (m_LeftAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory() && !m_Hud.escapeOpen())
        {
            movePlayer(-1, 0);
        }

        if (m_RightAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory() && !m_Hud.escapeOpen())
        {
            movePlayer(1, 0);
        }

        if (m_UpAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory() && !m_Hud.escapeOpen())
        {
            movePlayer(0, -1);
        }

        if (m_DownAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory() && !m_Hud.escapeOpen())
        {
            movePlayer(0, 1);
        }

        if (m_PassTurnAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory() && !m_Hud.escapeOpen())
        {
            if (m_CanPlay)
            {
                m_PassTurn = true;
            }
        }

        if (m_InventoryAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.escapeOpen())
        {
            m_Hud.showInventory();
        }

        if (m_MapAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.escapeOpen())
        {
            m_Hud.showMap();
        }

        if (m_HelpMenuAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.escapeOpen())
        {
            m_Hud.showHelp();
        }

        if (m_ChatAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard))
        {
            m_Hud.showChat();
        }

        if (m_EscapeAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard))
        {
            m_Hud.showEscape();
        }

        if( m_Spell1Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.escapeOpen() && m_Entities.getPlayer(m_Scenes.myPlayerId) != nullptr && m_Entities.getPlayer(m_Scenes.myPlayerId)->m_Level >= 2)
        {
            changeSpell(1);
        }

        if( m_Spell2Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.escapeOpen() && m_Entities.getPlayer(m_Scenes.myPlayerId) != nullptr && m_Entities.getPlayer(m_Scenes.myPlayerId)->m_Level >= 2)
        {
            changeSpell(2);
        }

        if( m_Spell3Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.escapeOpen() && m_Entities.getPlayer(m_Scenes.myPlayerId) != nullptr && m_Entities.getPlayer(m_Scenes.myPlayerId)->m_Level >= 3)
        {
            changeSpell(3);
        }

        if( m_Spell4Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.escapeOpen() && m_Entities.getPlayer(m_Scenes.myPlayerId) != nullptr && m_Entities.getPlayer(m_Scenes.myPlayerId)->m_Level >= 4)
        {
            changeSpell(4);
        }

        if( m_Spell5Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.escapeOpen() && m_Entities.getPlayer(m_Scenes.myPlayerId) != nullptr && m_Entities.getPlayer(m_Scenes.myPlayerId)->m_Level >= 5)
        {
            changeSpell(5);
        }

        if( m_Spell6Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.escapeOpen() && m_Entities.getPlayer(m_Scenes.myPlayerId) != nullptr && m_Entities.getPlayer(m_Scenes.myPlayerId)->m_Level >= 6)
        {
            changeSpell(6);
        }

        if( m_Spell7Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.escapeOpen() && m_Entities.getPlayer(m_Scenes.myPlayerId) != nullptr && m_Entities.getPlayer(m_Scenes.myPlayerId)->m_Level >= 7)
        {
            changeSpell(7);
        }

        if( m_Spell8Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.escapeOpen() && m_Entities.getPlayer(m_Scenes.myPlayerId) != nullptr && m_Entities.getPlayer(m_Scenes.myPlayerId)->m_Level >= 8)
        {
            changeSpell(8);
        }
    }

    bool GameScene::doEarlyProcessEvent(gf::Event &event)
    {
        return ImGui_ImplGF_ProcessEvent(event);
    }

    void GameScene::doProcessEvent(gf::Event& event)
    {
        switch (event.type)
        {
            case gf::EventType::MouseButtonReleased:
            {
                if (!m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory() && !m_Hud.escapeOpen())
                {
                    gf::Vector2i pos = m_Scenes.getRenderer().mapPixelToCoords(event.mouseButton.coords, getWorldView()) / World::TileSize;

                    Player* myPlayer = m_Entities.getPlayer(m_Scenes.myPlayerId);
                    if ( myPlayer != nullptr && m_CanPlay )
                    {
                        if ( myPlayer->canAttack(pos, *this) )
                        {
                            attackPos( pos );
                        }
                        else if (myPlayer->canMove(pos, m_World.m_SquareMap))
                        {
                            movePlayer( pos[0], pos[1], true );
                        }
                        else if ( myPlayer->canOpenTargetInventory(pos, *this) )
                        {
                            ClientEntity *entity = static_cast<ClientEntity*>(m_Entities.getProp(pos));
                            if (entity != nullptr)
                            {
                                m_Hud.getInventoryUI().setVinicityObject(entity);
                                
                                m_Hud.showInventory();
                            }
                        }
                    }
                }
                break;
            }

            case gf::EventType::MouseMoved:
            {
                if (!m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory() && !m_Hud.escapeOpen())
                {
                    gf::Vector2i pos = m_Scenes.getRenderer().mapPixelToCoords(event.mouseCursor.coords, getWorldView()) / World::TileSize;

                    Player* myPlayer = m_Entities.getPlayer(m_Scenes.myPlayerId);
                    if ( myPlayer != nullptr && m_CanPlay )
                    {
                        if ( myPlayer->canAttack(pos, *this) )
                        {
                            m_Scenes.getWindow().setMouseCursor(m_AttackCursor);
                            m_TempMove.clear();
                        }
                        else if (myPlayer->canMove(pos, m_World.m_SquareMap))
                        {
                            m_Scenes.getWindow().setMouseCursor(m_MoveCursor);

                            m_TempMove.clear();

                            std::vector<gf::Vector2i> allPos = m_World.m_SquareMap.computeRoute(myPlayer->m_Pos, pos, 0.0);

                            if (!allPos.empty())
                            {
                                m_TempMove.insert(m_TempMove.end(), ++allPos.begin(), allPos.end());
                            }
                        }
                        else if ( myPlayer->canOpenTargetInventory(pos, *this) )
                        {
                            m_Scenes.getWindow().setMouseCursor(m_ChestCursor);
                            m_TempMove.clear();
                        }
                        else
                        {
                            m_Scenes.getWindow().setMouseCursor(m_DefaultCursor);
                            m_TempMove.clear();
                        }
                    }
                    else
                    {
                        m_TempMove.clear();
                        m_Scenes.getWindow().setMouseCursor(m_DefaultCursor);
                    }
                }
                else
                {
                    m_TempMove.clear();
                    m_Scenes.getWindow().setMouseCursor(m_DefaultCursor);
                }

                break;
            }

            default:
            {
                break;
            }
        }

        m_Hud.processEvent(event);
    }

    void GameScene::doUpdate(gf::Time time)
    {
        gf::unused(time);
        if (!isActive())
        {
            return;
        }

        ProtocolBytes bytes;
        while (m_Network.queue.poll(bytes))
        {
            switch (bytes.getType())
            {
                //Receive the map
                case RedsquareServerInitMap::type:
                {
                    gf::Log::debug("(GAME) receive RedsquareServerInitMap\n");
                    auto data = bytes.as<RedsquareServerInitMap>();
                    m_Floor = data.floor;
                    m_World.generateWorld( std::move( data.world ) );
                    break;
                }

                //Spawn multiple entities
                case RedsquareServerInitEntities::type:
                {
                    gf::Log::debug("(GAME) receive RedsquareServerInitEntities\n");
                    auto data = bytes.as<RedsquareServerInitEntities>();

                    m_TempMove.clear();
                    m_TempMoveTarget = {0, 0};

                    for(auto &entity: data.allEntities)
                    {
                        if (entity.id != gf::InvalidId)
                        {
                            createEntity(entity);

                            if (entity.entityType == EntityType::Player && entity.id == m_Scenes.myPlayerId)
                            {
                                Player *myPlayer = m_Entities.getPlayer(m_Scenes.myPlayerId);
                                assert(myPlayer != nullptr);

                                MyPlayerReceivedTypeMessage message;
                                message.player = myPlayer;

                                gMessageManager().sendMessage(&message);
                                    
                                SpellUpdateMessage message2;
                                message2.spells = entity.characteristics.m_Spells;
                                gMessageManager().sendMessage(&message2);
                            }
                        }
                    }
                    break;
                }

                //Spawn one entity
                case RedsquareServerInitEntity::type:
                {
                    gf::Log::debug("(GAME) receive RedsquareServerInitEntity\n");
                    auto data = bytes.as<RedsquareServerInitEntity>();

                    createEntity(data.entity);
                    break;
                }

                case RedsquareServerPlayerTurn::type:
                {
                    gf::Log::debug("(GAME) receive RedsquareServerPlayerTurn\n");

                    Player* myPlayer = m_Entities.getPlayer(m_Scenes.myPlayerId);

                    if ( myPlayer == nullptr )
                    {
                        m_CanPlay = true;
                        break;
                    }

                    if ( m_TempMoveTarget != gf::Vector2i(0,0) && myPlayer->m_Pos != m_TempMoveTarget && !m_Entities.monsterInRange(myPlayer->m_Pos, myPlayer->m_Range) )
                    {
                        m_TempMove.clear();

                        std::vector<gf::Vector2i> allPos = m_World.m_SquareMap.computeRoute(myPlayer->m_Pos, m_TempMoveTarget, 0.0);

                        if (!allPos.empty())
                        {
                            m_TempMove.insert(m_TempMove.end(), ++(++allPos.begin()), allPos.end());

                            gf::Vector2i pos = allPos[1];

                            if ( m_Entities.getPlayer(pos) == nullptr && m_Entities.getMonster(pos) == nullptr )
                            {
                                RedsquareClientMove movePacket;
                                movePacket.dir = pos - myPlayer->m_Pos;
                                m_Network.send(movePacket);
                                break;
                            }
                        }
                    }

                    m_TempMoveTarget = {0, 0};
                    m_TempMove.clear();

                    m_CanPlay = true;
                    break;
                }

                //Receive a move from server
                case RedsquareServerMove::type:
                {
                    gf::Log::debug("(GAME) receive RedsquareServerMove\n");
                    auto data = bytes.as<RedsquareServerMove>();

                    if (data.id != gf::InvalidId)
                    {
                        switch (data.entityType)
                        {
                            case EntityType::Player:
                            {
                                Player* player = m_Entities.getPlayer( data.id );
                                assert( player != nullptr );

                                m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(player), true);
                                m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(player), true);

                                player->m_Pos = data.pos;

                                m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(player), false);
                                m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(player), false);
                                break;
                            }

                            case EntityType::Monster:
                            {
                                Monster* monster = m_Entities.getMonster( data.id );
                                assert( monster != nullptr );

                                m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(monster), true);
                                m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(monster), true);

                                monster->m_Pos = data.pos;

                                m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(monster), false);
                                m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(monster), false);
                                break;
                            }

                            default:
                            {
                                break;
                            }
                        }
                    }
                    break;
                }

                case RedsquareServerUpdateCharacteristic::type:
                {
                    gf::Log::debug("(GAME) receive RedsquareServerUpdateCharacteristic\n");
                    auto data = bytes.as<RedsquareServerUpdateCharacteristic>();

                    if (data.id != gf::InvalidId)
                    {
                        switch (data.entityType)
                        {
                            case EntityType::Player:
                            {
                                Player* player = m_Entities.getPlayer( data.id );
                                assert( player != nullptr );

                                player->loadCharacteristic(data.characteristics);

                                if (data.id == m_Scenes.myPlayerId)
                                {
                                    SpellUpdateMessage message;
                                    message.spells = data.characteristics.m_Spells;
                                    gMessageManager().sendMessage(&message);
                                }
                                break;
                            }

                            case EntityType::Monster:
                            {
                                Monster* monster = m_Entities.getMonster( data.id );
                                assert( monster != nullptr );

                                monster->loadCharacteristic(data.characteristics);
                                break;
                            }

                            default:
                            {
                                break;
                            }
                        }
                    }
                    break;
                }

                case RedsquareServerDeleteEntity::type:
                {
                    gf::Log::debug("(GAME) receive RedsquareServerDeleteEntity\n");
                    auto data = bytes.as<RedsquareServerDeleteEntity>();

                    if (data.id != gf::InvalidId)
                    {
                        switch (data.entityType)
                        {
                            case EntityType::Player:
                            {
                                Player* player = m_Entities.getPlayer( data.id );
                                assert( player != nullptr );

                                m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(player), true);
                                m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(player), true);

                                m_Entities.removePlayer( data.id );
                                break;
                            }

                            case EntityType::Monster:
                            {
                                Monster* monster = m_Entities.getMonster( data.id );
                                assert( monster != nullptr );

                                m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(monster), true);
                                m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(monster), true);

                                m_Entities.removeMonster( data.id );
                                break;
                            }

                            case EntityType::Prop:
                            {
                                Prop* prop = m_Entities.getProp( data.id );
                                assert( prop != nullptr );

                                m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(prop), true);
                                m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(prop), true);

                                m_Entities.removeProp( data.id );
                                break;
                            }

                            case EntityType::ItemHolder:
                            {
                                ItemHolder* itemHolder = m_Entities.getItemHolder( data.id );
                                assert( itemHolder != nullptr );

                                m_Entities.removeItemHolder( data.id );
                                break;
                            }
                        }
                    }
                    break;
                }

                case RedsquareServerUpdateItem::type:
                {
                    gf::Log::debug("(GAME) receive RedsquareServerUpdateItem\n");
                    auto data = bytes.as<RedsquareServerUpdateItem>();

                    if (data.id != gf::InvalidId)
                    {
                        ClientEntity *entity = nullptr;

                        switch (data.entityType)
                        {
                            case EntityType::Player:
                            {
                                entity = m_Entities.getPlayer(data.id);
                                break;
                            }

                            case EntityType::Monster:
                            {
                                entity = m_Entities.getMonster(data.id);
                                break;
                            }

                            case EntityType::Prop:
                            {
                                entity = m_Entities.getProp(data.id);
                                break;
                            }
                        }
                        assert(entity != nullptr);

                        if (data.removeItem)
                        {
                            entity->getInventory().removeItem(data.slotType, data.pos);
                        }
                        else
                        {
                            ClientItem item( data.typeItem, data.slotMask, data.baseFloorItem );
                            entity->getInventory().addItem(data.slotType, std::move(item), data.pos);
                        }
                    }
                    break;
                }

                default:
                {
                    break;
                }
            }
        }

        m_World.update( time );

        //We update only the player we are controlling
        Player* player = m_Entities.getPlayer(m_Scenes.myPlayerId);

        if ( player != nullptr )
        {
            getWorldView().setCenter( { (float)(player->m_Pos[0] * World::TileSize), (float)(player->m_Pos[1] * World::TileSize )} );

            //Do all actions stuff here
            doAction();
        }
    }

    void GameScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states)
    {
        ImGui::NewFrame();

        //World entities
        renderWorldEntities(target, states);
        //Show only when the player is not dead
        if (!m_PlayerDead)
        {
            auto it1 = m_TempMove.begin();

            // Iterate over the map using Iterator till end.
            while (it1 != m_TempMove.end())
            {
                //create sprite here
                gf::Sprite sprite;

                sprite.setPosition( (*it1) * World::TileSize );
                sprite.setScale( 1 );
                sprite.setTexture( m_NextPosTexture );
                target.draw( sprite );

                ++it1;
            }
        }

        //Hud entities
        renderHudEntities(target, states);

        ImGui::Render();
        ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
    }

    void GameScene::movePlayer( int dirX, int dirY, bool mouseClic )
    {
        if ( !m_CanPlay )
        {
            return;
        }

        m_MovePlayer.first = {dirX, dirY};
        m_MovePlayer.second = mouseClic;
    }

    void GameScene::attackPos( gf::Vector2i pos )
    {
        if ( !m_CanPlay )
        {
            return;
        }

        m_AttackPos = pos;
    }

    void GameScene::createEntity(EntityData &entity)
    {
        switch (entity.entityType)
        {
            case EntityType::Player:
            {
                gf::Log::debug("GameScene::createEntity EntityType::Player 1\n");
                m_Entities.addNewPlayer(entity.id, std::move(Player(entity.id, entity.entitySubType, entity.pos)));
                gf::Log::debug("GameScene::createEntity EntityType::Player 2\n");

                Player *player = m_Entities.getPlayer(entity.id);
                assert(player != nullptr);
                player->loadCharacteristic(entity.characteristics);

                m_World.setWalkableFromEntity(player, false);
                gf::Log::debug("GameScene::createEntity EntityType::Player 3\n");
                m_World.setTransparentFromEntity(player, false);
                gf::Log::debug("GameScene::createEntity EntityType::Player 4\n");
                break;
            }

            case EntityType::Monster:
            {
                m_Entities.addNewMonster(entity.id, std::move(Monster(entity.id, entity.entitySubType, entity.pos)));

                Monster *monster = m_Entities.getMonster(entity.id);
                assert(monster != nullptr);
                monster->loadCharacteristic(entity.characteristics);

                m_World.setWalkableFromEntity(monster, false);
                m_World.setTransparentFromEntity(monster, false);
                break;
            }

            case EntityType::Prop:
            {
                auto prop = m_Entities.addNewProp(entity.id, std::move(Prop(entity.id, entity.entitySubType, entity.pos)));

                m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(&prop), false);
                m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(&prop), false);
                break;
            }

            case EntityType::ItemHolder:
            {
                m_Entities.addNewItemHolder(entity.id, std::move(ItemHolder(entity.id, entity.holdingItem, entity.pos)));
                break;
            }
        }
    }

    void GameScene::doAction()
    {
        if ( !m_CanPlay )
        {
            return;
        }

        Player* myPlayer = m_Entities.getPlayer(m_Scenes.myPlayerId);

        if ( myPlayer == nullptr )
        {
            return;
        }

        //Player want to move
        if ( m_MovePlayer.first[0] != 0 || m_MovePlayer.first[1] != 0 )
        {
            //don't forget to call m_CanPlay false when send action
            m_CanPlay = false;

            RedsquareClientMove movePacket;

            //If it's move with key
            m_TempMove.clear();

            //used mouse clic
            if ( m_MovePlayer.second )
            {
                m_TempMove.clear();

                std::vector<gf::Vector2i> allPos = m_World.m_SquareMap.computeRoute(myPlayer->m_Pos, m_MovePlayer.first, 0.0);

                if (!allPos.empty())
                {
                    m_TempMove.insert(m_TempMove.end(), ++(++allPos.begin()), allPos.end());

                    movePacket.dir = allPos[1] - myPlayer->m_Pos;

                    if (allPos.size() > 2)
                    {
                        m_TempMoveTarget = m_MovePlayer.first;
                    }
                    else
                    {
                        m_TempMoveTarget = {0, 0};
                    }
                }
            }
            else
            {
                movePacket.dir = m_MovePlayer.first;
            }

            m_MovePlayer.first = {0,0};
            m_MovePlayer.second = false;

            m_Network.send(movePacket);
        }
        else if ( m_AttackPos[0] != 0 || m_AttackPos[1] != 0 )
        {
            //don't forget to call m_CanPlay false when send action
            m_CanPlay = false;

            RedsquareClientAttack attackPacket;
            attackPacket.pos = m_AttackPos;
            attackPacket.spellType = m_CurrentSpell;

            m_AttackPos = {0,0};
            m_TempMove.clear();

            m_Network.send(attackPacket);
        }
        else if ( m_PassTurn )
        {
            //don't forget to call m_CanPlay false when send action
            m_CanPlay = false;

            RedsquareClientPassTurn passTurnPacket;

            m_PassTurn = false;
            m_TempMove.clear();

            m_Network.send(passTurnPacket);
        }
    }

    void GameScene::changeSpell(int spell)
    {
        auto currentPlayer = m_Entities.getPlayer(m_Scenes.myPlayerId);
        if(currentPlayer != nullptr)
        {
            m_CurrentSpell = currentPlayer->m_SpellTab[spell-1];
        }
    }
}
