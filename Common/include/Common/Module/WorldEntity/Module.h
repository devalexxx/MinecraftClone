//
// Created by Alex on 21/06/2025.
//

#ifndef MCC_COMMON_MODULE_WORLD_ENTITY_MODULE_H
#define MCC_COMMON_MODULE_WORLD_ENTITY_MODULE_H

#include "Common/Module/WorldEntity/Component.h"
#include "Common/Module/WorldEntity/Tag.h"
#include "Common/Module/WorldEntity/Prefab.h"

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
