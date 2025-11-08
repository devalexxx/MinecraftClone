// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_MODULE_TERRAIN_MODULE_H
#define MCC_COMMON_MODULE_TERRAIN_MODULE_H

#include <flecs.h>

namespace Mcc
{

    struct TerrainModule
    {
        explicit TerrainModule(const flecs::world& world);
    };

}

#endif
