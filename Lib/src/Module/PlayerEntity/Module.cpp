//
// Created by Alex on 23/06/2025.
//

#include <MinecraftLib/Module/PlayerEntity/Component.h>
#include <MinecraftLib/Module/PlayerEntity/Module.h>
#include <MinecraftLib/Module/PlayerEntity/Prefab.h>
#include <MinecraftLib/Module/PlayerEntity/Tag.h>
#include <MinecraftLib/Module/WorldEntity/Module.h>
#include <MinecraftLib/Module/WorldEntity/Prefab.h>
#include <MinecraftLib/PlayerInput.h>
#include <MinecraftLib/Utils/Logging.h>
#include <MinecraftLib/Utils/Assert.h>

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