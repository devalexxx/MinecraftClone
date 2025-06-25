//
// Created by Alex on 23/06/2025.
//

#include "Common/Module/PlayerEntity/Component.h"
#include "Common/Module/PlayerEntity/Module.h"
#include "Common/Module/PlayerEntity/Prefab.h"
#include "Common/Module/PlayerEntity/Tag.h"
#include "Common/Module/WorldEntity/Module.h"
#include "Common/Module/WorldEntity/Prefab.h"
#include "Common/PlayerInput.h"
#include "Common/Utils/Logging.h"
#include "Common/Utils/Assert.h"

namespace Mcc
{

	PlayerEntityModule::PlayerEntityModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<WorldEntityModule>(), "PlayerEntityModule require WorldEntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import PlayerEntityModule...");
		world.module<PlayerEntityModule>();

		world.component<PlayerEntityTag>();
		world.component<PlayerInputQueue>();

		world.prefab<PlayerEntityPrefab>()
		    .is_a<WorldEntityPrefab>()
			.add<PlayerEntityTag>()
			.set_auto_override<PlayerInputQueue>({});
	}

}