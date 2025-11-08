// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_MODULE_NETWORK_MODULE_H
#define MCC_COMMON_MODULE_NETWORK_MODULE_H

#include <flecs.h>

namespace Mcc
{

    struct NetworkModule
    {
        explicit NetworkModule(const flecs::world& world);
    };

}

#endif
