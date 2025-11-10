// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_MODULE_TERRAIN_COMPONENT_H
#define MCC_COMMON_MODULE_TERRAIN_COMPONENT_H

#include "Common/World/Chunk.h"

#include <memory>
#include <string>

namespace Mcc
{

    struct BlockTag {};
    struct BlockStateTag {};
    struct ChunkTag {};

    struct BlockPrefab {};
    struct ChunkPrefab {};

    struct BlockStateRelation {};

    enum class BlockType
    {
        Solid,
        Fluid,
        Gas
    };

    enum class BlockFace
    {
        Left,
        Right,
        Front,
        Back,
        Top,
        Bottom,
        Count
    };

    struct BlockColor
    {
        glm::vec3 color;
    };

    struct BlockMeta
    {
        std::string id;
    };

    struct ChunkPosition
    {
        glm::ivec3 position;
    };

    struct ChunkHolder
    {
        std::shared_ptr<Chunk> chunk;
    };

    template<typename Archive>
    void serialize(Archive& ar, BlockMeta& packet);
    template<typename Archive>
    void serialize(Archive& ar, ChunkPosition& packet);

}

#include "Common/Module/Terrain/Component.inl"

#endif