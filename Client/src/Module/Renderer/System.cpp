// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Module/Renderer/System.h"

#include "Client/Graphics/Common.h"
#include "Client/Module/Renderer/Module.h"
#include "Client/WorldContext.h"

#include <glad/glad.h>

#include <ranges>

namespace Mcc
{

    void SetupRendererSystem(flecs::iter& it)
    {
        while (it.next()) {}
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
    }

    void PollWindowEventSystem(flecs::iter& it)
    {
        while (it.next()) {}
        Window::PollEvents();
    }

    void ClearFrameSystem(flecs::iter& it)
    {
        while (it.next()) {}
        glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        glCheck(glClearColor(1., 1., 1., 1.));
    }

    void RenderFrameSystem(flecs::iter& it)
    {
        while (it.next()) {}
        ClientWorldContext::Get(it.world())->window.SwapBuffer();
    }

}