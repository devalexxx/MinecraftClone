//
// Created by Alex on 28/06/2025.
//

#ifndef MCC_CLIENT_MODULE_ENTITY_REPLICATION_COMPONENT_H
#define MCC_CLIENT_MODULE_ENTITY_REPLICATION_COMPONENT_H

#include "Common/Module/Entity/Component.h"

#include <chrono>

namespace Mcc
{

	struct InterpolationExcludedTag {};

	using TimeClock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<TimeClock>;

	struct Snapshot
	{
			Transform transform{};
			TimePoint time;
	};

	struct SnapshotQueue
	{
			std::deque<Snapshot> data;
	};

}

#endif
