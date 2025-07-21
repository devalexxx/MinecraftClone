//
// Created by Alex Clorennec on 17/07/2025.
//

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
