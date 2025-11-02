//
// Created by Alex on 30/06/2025.
//

#ifndef MCC_SERVER_MODULE_TERRAIN_SYSTEM_H
#define MCC_SERVER_MODULE_TERRAIN_SYSTEM_H

#include <flecs.h>

namespace Mcc
{

    struct Transform;

    void OnPlayerCreatedObserver(flecs::entity entity, const Transform& transform);
    void OnPlayerMoveObserver   (flecs::iter& it, size_t row);

}

#endif