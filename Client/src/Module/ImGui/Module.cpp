// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Module/ImGui/Module.h"

#include "Client/Module/Renderer/Module.h"
#include "Client/WorldContext.h"

#include "Common/Phase.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Mcc
{

    ImGuiModule::ImGuiModule(flecs::world& world) : BaseModule(world)
    {
        const auto ctx = ClientWorldContext::Get(world);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io     = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;// Enable Gamepad Controls

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(ctx->window.Get(), true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    ImGuiModule::~ImGuiModule()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiModule::RegisterComponent(flecs::world& /* world */)
    {}

    void ImGuiModule::RegisterSystem(flecs::world& world)
    {
        world.system().kind<Phase::OnSetup>().run([](flecs::iter&) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        });

        world.system().kind<Phase::OnDrawGui>().run([](flecs::iter&) {
            // ImGui::ShowDemoWindow();
        });

        world.system().kind<Phase::PostDrawGui>().run([](flecs::iter&) { ImGui::Render(); });

        world.system().kind<Phase::PreRender>().run([](flecs::iter&) {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        });
    }

    void ImGuiModule::RegisterHandler(flecs::world& /* world */)
    {}
}
