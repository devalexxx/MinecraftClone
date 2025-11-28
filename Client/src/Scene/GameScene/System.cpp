// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Scene/GameScene/System.h"

#include "Client/Module/TerrainRenderer/Component.h"
#include "Client/Scene/GameScene/Component.h"
#include "Client/Scene/GameScene/Scene.h"
#include "Client/Scene/Scene.h"
#include "Client/WorldContext.h"

#include "Hexis/Core/LambdaAsFuncPtr.h"
#include "Hexis/Math/FloatingPoint.h"

#include <algorithm>

#include "imgui.h"

namespace Mcc
{

    void SetupStateSystem(flecs::iter& it)
    {
        while (it.next()) {};
        GameState::Load::Enter(it.world());
    }

    void ConnectToServerSystem(flecs::iter& it)
    {
        while (it.next()) {};

        const auto ctx = ClientWorldContext::Get(it.world());
        ctx->scheduler.Insert([=]() { ctx->networkManager.Connect(); }).Enqueue();
    }

    void ClearGameInfoSystem(flecs::iter& it)
    {
        while (it.next()) {};

        const auto ctx      = ClientWorldContext::Get(it.world());
        ctx->chunkMap       = {};
        ctx->networkMapping = {};
    }

    void DisconnectFromServerSystem(flecs::iter& it)
    {
        while (it.next()) {};

        const auto ctx = ClientWorldContext::Get(it.world());
        ctx->networkManager.Disconnect();
    }

    void DisplayEscapeMenuSystem(const flecs::iter& it, size_t)
    {
        const auto world = it.world();
        const auto ctx   = ClientWorldContext::Get(world);
        ImGui::Begin("Escape Menu");
        if (ImGui::Button("Return"))
        {
            GameState::InGame::Enter(world);
        }
        if (ImGui::Button("Back Main Menu"))
        {
            GameState::Enter<GameState::Shutdown>(world);
        }
        if (ImGui::Button("Quit Game"))
        {
            ctx->window.SetShouldClose();
        }
        ImGui::End();
    }

    void DisplayLoadScreenSystem(const flecs::iter&, size_t)
    {
        ImGui::Begin("Loading...");
        ImGui::End();
    }

    void JoinPendingMeshTaskSystem(const flecs::iter& it, size_t)
    {
        if (const auto ctx = ClientWorldContext::Get(it.world()); ctx->scheduler.IsJoined("game_group"))
        {
            ctx->scheduler.StopJoin("game_group");
            it.world().add<ActiveScene, MenuScene>();
        }
    }

    void DisplayDebugMenuSystem(flecs::iter& it)
    {
        static float  elapsed = 0, max = 0;
        static size_t frames      = 0;
        static char   overlay[50] = "value";

        const auto ctx  = it.ctx<DebugContext>();
        elapsed        += it.delta_time();
        frames++;
        if (elapsed >= .2f)
        {
            const auto old   = ctx->fpsHistory[ctx->fpsHistoryIndex];
            const auto value = static_cast<float>(frames) / elapsed;
            if (value > max)
                max = value;

            ctx->fpsHistory[ctx->fpsHistoryIndex] = value;
            if (Hx::Equal(max, old))
                max = *std::ranges::max_element(ctx->fpsHistory);

            snprintf(overlay, 50, "value %.2f", value);
            ctx->fpsHistoryIndex = (ctx->fpsHistoryIndex + 1) % ctx->fpsHistorySize;
            elapsed              = 0;
            frames               = 0;
        }

        const auto plotter = Hx::LambdaAsFuncPtr<float (*)(void*, int)>([&ctx](void*, const int i) {
            const auto index = (ctx->fpsHistoryIndex + i) % ctx->fpsHistorySize;
            return ctx->fpsHistory[index];
        });


        ImGui::Begin("Debug");
        ImGui::PlotLines(
            "FPS", plotter, &ctx->fpsHistory, ctx->fpsHistorySize, 0, overlay, 0.0f, max * 1.4f, ImVec2(200, 50)
        );

        if (const auto peer = ClientWorldContext::Get(it.world())->networkManager.GetPeer())
        {
            if (ImGui::CollapsingHeader("Network"))
            {
                ImGui::Text("ping %d ms", peer->roundTripTime);
                ImGui::Text("loss %d", peer->packetLoss);
                ImGui::Text("packet sent: %d", peer->packetsSent);
            }
        }

        if (ImGui::CollapsingHeader("Chunk"))
        {
            const auto totalChunk = it.world().count<ChunkTag>();
            const auto queued     = it.world().count<MeshHolder>();
            ImGui::Text("queued : %d", queued);
            ImGui::Text("loaded : %d", totalChunk - queued);
            ImGui::Text("display: %d", it.world().count<ShouldRenderChunkTag>());
        }
        ImGui::End();

        while (it.next()) {}
    }

}
