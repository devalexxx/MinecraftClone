//
// Created by Alex on 21/06/2025.
//

#include <MinecraftLib/Module/PlayerEntity/Module.h>
#include <MinecraftLib/Module/WorldEntity/Module.h>
#include <MinecraftLib/Utils/Logging.h>

namespace Mcc
{

	WorldEntityModule::WorldEntityModule(flecs::world& world)
	{
		MCC_LOG_DEBUG("Import WorldEntityModule...");
		world.module<WorldEntityModule>();

		world.component<WorldEntityTag>();
		world.component<WorldEntityCreatedTag>();
		world.component<WorldEntityUpdatedTag>();
		world.component<WorldEntityDestroyedTag>();
		world.component<WorldEntityExtra>();
		world.component<Position>();
		world.component<Rotation>();
		world.component<Forward>();
		world.component<Right>();

		world.prefab<WorldEntityPrefab>()
			.add<WorldEntityTag>()
			.set_auto_override<WorldEntityExtra>({})
			.set_auto_override<Position>({{ 0.f, 2.f, -4.f }})
			.set_auto_override<Rotation>({{ 0.f, 0.f 	   }})
			.set_auto_override<Forward> ({{ 0.f, 0.f, 0.f  }})
			.set_auto_override<Right>	({{ 0.f, 0.f, 0.f  }});
	}

}