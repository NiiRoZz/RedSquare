#ifndef REDSQUARE_CLIENT_SCENES_H
#define REDSQUARE_CLIENT_SCENES_H

#include <gf/SceneManager.h>
#include <gf/ResourceManager.h>
#include <gf/MessageManager.h>
#include <gf/SegueEffects.h>

#include "ClientNetwork.h"

#include "MainMenuScene.h"
#include "ConnectionScene.h"
#include "LobbyScene.h"
#include "RoomScene.h"
#include "GameScene.h"

namespace redsquare
{
  struct Scenes : public gf::SceneManager
  {
    Scenes(ClientNetwork& network);
    Scenes(const Scenes&) = delete;
    Scenes(Scenes&&) = delete;
    ~Scenes();

    Scenes& operator=(const Scenes&) = delete;
    Scenes& operator=(Scenes&&) = delete;

    gf::ResourceManager resources;
    gf::MessageManager messages;

    MainMenuScene mainMenu;
    ConnectionScene connection;
    LobbyScene lobby;
    RoomScene room;
    GameScene game;

    gf::GlitchSegueEffect glitchEffect;
    gf::PixelateSegueEffect pixelateEffect;

    gf::Id myPlayerId;
  };

}

#endif // REDSQUARE_CLIENT_SCENES_H
