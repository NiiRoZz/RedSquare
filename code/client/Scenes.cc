#include "Scenes.h"

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "config.h"

namespace redsquare {

    Scenes::Scenes(ClientNetwork& network)
    : gf::SceneManager("Redsquare", InitialSize)
    , resources({ REDSQUARE_DATA_DIR })
    , mainMenu(*this)
    , connection(*this, network)
    , lobby(*this, network)
    , room(*this, network)
    , game(*this, network)
    , myPlayerId(gf::InvalidId)
    {
        getWindow().setVerticalSyncEnabled(true);
        getWindow().setFramerateLimit(60u);
        
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        // config
        // io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = nullptr;

        // load font(s)
        io.Fonts->AddFontFromFileTTF(resources.getAbsolutePath("font/arial.ttf").string().c_str(), 20);

        ImGui_ImplGF_Init(getWindow(), getRenderer());
    }

    Scenes::~Scenes()
    {
        ImGui::DestroyContext();
    }

}

