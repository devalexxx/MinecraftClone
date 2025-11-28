// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_MODULE_NETWORK_COMPONENT_H
#define MCC_COMMON_MODULE_NETWORK_COMPONENT_H

#include "Common/Export.h"
#include "Common/Network/NetworkHandle.h"

namespace Mcc
{

    struct ServerTag
    {};
    struct ClientTag
    {};
    struct NetworkObjectTag
    {};

    struct NetworkObjectPrefab
    {};

    struct NetworkProps
    {
        NetworkHandle handle;
    };

}

#endif
