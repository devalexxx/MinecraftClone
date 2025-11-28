// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_ENTITY_REPLICATION_MODULE_H
#define MCC_CLIENT_MODULE_ENTITY_REPLICATION_MODULE_H

#include "Common/Module/Base/Module.h"
#include "Common/Module/Entity/Module.h"

#include <flecs.h>

namespace Mcc
{

    struct OnEntitiesCreated;
    struct OnEntitiesDestroyed;
    struct OnEntitiesUpdated;

    class EntityReplicationModule final : public BaseModule<EntityReplicationModule, EntityModule>
    {
      public:
        explicit EntityReplicationModule(flecs::world& world);

        void RegisterComponent(flecs::world& world) override;
        void RegisterSystem(flecs::world& world) override;
        void RegisterHandler(flecs::world& world) override;

      private:
        static void OnEntitiesCreatedHandler(const flecs::world& world, const OnEntitiesCreated& event);
        static void OnEntitiesDestroyedHandler(const flecs::world& world, const OnEntitiesDestroyed& event);
        static void OnEntitiesUpdatedHandler(const flecs::world& world, const OnEntitiesUpdated& event);
    };

}

#endif
