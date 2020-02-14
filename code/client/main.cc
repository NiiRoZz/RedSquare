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

#include "Scenes.h"
#include "../common/Singletons.h"
#include "ClientNetwork.h"
#include "config.h"


using namespace redsquare;

int main( int argc, char **argv )
{
    ClientNetwork network;

    gf::SingletonStorage<gf::MessageManager> storageForMessageManager(redsquare::gMessageManager);
    gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(redsquare::gResourceManager);

    redsquare::gResourceManager().addSearchDir(REDSQUARE_DATA_DIR);

    Scenes scenes(network);

    scenes.pushScene(scenes.mainMenu);
    scenes.run();

    return 0;
}
