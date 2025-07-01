//
// Created by Alex on 27/08/2024.
//

#include "Common/World/Chunk.h"
#include "Common/Utils/Assert.h"

namespace Mcc
{

	Chunk::Chunk() :
		mMapping({ Size * Size * Height, 2 }),
		mPalette({})
	{}

	Chunk::Chunk(flecs::entity_t filler) : mMapping({ Size * Size * Height, 2 })
	{
		mPalette.emplace_back(filler);
		for (int i = 0; i < mMapping.GetSize(); ++i)
		{
			mMapping.Set(i, 0);
		}
	}

	Chunk::Chunk(Chunk::Palette palette, BitArray mapping) :
		mPalette(std::move(palette)),
		mMapping(std::move(mapping))
	{}

	flecs::entity_t Chunk::Get(glm::uvec3 position) const
	{
		return mPalette[mMapping.Get(IndexFromPosition(position))];
	}

	void Chunk::Set(glm::uvec3 position, flecs::entity_t entity)
	{
		auto it = std::find(mPalette.begin(), mPalette.end(), entity);
		if (it == mPalette.cend())
		{
			mPalette.push_back(entity);
			mMapping.Set(IndexFromPosition(position), mPalette.size() - 1);
		}
		else
		{
			mMapping.Set(IndexFromPosition(position), std::distance(mPalette.begin(), it));
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

	size_t Chunk::IndexFromPosition(glm::uvec3 position)
	{
		return position.x + (position.y * Size) + (position.z * Size * Height);;
	}
}