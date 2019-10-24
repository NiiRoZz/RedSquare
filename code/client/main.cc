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

#include "World.h"
#include "Game.h"
#include "../common/Packet.h"
#include <gf/TileLayer.h>



using namespace redsquare;

int main( int argc, char **argv )
{
    if ( argc != 3 )
    {
        std::cerr << "Usage : ./RedSquare ip port" << std::endl;
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
    
    gf::Action clicAction("Clic");
    clicAction.addMouseButtonControl(gf::MouseButton::Left);

    // entities
    gf::EntityContainer mainEntities;
    // add entities to mainEntities
    Game game( argv[1], argv[2], mainView );

    //Client pause here until receive world
    game.receiveWorld();

    game.startThreadCom();
    mainEntities.addEntity( game );


    gf::EntityContainer hudEntities;
    // add entities to hudEntities


    gf::Cursor defaultCursor;
    gf::Image attackImage;
    gf::Cursor attackCursor;
    gf::Image moveImage;
    gf::Cursor moveCursor;

    if (attackImage.loadFromFile("data/redsquare/img/attackCursor.png"))
    {
        attackCursor.loadFromImage(attackImage, { 8u, 8u });
    }

    if (moveImage.loadFromFile("data/redsquare/img/moveCursor.png"))
    {
        moveCursor.loadFromImage(moveImage, { 8u, 8u });
    }

    defaultCursor.loadFromSystem( gf::Cursor::Type::Arrow );

    window.setMouseCursor(defaultCursor);

    // game loop
    renderer.clear(gf::Color::Black);
    gf::Clock clock;
    std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
    while (window.isOpen())
    {
        // 1. input
        gf::Event event;
        
        while (window.pollEvent(event))
        {
            actions.processEvent(event);
            views.processEvent(event);

            switch (event.type)
            {
                case gf::EventType::MouseButtonPressed:
                {
                    end = std::chrono::system_clock::now();
                    int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
                    if( elapsed_seconds >= 500 )
                    {
                        start = std::chrono::system_clock::now();

                        gf::Vector2i pos = renderer.mapPixelToCoords(event.mouseButton.coords,mainView) / World::TileSize;

                        Player* myPlayer = game.getPlayer(game.m_PlayerID);
                        if ( myPlayer != nullptr && game.m_CanPlay )
                        {
                            if ( myPlayer->canAttack(pos, game.m_Players, game.m_Monsters) )
                            {
                                game.attackPos( pos[0], pos[1] );
                            }
                            else if (myPlayer->canMove(pos, game.m_Players, game.m_Monsters, game.m_World.m_SquareMap))
                            {
                                game.movePlayer(pos[0],pos[1]);
                            }
                        }
                    }
                    break;
                }

                case gf::EventType::MouseMoved:
                {
                    gf::Vector2i pos = renderer.mapPixelToCoords(event.mouseCursor.coords,mainView) / World::TileSize;

                    Player* myPlayer = game.getPlayer(game.m_PlayerID);
                    if ( myPlayer != nullptr && game.m_CanPlay )
                    {
                        if ( myPlayer->canAttack(pos, game.m_Players, game.m_Monsters) )
                        {
                            window.setMouseCursor(attackCursor);
                        }
                        else if (myPlayer->canMove(pos, game.m_Players, game.m_Monsters, game.m_World.m_SquareMap))
                        {
                            window.setMouseCursor(moveCursor);
                        }
                        else
                        {
                            window.setMouseCursor(defaultCursor);
                        }
                    }
                    else
                    {
                        window.setMouseCursor(defaultCursor);
                    }
                    break;
                }
            }
        }

        if (closeWindowAction.isActive())
        {
            window.close();
        }

        if (fullscreenAction.isActive())
        {
            window.toggleFullscreen();
        }

        if (rightAction.isActive())
        {
            game.movePlayer( 1,0);
        } 
        else if (leftAction.isActive())
        {
            game.movePlayer( -1,0 );
        } 
        else if (upAction.isActive())
        {
            game.movePlayer( 0, -1 );
        } 
        else if (downAction.isActive())
        {
            game.movePlayer( 0,1);
        }
        
        // 2. update
        gf::Time time = clock.restart();
        mainEntities.update(time);
        hudEntities.update(time);

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
