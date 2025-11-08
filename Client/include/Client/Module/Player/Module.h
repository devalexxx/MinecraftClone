// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_PLAYER_MODULE_H
#define MCC_CLIENT_MODULE_PLAYER_MODULE_H

#include "Client/Graphics/Window/Event.h"

#include "Common/Network/Packet.h"

#include <flecs.h>

namespace Mcc
{

    class PlayerModule
    {
      public:
        explicit PlayerModule(flecs::world& world);

      private:
        static void OnEntitiesCreatedHandler(const flecs::world& world, const OnEntitiesCreated& event);
        static void OnEntitiesUpdatedHandler(const flecs::world& world, const OnEntitiesUpdated& event);

        static void OnKeyEventHandler(const flecs::world& world, const KeyEvent& event);
        static void OnCursorPosEventHandler(const flecs::world& world, const CursorPosEvent& event);
    };

}

#endif
