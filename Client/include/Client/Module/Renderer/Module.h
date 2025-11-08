// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_RENDERER_MODULE_H
#define MCC_CLIENT_MODULE_RENDERER_MODULE_H

#include <glm/fwd.hpp>

#include <flecs.h>

namespace Mcc
{

    struct RendererModule
    {
        explicit RendererModule(flecs::world& world);

        static std::tuple<glm::vec3, glm::mat4, glm::mat4> GetView(const flecs::world& world);
    };

}

#endif
