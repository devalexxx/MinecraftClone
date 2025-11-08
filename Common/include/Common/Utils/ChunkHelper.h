// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_UTILS_CHUNK_HELPER_H
#define MCC_COMMON_UTILS_CHUNK_HELPER_H

#include <Common/Module/Entity/Component.h>

#include <glm/glm.hpp>

#include <functional>

namespace Mcc::Helper
{

    std::pair<int, int> GetPlayerChunkPosition(const glm::vec3& position);

    bool IsInCircle(const glm::vec2& c, const glm::vec2& p, long radius);
    void ForInCircle(long x, long y, int radius, std::function<void(long x, long y)>&& fn);

}

#endif
