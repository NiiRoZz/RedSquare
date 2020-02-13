#include "GameScene.h"
#include "Scenes.h"
#include "../common/Singletons.h"
#include "Message.h"

#include <iostream>
#include <utility>

#include <gf/VectorOps.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Unused.h>

namespace redsquare
{
    GameScene::GameScene( Scenes &scenes )
    : gf::Scene({1024, 576})
    , m_Scenes(scenes)
    , m_Hud(scenes, *this, scenes.resources.getFont("font/arial.ttf"))
    , m_ThreadCom(m_ComQueue)
    , m_CanPlay( false )
    , m_MovePlayer({0,0},false)
    , m_AttackX(0)
    , m_AttackY(0)
    , m_PassTurn(false)
    , m_TempMoveTarget(false)
    , m_PlayerDead(false)
    , m_NextPosTexture(gResourceManager().getTexture("img/Cursor/case_selected.png"))
    //, m_Name(name)
    , m_Floor(0)
    , m_CurrentSpell(SpellType::BasicAttack)
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
    , m_MoveCursorImage(scenes.resources.getTexture("img/Cursor/moveCursor.png").copyToImage())
    , m_ChestCursorImage(scenes.resources.getTexture("img/Cursor/chestCursor.png").copyToImage())
    {
        setClearColor(gf::Color::Black);

        m_AttackCursorImage.flipHorizontally();
        m_MoveCursorImage.flipHorizontally();
        m_ChestCursorImage.flipHorizontally();

        getWorldView().setCenter({0.0f, 0.0f});
        getWorldView().setSize({300.0f, 300.0f});

        m_World.setPriority(-1);
        addWorldEntity(m_World);

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

    bool GameScene::connect(const char *ip, const char *port, const char *name, const EntitySubType entitySubType)
    {
        if (m_ThreadCom.getSocket().getState() != SocketState::Disconnected)
        {
            assert(false);
            return false;
        }

        m_ThreadCom.getSocket().connectTo(ip, port);

        if (m_ThreadCom.getSocket().getState() != SocketState::Connected)
        {
            gf::Log::debug("Can't connect to game: %s:%s\n",ip,port);
            return false;
        }

        int portChat = atoi(port)+1;
        m_Hud.getChat().connect(ip, std::to_string(portChat).c_str(), name);

        if (m_Hud.getChat().getChatCom().getSocket().getState() != SocketState::Connected)
        {
            gf::Log::debug("Can't connect to chat: %s:%d\n",ip,portChat);
            return false;
        }

        sendInfoConnection(entitySubType, name);

        receiveWorld();

        m_ThreadCom.start();

        return true;
    }

    void GameScene::sendInfoConnection(EntitySubType type, const char *name)
    {
        Packet sendPacket;
        sendPacket.type = PacketType::PlayerInfoConnection;
        sendPacket.playerInfoConnection.entitySubType = type;
        strncpy(sendPacket.playerInfoConnection.name, name, 20);

        m_ThreadCom.sendPacket(sendPacket);
    }

    void GameScene::receiveWorld()
    {
        NewPlayer newPlayerData;

        m_ThreadCom.receivePacket( newPlayerData );

        m_PlayerID = newPlayerData.playerID;

        m_Floor = newPlayerData.floor;

        m_World.generateWorld( std::move( newPlayerData.world ) );
    }

    void GameScene::doHandleActions(gf::Window &window)
    {
        if (m_FullScreenAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard))
        {
            m_Scenes.getWindow().toggleFullscreen();
        }

        if (m_LeftAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory())
        {
            movePlayer(-1, 0);
        }

        if (m_RightAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory())
        {
            movePlayer(1, 0);
        }

        if (m_UpAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory())
        {
            movePlayer(0, -1);
        }

        if (m_DownAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory())
        {
            movePlayer(0, 1);
        }

        if (m_PassTurnAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory())
        {
            passTurn();
        }

        if (m_InventoryAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard))
        {
            m_Hud.showInventory();
        }

        if (m_MapAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard))
        {
            m_Hud.showMap();
        }

        if (m_HelpMenuAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard))
        {
            m_Hud.showHelp();
        }

        if (m_ChatAction.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard))
        {
            m_Hud.showChat();
        }

        if( m_Spell1Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && getMyPlayer() != nullptr && getMyPlayer()->m_Level >= 2)
        {
            changeSpell(1);
        }

        if( m_Spell2Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && getMyPlayer() != nullptr && getMyPlayer()->m_Level >= 2)
        {
            changeSpell(2);
        }

        if( m_Spell3Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && getMyPlayer() != nullptr && getMyPlayer()->m_Level >= 3)
        {
            changeSpell(3);
        }

        if( m_Spell4Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && getMyPlayer() != nullptr && getMyPlayer()->m_Level >= 4)
        {
            changeSpell(4);
        }

        if( m_Spell5Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && getMyPlayer() != nullptr && getMyPlayer()->m_Level >= 5)
        {
            changeSpell(5);
        }

        if( m_Spell6Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && getMyPlayer() != nullptr && getMyPlayer()->m_Level >= 6)
        {
            changeSpell(6);
        }

        if( m_Spell7Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && getMyPlayer() != nullptr && getMyPlayer()->m_Level >= 7)
        {
            changeSpell(7);
        }

        if( m_Spell8Action.isActive() && !m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && getMyPlayer() != nullptr && getMyPlayer()->m_Level >= 8)
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
                if (!m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory())
                {
                    gf::Vector2i pos = m_Scenes.getRenderer().mapPixelToCoords(event.mouseButton.coords, getWorldView()) / World::TileSize;

                    Player* myPlayer = getMyPlayer();
                    if ( myPlayer != nullptr && m_CanPlay )
                    {
                        if ( myPlayer->canAttack(pos, *this) )
                        {
                            attackPos( pos[0], pos[1] );
                        }
                        else if (myPlayer->canMove(pos, m_Players, m_Monsters, m_Props, m_World.m_SquareMap))
                        {
                            movePlayer( pos[0], pos[1], true );
                        }
                        else if ( myPlayer->canOpenTargetInventory(pos, *this) )
                        {
                            ClientEntity *entity = static_cast<ClientEntity*>(getProp(pos));
                            if (entity)
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
                if (!m_Hud.hoveringChat() && !(ImGui::GetIO().WantCaptureKeyboard) && !m_Hud.shownInventory())
                {
                    gf::Vector2i pos = m_Scenes.getRenderer().mapPixelToCoords(event.mouseCursor.coords, getWorldView()) / World::TileSize;

                    Player* myPlayer = getMyPlayer();
                    if ( myPlayer != nullptr && m_CanPlay )
                    {
                        if ( myPlayer->canAttack(pos, *this) )
                        {
                            m_Scenes.getWindow().setMouseCursor(m_AttackCursor);
                            m_TempMove.clear();
                        }
                        else if (myPlayer->canMove(pos, m_Players, m_Monsters, m_Props, m_World.m_SquareMap))
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
        }

        m_Hud.processEvent(event);
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

    void GameScene::doUpdate(gf::Time time)
    {
        ImGui_ImplGF_Update(time);

        //Checking if we have received a message from server
        processPackets();

        m_World.update( time );

        //We update only the player we are controlling
        Player* player = getMyPlayer();

        if ( player != nullptr )
        {
            player->update( time );

            getWorldView().setCenter( { (float)(player->m_Pos[0] * World::TileSize), (float)(player->m_Pos[1] * World::TileSize )} );

            //Do all actions stuff here
            doAction();
        }
    }

    void GameScene::doAction()
    {
        if ( !m_CanPlay )
        {
            return;
        }

        Player* myPlayer = getMyPlayer();

        if ( myPlayer == nullptr )
        {
            return;
        }

        //Player want to move
        if ( m_MovePlayer.first[0] != 0 || m_MovePlayer.first[1] != 0 )
        {
            //don't forget to call m_CanPlay false when send action
            m_CanPlay = false;

            Packet packet;
            packet.type = PacketType::RequestMove;
            packet.requestMove.playerID = m_PlayerID;

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

                    packet.requestMove.dirX = (allPos[1] - myPlayer->m_Pos)[0];
                    packet.requestMove.dirY = (allPos[1] - myPlayer->m_Pos)[1];

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
                packet.requestMove.dirX = m_MovePlayer.first[0];
                packet.requestMove.dirY = m_MovePlayer.first[1];
            }

            m_MovePlayer.first = {0,0};
            m_MovePlayer.second = false;

            m_ThreadCom.sendPacket( packet );
        }
        else if ( m_AttackX != 0 || m_AttackY != 0 )
        {
            //don't forget to call m_CanPlay false when send action
            m_CanPlay = false;

            Packet packet;
            packet.type = PacketType::RequestAttack;
            packet.requestAttack.playerID = m_PlayerID;
            packet.requestAttack.spellType = m_CurrentSpell;
            packet.requestAttack.posX = m_AttackX;
            packet.requestAttack.posY = m_AttackY;

            m_AttackX = 0;
            m_AttackY = 0;
            m_TempMove.clear();

            m_ThreadCom.sendPacket( packet );
        }
        else if ( m_PassTurn )
        {
            //don't forget to call m_CanPlay false when send action
            m_CanPlay = false;

            Packet packet;
            packet.type = PacketType::PassTurn;
            packet.passTurn.playerID = m_PlayerID;

            m_PassTurn = false;
            m_TempMove.clear();

            m_ThreadCom.sendPacket( packet );
        }
    }

    void GameScene::processPackets()
    {
        Packet packet;
        while ( m_ComQueue.poll(packet) )
        {
            switch (packet.type)
            {
                case PacketType::ReceiveMove:
                {
                    switch (packet.receiveMove.typeEntity)
                    {
                        case EntityType::Player:
                        {
                            Player* player = getPlayer( packet.receiveMove.entityID );
                            assert( player != nullptr );

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(player), true);
                            m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(player), true);

                            player->m_Pos[0] = packet.receiveMove.posX;
                            player->m_Pos[1] = packet.receiveMove.posY;

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(player), false);
                            m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(player), false);
                            break;
                        }

                        case EntityType::Monster:
                        {
                            Monster* monster = getMonster( packet.receiveMove.entityID );
                            assert( monster != nullptr );

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(monster), true);
                            m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(monster), true);

                            monster->m_Pos[0] = packet.receiveMove.posX;
                            monster->m_Pos[1] = packet.receiveMove.posY;

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(monster), false);
                            m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(monster), false);
                            break;
                        }
                    }
                    break;
                }

                case PacketType::EntityDisconnected:
                {
                    switch (packet.entityDisconnected.typeEntity)
                    {
                        case EntityType::Player:
                        {
                            Player* player = getPlayer( packet.entityDisconnected.entityID );
                            assert( player != nullptr );

                            removeWorldEntity(player);

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(player), true);
                            m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(player), true);

                            m_Players.erase( packet.entityDisconnected.entityID );
                            break;
                        }

                        case EntityType::Monster:
                        {
                            Monster* monster = getMonster( packet.entityDisconnected.entityID );
                            assert( monster != nullptr );

                            removeWorldEntity(monster);

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(monster), true);
                            m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(monster), true);

                            m_Monsters.erase( packet.entityDisconnected.entityID );
                            break;
                        }

                        case EntityType::Prop:
                        {
                            Prop* prop = getProp( packet.entityDisconnected.entityID );
                            assert( prop != nullptr );

                            removeWorldEntity(prop);

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(prop), true);
                            m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(prop), true);

                            m_Props.erase( packet.entityDisconnected.entityID );
                            break;
                        }

                        case EntityType::ItemHolder:
                        {
                            ItemHolder* itemHolder = getItemHolder( packet.entityDisconnected.entityID );
                            assert( itemHolder != nullptr );

                            removeWorldEntity(itemHolder);

                            m_ItemHolders.erase( packet.entityDisconnected.entityID );
                            break;
                        }
                    }

                    break;
                }

                case PacketType::PlayerTurn:
                {
                    // TODO cd on some spell

                    Player* myPlayer = getMyPlayer();

                    if ( myPlayer == nullptr )
                    {
                        break;
                    }

                    if ( m_TempMoveTarget != gf::Vector2i(0,0) && myPlayer->m_Pos != m_TempMoveTarget && !monsterInRange() && getPlayer(m_TempMoveTarget) == nullptr && getMonster(m_TempMoveTarget) == nullptr )
                    {
                        m_TempMove.clear();

                        std::vector<gf::Vector2i> allPos = m_World.m_SquareMap.computeRoute(myPlayer->m_Pos, m_TempMoveTarget, 0.0);

                        if (!allPos.empty())
                        {
                            m_TempMove.insert(m_TempMove.end(), ++(++allPos.begin()), allPos.end());

                            gf::Vector2i pos = allPos[1];

                            if ( getPlayer(pos) == nullptr && getMonster(pos) == nullptr )
                            {
                                Packet packet;
                                packet.type = PacketType::RequestMove;
                                packet.requestMove.playerID = m_PlayerID;
                                packet.requestMove.dirX = (pos - myPlayer->m_Pos)[0];
                                packet.requestMove.dirY = (pos - myPlayer->m_Pos)[1];

                                m_ThreadCom.sendPacket( packet );

                                /*packet.type = PacketType::PassTurn;
                                packet.passTurn.playerID = m_PlayerID;

                                m_ThreadCom.sendPacket( packet );*/
                                break;
                            }
                        }
                    }

                    m_TempMoveTarget = {0, 0};
                    m_TempMove.clear();

                    m_CanPlay = packet.playerTurn.playerTurn;

                    break;
                }

                case PacketType::SpawnEntity:
                {
                    switch (packet.spawnEntity.typeEntity)
                    {
                        case EntityType::Player:
                        {
                            auto it = m_Players.insert( std::make_pair( packet.spawnEntity.entityID, Player( packet.spawnEntity.entityID, packet.spawnEntity.typeOfEntity, gf::Vector2i(packet.spawnEntity.posX, packet.spawnEntity.posY) ) ) );
                            assert( it.second );

                            addWorldEntity(it.first->second);

                            if (packet.spawnEntity.entityID == m_PlayerID)
                            {
                                MyPlayerReceivedTypeMessage message;
                                message.player = &(it.first->second);

                                gMessageManager().sendMessage(&message);
                            }

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(&(it.first->second)), false);
                            m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(&(it.first->second)), false);
                            break;
                        }

                        case EntityType::Monster:
                        {
                            auto it = m_Monsters.insert( std::make_pair( packet.spawnEntity.entityID, Monster( packet.spawnEntity.entityID, packet.spawnEntity.typeOfEntity, gf::Vector2i(packet.spawnEntity.posX, packet.spawnEntity.posY) ) ) );
                            assert( it.second );

                            addWorldEntity(it.first->second);

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(&(it.first->second)), false);
                            m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(&(it.first->second)), false);
                            break;
                        }

                        case EntityType::Prop:
                        {
                            auto it = m_Props.insert( std::make_pair( packet.spawnEntity.entityID, Prop( packet.spawnEntity.entityID, packet.spawnEntity.typeOfEntity, gf::Vector2i(packet.spawnEntity.posX, packet.spawnEntity.posY) ) ) );
                            assert( it.second );

                            addWorldEntity(it.first->second);

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(&(it.first->second)), false);
                            m_World.setTransparentFromEntity(static_cast<redsquare::Entity*>(&(it.first->second)), false);
                            break;
                        }

                        case EntityType::ItemHolder:
                        {
                            auto it = m_ItemHolders.insert( std::make_pair( packet.spawnEntity.entityID, ItemHolder( packet.spawnEntity.entityID, packet.spawnEntity.holdingItem, gf::Vector2i(packet.spawnEntity.posX, packet.spawnEntity.posY) ) ) );
                            assert( it.second );

                            addWorldEntity(it.first->second);
                            break;
                        }
                    }
                    break;
                }

                case PacketType::EntityCar:
                {
                    if ( packet.entityCar.entityType == EntityType::Player )
                    {
                        Player* player = getPlayer(packet.entityCar.entityID);
                        assert(player != nullptr);

                        player->m_LifePoint = packet.entityCar.m_LifePoint;
                        player->m_MaxLifePoint = packet.entityCar.m_MaxLifePoint;

                        player->m_ManaPoint = packet.entityCar.m_ManaPoint;
                        player->m_MaxManaPoint = packet.entityCar.m_MaxManaPoint;

                        player->m_AttackPoint = packet.entityCar.m_AttackPoint;
                        player->m_DefensePoint = packet.entityCar.m_DefensePoint;

                        player->m_MaxAttackPoint = packet.entityCar.m_MaxAttackPoint;
                        player->m_MaxDefensePoint = packet.entityCar.m_MaxDefensePoint;

                        player->m_Range = packet.entityCar.m_Range;

                        player->m_XP = packet.entityCar.m_XP;
                        player->m_Max_XP = packet.entityCar.m_MaxXP;

                        player->m_Level = packet.entityCar.m_Level;
                    }
                    else if ( packet.entityCar.entityType == EntityType::Monster )
                    {
                        Monster* monster = getMonster(packet.entityCar.entityID);
                        assert(monster != nullptr);

                        monster->m_LifePoint = packet.entityCar.m_LifePoint;
                        monster->m_MaxLifePoint = packet.entityCar.m_MaxLifePoint;

                        monster->m_AttackPoint = packet.entityCar.m_AttackPoint;
                        monster->m_DefensePoint = packet.entityCar.m_DefensePoint;

                        monster->m_MaxAttackPoint = packet.entityCar.m_MaxAttackPoint;
                        monster->m_MaxDefensePoint = packet.entityCar.m_MaxDefensePoint;

                        monster->m_Range = packet.entityCar.m_Range;

                        monster->m_Level = packet.entityCar.m_Level;
                    }

                    break;
                }

                case PacketType::PlayerDead:
                {
                    m_PlayerDead = true;

                    MyPlayerDeadMessage message;

                    gMessageManager().sendMessage(&message);
                    break;
                }

                case PacketType::NewMap:
                {
                    m_TempMove.clear();
                    m_TempMoveTarget = {0, 0};
                    receiveWorld();
                    m_Scenes.replaceScene(m_Scenes.game, m_Scenes.glitchEffect, gf::seconds(1.0f));
                    m_ThreadCom.start();
                    break;
                }

                case PacketType::UpdateSpells:
                {
                    Player *myPlayer = getMyPlayer();

                    if ( myPlayer == nullptr )
                    {
                        break;
                    }

                    std::copy(packet.updateSpells.spells, packet.updateSpells.spells + MAX_SPELL_PER_PLAYER, myPlayer->m_SpellTab.begin() );

                    SpellUpdateMessage message;
                    std::copy(packet.updateSpells.spells, packet.updateSpells.spells + MAX_SPELL_PER_PLAYER, message.spells.begin());

                    gMessageManager().sendMessage(&message);
                    break;
                }

                case PacketType::UpdateItem:
                {
                    ClientEntity *entity = nullptr;

                    switch (packet.updateItem.entityType)
                    {
                        case EntityType::Player:
                        {
                            entity = getPlayer(packet.updateItem.entityID);
                            break;
                        }

                        case EntityType::Monster:
                        {
                            entity = getMonster(packet.updateItem.entityID);
                            break;
                        }

                        case EntityType::Prop:
                        {
                            entity = getProp(packet.updateItem.entityID);
                            break;
                        }
                    }
                    assert(entity != nullptr);

                    if (packet.updateItem.removeItem)
                    {
                        entity->getInventory().removeItem(packet.updateItem.slotType, packet.updateItem.pos);
                    }
                    else
                    {
                        ClientItem item( packet.updateItem.typeItem, packet.updateItem.slotMask, packet.updateItem.baseFloorItem );
                        entity->getInventory().addItem(packet.updateItem.slotType, std::move(item), packet.updateItem.pos);
                    }

                    break;
                }

                case PacketType::MoveItem:
                {
                    ClientEntity *oldEntity = nullptr;
                    ClientEntity *newEntity = nullptr;

                    switch (packet.moveItem.oldEntityType)
                    {
                        case EntityType::Player:
                        {
                            oldEntity = getPlayer(packet.moveItem.oldEntityID);
                            break;
                        }

                        case EntityType::Monster:
                        {
                            oldEntity = getMonster(packet.moveItem.oldEntityID);
                            break;
                        }

                        case EntityType::Prop:
                        {
                            oldEntity = getProp(packet.moveItem.oldEntityID);
                            break;
                        }
                    }
                    assert(oldEntity != nullptr);

                    switch (packet.moveItem.newEntityType)
                    {
                        case EntityType::Player:
                        {
                            newEntity = getPlayer(packet.moveItem.newEntityID);
                            break;
                        }

                        case EntityType::Monster:
                        {
                            newEntity = getMonster(packet.moveItem.newEntityID);
                            break;
                        }

                        case EntityType::Prop:
                        {
                            newEntity = getProp(packet.moveItem.newEntityID);
                            break;
                        }
                    }
                    assert(newEntity != nullptr);

                    if (oldEntity == newEntity)
                    {
                        assert(newEntity->getInventory().moveItem(packet.moveItem));
                    }

                    break;
                }
            }
        }
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

    void GameScene::attackPos( int posX, int posY )
    {
        if ( !m_CanPlay )
        {
            return;
        }

        m_AttackX = posX;
        m_AttackY = posY;
    }

    void GameScene::passTurn()
    {
        if ( !m_CanPlay )
        {
            return;
        }

        m_PassTurn = true;
    }

    Player* GameScene::getMyPlayer()
    {
        auto player = m_Players.find( m_PlayerID );

        if ( player != m_Players.end() )
        {
            return &player->second;
        }

        return nullptr;
    }

    Player* GameScene::getPlayer( gf::Id playerID )
    {
        auto player = m_Players.find( playerID );

        if ( player != m_Players.end() )
        {
            return &player->second;
        }

        return nullptr;
    }

    Player* GameScene::getPlayer( gf::Vector2i pos )
    {
        auto it = m_Players.begin();

        // Iterate over the map using Iterator till end.
        while ( it != m_Players.end() )
        {
            if ( it->second.isInsideMe(pos) )
            {
                return &it->second;
            }

            ++it;
        }

        return nullptr;
    }

    Monster* GameScene::getMonster( gf::Id monsterID )
    {
        auto monster = m_Monsters.find( monsterID );

        if ( monster != m_Monsters.end() )
        {
            return &monster->second;
        }

        return nullptr;
    }

    Monster* GameScene::getMonster( gf::Vector2i pos )
    {
        auto it = m_Monsters.begin();

        // Iterate over the map using Iterator till end.
        while ( it != m_Monsters.end() )
        {
            if ( it->second.isInsideMe(pos) )
            {
                return &it->second;
            }

            ++it;
        }

        return nullptr;
    }

    Prop* GameScene::getProp( gf::Id propID )
    {
        auto prop = m_Props.find( propID );

        if ( prop != m_Props.end() )
        {
            return &prop->second;
        }

        return nullptr;
    }

    Prop* GameScene::getProp( gf::Vector2i pos )
    {
        auto it = m_Props.begin();

        // Iterate over the map using Iterator till end.
        while ( it != m_Props.end() )
        {
            if ( it->second.isInsideMe(pos) )
            {
                return &it->second;
            }

            ++it;
        }

        return nullptr;
    }

    ItemHolder* GameScene::getItemHolder( gf::Id itemHolderID )
    {
        auto itemHolder = m_ItemHolders.find( itemHolderID );

        if ( itemHolder != m_ItemHolders.end() )
        {
            return &itemHolder->second;
        }

        return nullptr;
    }

    bool GameScene::monsterInRange()
    {
        Player* myPlayer = getMyPlayer();

        if ( myPlayer == nullptr )
        {
            return false;
        }

        gf::Distance2<int> distFn = gf::manhattanDistance<int, 2>;

        auto it = m_Monsters.begin();

        while ( it != m_Monsters.end() )
        {
            if ( distFn(myPlayer->m_Pos, it->second.m_Pos) <= myPlayer->m_Range )
            {
                return true;
            }

            ++it;
        }

        return false;
    }
    
    void GameScene::changeSpell(int spell){

        auto currentPlayer = getMyPlayer();
        if(currentPlayer != nullptr){
            m_CurrentSpell = currentPlayer->m_SpellTab[spell-1];
        }else{
            std::cout << "ERROR" <<std::endl;
        }
        std::cout << "SPELL CHANGED FOR SPELL N°" << spell << std::endl;

        return;
    }

    void GameScene::sendPacket(Packet &packet)
    {
        m_ThreadCom.sendPacket(packet);
    }
}
