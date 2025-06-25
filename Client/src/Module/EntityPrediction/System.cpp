//
// Created by Alex on 25/06/2025.
//

#include "Client/Module/EntityPrediction/System.h"

#include "Common/Module/WorldEntity/Component.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	void InterpolateEntityTransform(Transform& transform, SnapshotQueue& queue)
	{
		const auto delay      =  std::chrono::milliseconds(50);
		const auto delayedNow = TimeClock::now() - delay;

		// Cleanup old snapshot
		while (!queue.data.empty() && queue.data.back().time < delayedNow - delay) { queue.data.pop_back(); }

		if (queue.data.empty())
		{
			return;
		}

		if (queue.data.size() == 1)
		{
			const Snapshot& snapshot = queue.data.front();
			transform.position = snapshot.transform.position;
			transform.rotation = snapshot.transform.rotation;
			transform.scale	   = snapshot.transform.scale;
			return;
		}

		for (size_t i = 0; i < queue.data.size() - 1; ++i)
		{
			if (queue.data[i].time >= delayedNow && queue.data[i + 1].time <= delayedNow)
			{
				const auto& a = queue.data[i];
				const auto& b = queue.data[i + 1];

				const auto elapsed = std::chrono::duration<float>(delayedNow - a.time).count();
				const auto length  = std::chrono::duration<float>(b.time     - a.time).count();

				transform.position = glm::mix(a.transform.position, b.transform.position, elapsed / length);
				transform.rotation = glm::mix(a.transform.rotation, b.transform.rotation, elapsed / length);
				transform.scale	   = a.transform.scale;
				break;
			}
		}
	}

}