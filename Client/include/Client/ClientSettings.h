// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_CLIENT_SETTINGS_H
#define MCC_CLIENT_CLIENT_SETTINGS_H

#include "Common/Network/Packet/Session.h"

namespace Mcc
{

    struct ClientSettings : ClientInfo
    {
        float fov;
        float sensitivity;

        size_t renderDistance;
        size_t preloadDistance;
    };

}

#endif
