// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_SCENE_GAME_SCENE_COMPONENT_H
#define MCC_CLIENT_SCENE_GAME_SCENE_COMPONENT_H

#include <vector>

namespace Mcc
{

    struct DebugContext
    {
        std::vector<float> fpsHistory;
        size_t             fpsHistorySize;
        size_t             fpsHistoryIndex;
    };

}

#endif
