// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_SERVER_MODULE_PLAYER_MODULE_H
#define MCC_SERVER_MODULE_PLAYER_MODULE_H

#include "Common/Network/Packet.h"

namespace Mcc
{

    class PlayerModule
    {
      public:
        explicit PlayerModule(flecs::world& world);

      private:
        static void OnPlayerInputHandler(const flecs::world& world, const From<OnPlayerInput>& from);
    };

}

#endif
