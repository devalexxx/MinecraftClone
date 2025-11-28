// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Common/SceneImporter.h"

namespace Mcc
{

    flecs::component<SceneRoot> GetSceneRoot(const flecs::world& world)
    {
        return world.component<SceneRoot>();
    }

}