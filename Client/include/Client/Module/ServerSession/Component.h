// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_SERVER_SESSION_COMPONENT_H
#define MCC_CLIENT_MODULE_SERVER_SESSION_COMPONENT_H
#include "Common/State.h"

namespace Mcc
{

    REGISTER_STATE(StateTargetScene, ServerConnectionState, Pending, Connected, Error)

}

#endif
