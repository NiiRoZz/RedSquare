#include <cstdlib>
#include <cstdio>
#include <vector>
#include <deque>
#include <gf/Geometry.h>
#include <gf/Grid.h>
#include <gf/Math.h>
#include <gf/Random.h>
#include <chrono>

#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>
#include <iostream>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>
#include <gf/VertexArray.h>
#include <gf/Cursor.h>
#include <gf/Font.h>
#include <gf/TileLayer.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "World.h"
#include "Game.h"
#include "Chat.h"
#include "Inventory.h"
#include "MainMenu.h"
#include "Hud.h"
#include "../common/Singletons.h"
#include "../common/Packet.h"
#include "config.h"

#define NBTYPEPLAYER 4

using namespace redsquare;

int main( int argc, char **argv )
{
    if ( argc != 5 || atoi(argv[4]) > NBTYPEPLAYER || atoi(argv[4]) < 1)
    {
        std::cerr << "Usage : ./RedSquare ip port playerName typePlayer" << std::endl;
        std::cerr << "typePlayer: " << std::endl << "1 for Magus" << std::endl << "2 for Warrior" << std::endl << "3 for Rogue" << std::endl << "4 for Ranger" << std::endl;
        return 1;
    }

    static constexpr gf::Vector2f ViewSize(300.0f, 300.0f); // dummy values
    static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f); // dummy values

    // initialization
    gf::Vector2i screenSize = { 1024, 576 };
    gf::Window window("RedSquare", screenSize);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    gf::RenderWindow renderer(window);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // config
    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr;
    ImGui_ImplGF_Init(window, renderer);

    gf::SingletonStorage<gf::MessageManager> storageForMessageManager(redsquare::gMessageManager);
    gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(redsquare::gResourceManager);

    // setup resource directories
    redsquare::gResourceManager().addSearchDir(REDSQUARE_DATA_DIR);

    // initialization redsquare
    gf::Font &fontChat(gResourceManager().getFont("font/arial.ttf"));

    bool inventoryVisible = false;
    bool MainMenuVisible = false;
    
    // views
    gf::ViewContainer views;
    gf::ExtendView mainView( ViewCenter, ViewSize );
    views.addView( mainView );
    gf::ScreenView hudView;
    views.addView( hudView );
    views.setInitialScreenSize( screenSize );

    // actions
    gf::ActionContainer actions;
    gf::Action closeWindowAction("Close window");
    closeWindowAction.addCloseControl();
    closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
    actions.addAction(closeWindowAction);

    gf::Action fullscreenAction("Fullscreen");
    fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
    actions.addAction(fullscreenAction);

    gf::Action MainMenuAction("AfficheMainMenu");
    MainMenuAction.addKeycodeKeyControl(gf::Keycode::N);
    actions.addAction(MainMenuAction);

    gf::Action HelpMenuAction("AfficherAide");
    HelpMenuAction.addKeycodeKeyControl(gf::Keycode::H);
    actions.addAction(HelpMenuAction);

    gf::Action inventoryAction("Inventory");
    inventoryAction.addKeycodeKeyControl(gf::Keycode::I);
    actions.addAction(inventoryAction);

    gf::Action mapAction("Map");
    mapAction.addKeycodeKeyControl(gf::Keycode::M);
    actions.addAction(mapAction);

    gf::Action chatAction("Chat");
    chatAction.addKeycodeKeyControl(gf::Keycode::C);
    actions.addAction(chatAction);

    gf::Action leftAction("Left");
    leftAction.addScancodeKeyControl(gf::Scancode::A);
    leftAction.addScancodeKeyControl(gf::Scancode::Left);
    //leftAction.setContinuous();
    actions.addAction(leftAction);

    gf::Action rightAction("Right");
    rightAction.addScancodeKeyControl(gf::Scancode::D);
    rightAction.addScancodeKeyControl(gf::Scancode::Right);
    //rightAction.setContinuous();
    actions.addAction(rightAction);

    gf::Action upAction("Up");
    upAction.addScancodeKeyControl(gf::Scancode::W);
    upAction.addScancodeKeyControl(gf::Scancode::Up);
    //upAction.setContinuous();
    actions.addAction(upAction);

    gf::Action downAction("Down");
    downAction.addScancodeKeyControl(gf::Scancode::S);
    downAction.addScancodeKeyControl(gf::Scancode::Down);
    //downAction.setContinuous();
    actions.addAction(downAction);

    gf::Action passTurn("PassTurn");
    passTurn.addKeycodeKeyControl(gf::Keycode::Space);
    actions.addAction(passTurn);

    gf::Action clicAction("Clic");
    clicAction.addMouseButtonControl(gf::MouseButton::Left);

    gf::Action changeSpell1("Spell1");
    changeSpell1.addScancodeKeyControl(gf::Scancode::Num1);
    changeSpell1.addScancodeKeyControl(gf::Scancode::Numpad1);
    actions.addAction(changeSpell1);

    gf::Action changeSpell2("Spell2");
    changeSpell2.addScancodeKeyControl(gf::Scancode::Num2);
    changeSpell2.addScancodeKeyControl(gf::Scancode::Numpad2);
    actions.addAction(changeSpell2);

    gf::Action changeSpell3("Spell3");
    changeSpell3.addScancodeKeyControl(gf::Scancode::Num3);
    changeSpell3.addScancodeKeyControl(gf::Scancode::Numpad3);
    actions.addAction(changeSpell3);

    gf::Action changeSpell4("Spell4");
    changeSpell4.addScancodeKeyControl(gf::Scancode::Num4);
    changeSpell4.addScancodeKeyControl(gf::Scancode::Numpad4);
    actions.addAction(changeSpell4);

    gf::Action changeSpell5("Spell5");
    changeSpell5.addScancodeKeyControl(gf::Scancode::Num5);
    changeSpell5.addScancodeKeyControl(gf::Scancode::Numpad5);
    actions.addAction(changeSpell5);

    gf::Action changeSpell6("Spell6");
    changeSpell6.addScancodeKeyControl(gf::Scancode::Num6);
    changeSpell6.addScancodeKeyControl(gf::Scancode::Numpad6);
    actions.addAction(changeSpell6);

    gf::Action changeSpell7("Spell7");
    changeSpell7.addScancodeKeyControl(gf::Scancode::Num7);
    changeSpell7.addScancodeKeyControl(gf::Scancode::Numpad7);
    actions.addAction(changeSpell7);

    gf::Action changeSpell8("Spell8");
    changeSpell8.addScancodeKeyControl(gf::Scancode::Num8);
    changeSpell8.addScancodeKeyControl(gf::Scancode::Numpad8);
    actions.addAction(changeSpell8);

    // entities
    gf::EntityContainer mainEntities;
    // add entities to mainEntities
    Game game( argv[1], argv[2], mainView, argv[3] );
    int Iport_Chat=atoi(argv[2])+1;
    char  Cport_Chat[10] ;
    sprintf(Cport_Chat,"%d",Iport_Chat);
    std::cout << "hud : "<< Cport_Chat<<std::endl;
    Hud hud(game, fontChat,Cport_Chat,argv[1],mainView,argv[3]);

    //Send info about us, before get world
    game.sendInfoConnection(static_cast<EntitySubType>(atoi(argv[4])), argv[3]);

    //Client pause here until receive world
    game.receiveWorld();

    game.startThreadCom();
    mainEntities.addEntity( game );

    gf::EntityContainer hudEntities;
    // add entities to hudEntities
    hudEntities.addEntity(hud);

    gf::Cursor defaultCursor(gf::Cursor::Type::Arrow);

    gf::Image attackImage(std::move(gResourceManager().getTexture("img/Cursor/attackCursor.png").copyToImage()));
    attackImage.flipHorizontally();
    gf::Cursor attackCursor(attackImage, { 8u, 8u });

    gf::Image moveImage(std::move(gResourceManager().getTexture("img/Cursor/moveCursor.png").copyToImage()));
    moveImage.flipHorizontally();
    gf::Cursor moveCursor(moveImage, { 8u, 8u });

    gf::Image openChestImage(std::move(gResourceManager().getTexture("img/Cursor/chestCursor.png").copyToImage()));
    openChestImage.flipHorizontally();
    gf::Cursor openChestCursor(openChestImage, { 8u, 8u });

    window.setMouseCursor(defaultCursor);

    // game loop
    renderer.clear(gf::Color::Black);
    gf::Clock clock;
    std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
    while (window.isOpen())
    {
        if (game.m_PlayerDead)
        {
            gf::Event event;

            while (window.pollEvent(event))
            {
                actions.processEvent(event);
                views.processEvent(event);
                hud.processEvent(event);
            }

            game.m_TempMove.clear();
            window.setMouseCursor(defaultCursor);
        }
        else
        {
            // 1. input
            gf::Event event;

            while (window.pollEvent(event))
            {
                actions.processEvent(event);
                views.processEvent(event);
                hud.processEvent(event);
                ImGui_ImplGF_ProcessEvent(event);

                switch (event.type)
                {
                    case gf::EventType::MouseButtonPressed:
                    {
                        if (!hud.hoveringChat() && !(io.WantCaptureKeyboard) && !inventoryVisible)
                        {
                            end = std::chrono::system_clock::now();
                            int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
                            if( elapsed_seconds >= 500 )
                            {
                                start = std::chrono::system_clock::now();

                                gf::Vector2i pos = renderer.mapPixelToCoords(event.mouseButton.coords,mainView) / World::TileSize;

                                Player* myPlayer = game.getMyPlayer();
                                if ( myPlayer != nullptr && game.m_CanPlay )
                                {
                                    if ( myPlayer->canAttack(pos, game) )
                                    {
                                        game.attackPos( pos[0], pos[1] );
                                    }
                                    else if (myPlayer->canMove(pos, game.m_Players, game.m_Monsters, game.m_Props, game.m_World.m_SquareMap))
                                    {
                                        game.movePlayer( pos[0], pos[1], true );
                                    }
                                    else if ( myPlayer->canOpenTargetInventory(pos, game) )
                                    {
                                        ClientEntity *entity = static_cast<ClientEntity*>(game.getProp(pos));
                                        if (entity)
                                        {
                                            hud.getInventoryUI().setVinicityObject(entity);
                                            InventoryShowUpdateMessage message;
                                            gMessageManager().sendMessage(&message);

                                            inventoryVisible = !inventoryVisible;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }

                    case gf::EventType::MouseMoved:
                    {
                        if (!hud.hoveringChat() && !(io.WantCaptureKeyboard) && !inventoryVisible)
                        {
                            gf::Vector2i pos = renderer.mapPixelToCoords(event.mouseCursor.coords,mainView) / World::TileSize;

                            Player* myPlayer = game.getMyPlayer();
                            if ( myPlayer != nullptr && game.m_CanPlay )
                            {
                                if ( myPlayer->canAttack(pos, game) )
                                {
                                    window.setMouseCursor(attackCursor);
                                    game.m_TempMove.clear();
                                }
                                else if (myPlayer->canMove(pos, game.m_Players, game.m_Monsters, game.m_Props, game.m_World.m_SquareMap))
                                {
                                    window.setMouseCursor(moveCursor);

                                    game.m_TempMove.clear();

                                    std::vector<gf::Vector2i> allPos = game.m_World.m_SquareMap.computeRoute(myPlayer->m_Pos, pos, 0.0);

                                    if (!allPos.empty())
                                    {
                                        game.m_TempMove.insert(game.m_TempMove.end(), ++allPos.begin(), allPos.end());
                                    }
                                }
                                else if ( myPlayer->canOpenTargetInventory(pos, game) )
                                {
                                    window.setMouseCursor(openChestCursor);
                                    game.m_TempMove.clear();
                                }
                                else
                                {
                                    window.setMouseCursor(defaultCursor);
                                    game.m_TempMove.clear();
                                }
                            }
                            else
                            {
                                game.m_TempMove.clear();
                                window.setMouseCursor(defaultCursor);
                            }
                        }
                        else
                        {
                            game.m_TempMove.clear();
                            window.setMouseCursor(defaultCursor);
                        }

                        break;
                    }
                }
            }

            if (fullscreenAction.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard))
            {
                window.toggleFullscreen();
            }
            if (rightAction.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && !inventoryVisible)
            {
                game.movePlayer( 1, 0 );
            }
            else if (leftAction.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && !inventoryVisible)
            {
                game.movePlayer( -1, 0 );
            }
            else if (upAction.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && !inventoryVisible)
            {
                game.movePlayer( 0, -1 );
            }
            else if (downAction.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && !inventoryVisible)
            {
                game.movePlayer( 0, 1 );
            }

            if (passTurn.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && !inventoryVisible)
            {
                game.passTurn();
            }
            if (inventoryAction.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard))
            {
                InventoryShowUpdateMessage message;
                gMessageManager().sendMessage(&message);

                inventoryVisible = !inventoryVisible;
            }
            if (mapAction.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard))
            {
                hud.showMap();
            }
            if (chatAction.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard))
            {
                hud.hideChat();
            }
            if (MainMenuAction.isActive())
            {
                hud.m_MainMenu.m_ShowMainMenu = !hud.m_MainMenu.m_ShowMainMenu;
            }
            if (HelpMenuAction.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard))
            {
                hud.showHelp();
            }
            if( changeSpell1.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && game.getMyPlayer() != nullptr && game.getMyPlayer()->m_Level >= 2)
            {
                game.changeSpell(1);
            }
            if( changeSpell2.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && game.getMyPlayer() != nullptr && game.getMyPlayer()->m_Level >= 2)
            {
                game.changeSpell(2);
            }
            if( changeSpell3.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && game.getMyPlayer() != nullptr && game.getMyPlayer()->m_Level >= 3 )
            {
                game.changeSpell(3);
            }
            if( changeSpell4.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && game.getMyPlayer() != nullptr && game.getMyPlayer()->m_Level >= 4 )
            {
                game.changeSpell(4);
            }
            if( changeSpell5.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && game.getMyPlayer() != nullptr && game.getMyPlayer()->m_Level >= 5 )
            {
                game.changeSpell(5);
            }
            if( changeSpell6.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && game.getMyPlayer() != nullptr && game.getMyPlayer()->m_Level >= 6 )
            {
                game.changeSpell(6);
            }
            if( changeSpell7.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && game.getMyPlayer() != nullptr && game.getMyPlayer()->m_Level >= 7 )
            {
                game.changeSpell(7);
            }
            if( changeSpell8.isActive() && !hud.hoveringChat() && !(io.WantCaptureKeyboard) && game.getMyPlayer() != nullptr && game.getMyPlayer()->m_Level >= 8 )
            {
                game.changeSpell(8);
            }
        }

        if (closeWindowAction.isActive())
        {
            window.close();
        }

        // 2. update
        gf::Time time = clock.restart();
        mainEntities.update(time);
        hudEntities.update(time);
        ImGui_ImplGF_Update(time);

        // 3. draw
        renderer.clear();

        renderer.setView(mainView);
        mainEntities.render(renderer);

        renderer.setView(hudView);
        hudEntities.render(renderer);

        renderer.display();

        actions.reset();
	}

	return 0;
}
