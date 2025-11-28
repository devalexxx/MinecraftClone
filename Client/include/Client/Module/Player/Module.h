// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_PLAYER_MODULE_H
#define MCC_CLIENT_MODULE_PLAYER_MODULE_H

#include "Client/Graphics/Window/Event.h"
#include "Client/Module/Camera/Module.h"
#include "Client/Module/EntityReplication/Module.h"

#include "Common/Module/Base/Module.h"
#include "Common/Utils/EventManager.h"

#include <flecs.h>

namespace Mcc
{

    class PlayerModule final : public BaseModule<PlayerModule, EntityReplicationModule, CameraModule>
    {
      public:
        explicit PlayerModule(flecs::world& world);

        void RegisterComponent(flecs::world& world) override;
        void RegisterSystem(flecs::world& world) override;
        void RegisterHandler(flecs::world& world) override;

      private:
        static void OnEntitiesCreatedHandler(const flecs::world& world, const OnEntitiesCreated& event);
        static void OnEntitiesUpdatedHandler(const flecs::world& world, const OnEntitiesUpdated& event);

        static void OnKeyEventHandler(const flecs::world& world, const KeyEvent& event);
        static void OnCursorPosEventHandler(const flecs::world& world, const CursorPosEvent& event);

        EventHandlerID mKeyEventHandlerID;
        EventHandlerID mCursorPosEventHandlerID;
    };

}

#endif
