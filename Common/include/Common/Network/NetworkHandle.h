// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_NETWORK_NETWORK_HANDLE_H
#define MCC_COMMON_NETWORK_NETWORK_HANDLE_H

#include "Common/Export.h"

namespace Mcc
{
    using NetworkHandle = unsigned int;

    MCC_LIB_API NetworkHandle GenerateNetworkHandle();

    inline NetworkHandle Null()
    {
        return 0;
    }

    inline bool IsValid(const NetworkHandle handle)
    {
        return handle > 0;
    }


}

#endif