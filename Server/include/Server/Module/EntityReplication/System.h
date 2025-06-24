//
// Created by Alex on 22/06/2025.
//

#ifndef MINECRAFT_MODULE_SERVER_WORLD_ENTITY_SYSTEM_H
#define MINECRAFT_MODULE_SERVER_WORLD_ENTITY_SYSTEM_H

#include <flecs.h>

namespace Mcc
{

	void BroadcastEntitiesCreated  (flecs::iter& it);
	void BroadcastEntitiesUpdated  (flecs::iter& it);
	void BroadcastEntitiesDestroyed(flecs::iter& it);

}

#endif
