// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Module/Camera/Module.h"

#include "Client/Module/Camera/Component.h"
#include "Client/Module/Camera/System.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Module/Entity/Module.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

    CameraModule::CameraModule(const flecs::world& world)
    {
        MCC_ASSERT(world.has<EntityModule>(), "CameraModule require WorldEntityModule, you must import it before.");
        MCC_LOG_DEBUG("Import CameraModule...");
        world.module<CameraModule>();

        world.component<CameraTag>();
        world.component<CameraFollowTag>();
        world.component<ActiveCameraTag>();

        world.component<CameraFollowRelation>();

        world.component<CameraSettings>();

        world.system<Transform, const CameraFollowSettings>()
            .with<CameraFollowTag>()
            .with<CameraFollowRelation>(flecs::Wildcard)
            .each(CameraFollowSystem);

        world.prefab<CameraPrefab>().is_a<EntityPrefab>().add<CameraTag>().set_auto_override<CameraSettings>({});

        world.prefab<CameraFollowPrefab>()
            .is_a<CameraPrefab>()
            .add<CameraFollowTag>()
            .set_auto_override<CameraFollowSettings>({});
    }

}