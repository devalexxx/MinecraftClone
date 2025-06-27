//
// Created by Alex on 26/06/2025.
//

#include "Client/Module/Camera/Module.h"
#include "Client/Module/Camera/Component.h"
#include "Client/Module/Camera/Prefab.h"
#include "Client/Module/Camera/Tag.h"

#include "Common/Module/WorldEntity/Component.h"
#include "Common/Module/WorldEntity/Module.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	CameraModule::CameraModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<WorldEntityModule>(), "CameraModule require WorldEntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import CameraModule...");
		world.module<CameraModule>();

		world.component<CameraSettings>();
		world.component<CameraTag>();
		world.component<ActiveCameraTag>();

		world.prefab<CameraPrefab>()
		    .add<CameraTag>()
		    .set_auto_override<Transform>({})
			.set_auto_override<CameraSettings>({});
	}

}