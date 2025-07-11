//
// Created by Alex on 27/08/2024.
//

#include "Common/World/Chunk.h"

#include "Common/Module/Network/Component.h"
#include "Common/Module/Terrain/Component.h"
#include "Common/WorldContext.h"

#include "Common/Utils/Logging.h"

#include <algorithm>

namespace Mcc
{

	Chunk::Chunk() : mData({}, { Size * Size * Height, 2 })
    {}

	Chunk::Chunk(flecs::entity_t filler) : mData({}, { Size * Size * Height, 2 })
	{
		mData.palette.emplace_back(filler);
		for (size_t i = 0; i < mData.mapping.GetSize(); ++i)
		{
			mData.mapping.Set(i, 0);
		}
	}

	Chunk::Chunk(ChunkData<flecs::entity_t> data) : mData(std::move(data)) {}

	flecs::entity_t Chunk::Get(const glm::ivec3 position) const
	{
		if (position.x < 0 || position.y < 0 || position.z < 0 || position.x >= Size || position.y >= Height || position.z >= Size)
			return flecs::entity::null().id();

		const size_t index = IndexFromPosition(position);
		return index < mData.mapping.GetSize() ? mData.palette[mData.mapping.Get(index)] : flecs::entity::null().id();
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

		if (index >= mData.mapping.GetSize())
			return;

		if (const auto it = std::ranges::find(mData.palette, entity); it == mData.palette.cend())
		{
			mData.palette.push_back(entity);
			mData.mapping.Set(index, mData.palette.size() - 1);
		}
		else
		{
			mData.mapping.Set(index, std::distance(mData.palette.begin(), it));
		}
	}

	const Chunk::Palette& Chunk::GetPalette() const
	{
		return mData.palette;
	}

	const BitArray& Chunk::GetMapping() const
    {
        return mData.mapping;
    }

    std::optional<ChunkData<NetworkHandle>> Chunk::ToNetwork(const flecs::world& world) const
    {
	    return Helper::ToNetwork(mData, world);
    }

    size_t Chunk::IndexFromPosition(const glm::uvec3 position)
	{
		return position.x + (position.y * Size) + (position.z * Size * Height);
	}

    namespace Helper
	{

	    std::optional<ChunkData<NetworkHandle>> ToNetwork(const ChunkData<flecs::entity_t>& data, const flecs::world& world)
	    {
	        ChunkData<NetworkHandle>::Palette palette;
	        for (auto id: data.palette)
	        {
	            if (const auto props = world.entity(id).try_get<NetworkProps>(); props)
	            {
                    if (!IsValid(props->handle))
                    {
                        MCC_LOG_WARN("The network id attached to #{} is invalid", id);
                        return std::nullopt;
                    }

	                palette.push_back(props->handle);
	            }
	            else
	            {
	                MCC_LOG_WARN("No network id attached to #{}", id);
                    return std::nullopt;
	            }
	        }

	        return {{ .palette=std::move(palette), .mapping=data.mapping }};
	    }

	    std::optional<ChunkData<flecs::entity_t>> FromNetwork(const ChunkData<NetworkHandle>& data, const flecs::world& world)
	    {
            const auto* ctx = WorldContext<>::Get(world);

	        ChunkData<flecs::entity_t>::Palette palette;
	        for (auto handle: data.palette)
	        {
                if (const auto id = ctx->networkMapping.GetLHandle(handle); id.has_value())
                {
                    if (!world.is_valid(*id))
                    {
                        MCC_LOG_WARN("The local id attached to #{} is invalid", handle);
                        return std::nullopt;
                    }

                    palette.push_back(*id);
                }
	            else
	            {
	                MCC_LOG_WARN("No local id attached to {}", handle);
	                return std::nullopt;
	            }
	        }

	        return {{ .palette=std::move(palette), .mapping=data.mapping }};
	    }

	}
}