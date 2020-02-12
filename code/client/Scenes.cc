#include "Scenes.h"

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "config.h"

namespace redsquare {

    Scenes::Scenes()
    : gf::SceneManager("Redsquare", {1024, 576})
    , resources({ REDSQUARE_DATA_DIR })
    , mainMenu(*this)
    , connection(*this)
    , game(*this)
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

