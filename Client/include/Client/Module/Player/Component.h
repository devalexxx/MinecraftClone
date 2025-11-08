// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_PLAYER_COMPONENT_H
#define MCC_CLIENT_MODULE_PLAYER_COMPONENT_H

#include "Common/Module/Entity/Component.h"

namespace Mcc
{

    struct PlayerEntityTag
    {};

    struct PlayerEntityPrefab
    {};

    struct CurrentPlayerInput
    {
        UserInput input;
    };

}

#endif
