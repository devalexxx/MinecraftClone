// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_SERVER_MODULE_TERRAIN_COMPONENT_H
#define MCC_SERVER_MODULE_TERRAIN_COMPONENT_H

namespace Mcc
{

    struct ChunkDirtyTag
    {};
    struct ChunkCreatedTag
    {};
    struct ChunkDestroyedTag
    {};

    struct BlockDirtyTag
    {};
    struct BlockCreatedTag
    {};
    struct BlockDestroyedTag
    {};

}

#endif