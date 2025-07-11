//
// Created by Alex Clorennec on 11/07/2025.
//

#ifndef MCC_COMMON_MODULE_NETWORK_SYSTEM_H
#define MCC_COMMON_MODULE_NETWORK_SYSTEM_H

#include "Common/Module/Network/Component.h"

#include <flecs.h>

namespace Mcc
{

    void GenerateNetworkHandleObserver(flecs::entity entity, NetworkProps& props);
    void AddToContextObserver         (flecs::iter& it);
    void RemoveFromContextObserver    (flecs::iter& it);

}

#endif
