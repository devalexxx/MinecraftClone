//
// Created by Alex on 29/06/2025.
//

#ifndef MCC_COMMON_MODULE_TERRAIN_COMPONENT_H
#define MCC_COMMON_MODULE_TERRAIN_COMPONENT_H

#include "Common/World/Chunk.h"

#include <flecs.h>

#include <memory>
#include <string>

namespace Mcc
{

	struct BlockTag {};
	struct BlockStateTag {};
	struct ChunkTag {};

	struct BlockStateRelation {};

	enum class BlockType
	{
		Solid, Fluid, Gas
	};

	struct BlockMeta
	{
			std::string id;
	};

	struct ChunkPosition
	{
			glm::uvec3 position;
	};

	struct ChunkData
	{
			std::unique_ptr<Chunk> data;
	};

	template<typename Archive>
	void serialize(Archive& ar, BlockMeta& packet);
	template<typename Archive>
	void serialize(Archive& ar, ChunkPosition& packet);

}

#include "Common/Module/Terrain/Component.inl"

#endif