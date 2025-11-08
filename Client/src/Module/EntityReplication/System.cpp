// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Module/EntityReplication/System.h"

#include "Common/WorldContext.h"

namespace Mcc
{

    void EntityInterpolationSystem(Transform& transform, SnapshotQueue& queue)
    {
        constexpr auto delay      = std::chrono::milliseconds(50);
        const auto     delayedNow = TimeClock::now() - delay;

        // Cleanup old snapshot
        while (!queue.data.empty() && queue.data.back().time < delayedNow - delay) { queue.data.pop_back(); }

        if (queue.data.empty())
        {
            return;
        }

        if (queue.data.size() == 1)
        {
            const auto& [tr, time] = queue.data.front();
            transform.position     = tr.position;
            transform.rotation     = tr.rotation;
            transform.scale        = tr.scale;
            return;
        }

        for (size_t i = 0; i < queue.data.size() - 1; ++i)
        {
            if (queue.data[i].time >= delayedNow && queue.data[i + 1].time <= delayedNow)
            {
                const auto& [aTr, aTime] = queue.data[i];
                const auto& [bTr, bTime] = queue.data[i + 1];

                const auto elapsed = std::chrono::duration<float>(delayedNow - aTime).count();
                const auto length  = std::chrono::duration<float>(bTime - aTime).count();

                transform.position = glm::mix(aTr.position, bTr.position, elapsed / length);
                transform.rotation = glm::slerp(aTr.rotation, bTr.rotation, elapsed / length);
                transform.scale    = aTr.scale;
                break;
            }
        }
    }

}