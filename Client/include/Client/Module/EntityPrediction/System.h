//
// Created by Alex on 25/06/2025.
//

#ifndef MCC_CLIENT_MODULE_ENTITY_PREDICTION_SYSTEM_H
#define MCC_CLIENT_MODULE_ENTITY_PREDICTION_SYSTEM_H

namespace Mcc
{

	struct Transform;
	struct SnapshotQueue;

	void InterpolateEntityTransform(Transform& transform, SnapshotQueue& queue);

}

#endif
