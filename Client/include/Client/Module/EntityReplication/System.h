//
// Created by Alex on 28/06/2025.
//

#ifndef MCC_CLIENT_MODULE_ENTITY_REPLICATION_SYSTEM_H
#define MCC_CLIENT_MODULE_ENTITY_REPLICATION_SYSTEM_H

#include "Client/Module/EntityReplication/Component.h"
#include "Client/Module/ServerSession/Component.h"

#include "Common/Module/Entity/Component.h"

#include <flecs.h>

namespace Mcc
{

    void DispatchInitialEntityStateSystem(const flecs::iter& it, size_t, InitialWorldState& iws);
	void EntityInterpolationSystem       (Transform& transform, SnapshotQueue& queue);

}

#endif
