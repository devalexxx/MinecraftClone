//
// Created by Alex on 21/06/2025.
//

#ifndef MCC_MODULE_WORLD_ENTITY_MODULE_H
#define MCC_MODULE_WORLD_ENTITY_MODULE_H

#include <MinecraftLib/Module/WorldEntity/Component.h>
#include <MinecraftLib/Module/WorldEntity/Tag.h>
#include <MinecraftLib/Module/WorldEntity/Prefab.h>

#include <flecs.h>

namespace Mcc
{

	class WorldEntityModule
	{
		public:
			WorldEntityModule(flecs::world& world);
	};

}

#endif
