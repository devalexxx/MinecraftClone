// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_CAMERA_MODULE_H
#define MCC_CLIENT_MODULE_CAMERA_MODULE_H

#include <flecs.h>

namespace Mcc
{

    struct CameraModule
    {
        explicit CameraModule(const flecs::world& world);
    };

}

#endif
