//
// Created by Alex on 22/06/2025.
//

#ifndef MCC_SERVER_MODULE_ENTITY_REPLICATION_SYSTEM_H
#define MCC_SERVER_MODULE_ENTITY_REPLICATION_SYSTEM_H

#include <flecs.h>

namespace Mcc
{

	void BroadcastEntitiesCreated  (flecs::iter& it);
	void BroadcastEntitiesUpdated  (flecs::iter& it);
	void BroadcastEntitiesDestroyed(flecs::iter& it);

}

#endif
