//
// Created by Alex Clorennec on 10/07/2025.
//

#ifndef MCC_COMMON_MODULE_NETWORK_COMPONENT_H
#define MCC_COMMON_MODULE_NETWORK_COMPONENT_H

#include "Common/Network/NetworkHandle.h"

namespace Mcc
{

    struct ServerTag {};
    struct ClientTag {};
    struct NetworkObjectTag {};

    struct NetworkObjectPrefab {};

    struct NetworkProps
    {
            NetworkHandle handle;
    };

}

#endif
