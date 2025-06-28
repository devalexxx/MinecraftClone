//
// Created by Alex on 28/06/2025.
//

#ifndef MCC_CLIENT_MODULE_ENTITY_REPLICATION_SYSTEM_H
#define MCC_CLIENT_MODULE_ENTITY_REPLICATION_SYSTEM_H

#include "Client/Module/EntityReplication/Component.h"

#include "Common/Module/Entity/Component.h"

namespace Mcc
{

	void EntityInterpolationSystem(Transform& transform, SnapshotQueue& queue);

}

#endif
