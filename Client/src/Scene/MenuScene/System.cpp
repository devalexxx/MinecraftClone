// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Scene/MenuScene/System.h"

#include "Client/Scene/Scene.h"
#include "Client/WorldContext.h"

#include "Common/SceneImporter.h"

#include "imgui.h"

namespace Mcc
{

    void DisplayMainMenuSystem(flecs::iter& it)
    {
        const auto world = it.world();
        const auto ctx   = ClientWorldContext::Get(world);

        ImGui::Begin("Main Menu");
        if (ImGui::Button("Enter Game"))
        {
            world.add<ActiveScene, GameScene>();
        }
        if (ImGui::Button("Quit Game"))
        {
            ctx->window.SetShouldClose();
        }
        ImGui::End();

        while (it.next());
    }

}
