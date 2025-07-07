//
// Created by Alex on 27/08/2024.
//

#include "Common/Module/Terrain/Component.h"
#include "Common/World/Chunk.h"

#include <algorithm>

namespace Mcc
{

	Chunk::Chunk() :
		mPalette({}),
		mMapping({ Size * Size * Height, 2 })
	{}

	Chunk::Chunk(flecs::entity_t filler) : mMapping({ Size * Size * Height, 2 })
	{
		mPalette.emplace_back(filler);
		for (int i = 0; i < mMapping.GetSize(); ++i)
		{
			mMapping.Set(i, 0);
		}
	}

	Chunk::Chunk(Palette palette, BitArray mapping) :
		mPalette(std::move(palette)),
		mMapping(std::move(mapping))
	{}

	flecs::entity_t Chunk::Get(const glm::ivec3 position) const
	{
		if (position.x < 0 || position.y < 0 || position.z < 0 || position.x >= Size || position.y >= Height || position.z >= Size)
			return flecs::entity::null().id();

		const size_t index = IndexFromPosition(position);
		return index < mMapping.GetSize() ? mPalette[mMapping.Get(index)] : flecs::entity::null().id();
	}

	Hx::EnumArray<BlockFace, flecs::entity_t> Chunk::GetNeighbors(const glm::ivec3 position) const
	{
		static glm::ivec3 left	(-1,  0,  0);
		static glm::ivec3 right	( 1,  0,  0);
		static glm::ivec3 front	( 0,  0,  1);
		static glm::ivec3 back	( 0,  0, -1);
		static glm::ivec3 top	( 0,  1,  0);
		static glm::ivec3 bottom( 0, -1,  0);

		return {
			{ BlockFace::Left,   Get(position + left)   },
			{ BlockFace::Right,  Get(position + right)  },
			{ BlockFace::Front,  Get(position + front)  },
			{ BlockFace::Back,   Get(position + back)   },
			{ BlockFace::Top,    Get(position + top)    },
			{ BlockFace::Bottom, Get(position + bottom) }
		};
	}

	void Chunk::Set(const glm::uvec3 position, const flecs::entity_t entity)
	{
		const size_t index = IndexFromPosition(position);

		if (index >= mMapping.GetSize())
			return;

		if (const auto it = std::ranges::find(mPalette, entity); it == mPalette.cend())
		{
			mPalette.push_back(entity);
			mMapping.Set(index, mPalette.size() - 1);
		}
		else
		{
			mMapping.Set(index, std::distance(mPalette.begin(), it));
		}
	}

	const Chunk::Palette& Chunk::GetPalette() const
	{
		return mPalette;
	}

	const BitArray& Chunk::GetMapping() const
	{
		return mMapping;
	}

	size_t Chunk::IndexFromPosition(const glm::uvec3 position)
	{
		return position.x + (position.y * Size) + (position.z * Size * Height);
	}
}