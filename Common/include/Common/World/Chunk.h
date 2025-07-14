//
// Created by Alex on 27/08/2024.
//

#ifndef MCC_COMMON_WORLD_CHUNK_H
#define MCC_COMMON_WORLD_CHUNK_H

#include "Common/Network/NetworkHandle.h"
#include "Common/Utils/BitArray.h"

#include <Hexis/Core/EnumArray.h>

#include <glm/glm.hpp>

#include <flecs.h>

#include <vector>

namespace Mcc
{
	enum class BlockFace;

    template<typename T>
    struct ChunkData
    {
        using Palette = std::vector<T>;

        Palette  palette{};
        BitArray mapping{0, 1};
    };

    struct CompressedChunkData
    {
        std::vector<std::tuple<NetworkHandle, size_t>> data;
    };

    template<typename Archive, typename T>
    void serialize(Archive& ar, ChunkData<T>& data);
    template<typename Archive>
    void serialize(Archive& ar, CompressedChunkData& data);

	class Chunk
	{
		public:
			using Palette = ChunkData<flecs::entity_t>::Palette;

			static constexpr unsigned char Size   = 15;
			static constexpr unsigned char Height = 127;

			Chunk();
			Chunk(flecs::entity_t filler);
			Chunk(ChunkData<flecs::entity_t> data);

			[[nodiscard]] flecs::entity_t Get(glm::ivec3 position) const;
			[[nodiscard]] Hx::EnumArray<BlockFace, flecs::entity_t> GetNeighbors(glm::ivec3 position) const;

			void Set(glm::uvec3 position, flecs::entity_t entity);

			[[nodiscard]] const Palette&  GetPalette() const;
			[[nodiscard]] const BitArray& GetMapping() const;

	        [[nodiscard]] std::optional<CompressedChunkData> ToNetwork(const flecs::world& world) const;

		private:
	        ChunkData<flecs::entity_t> mData;

			static size_t IndexFromPosition(glm::uvec3 position) ;
	};

    namespace Helper
    {

        std::optional<CompressedChunkData>        ToNetwork  (const ChunkData<flecs::entity_t>& data, const flecs::world& world);
        std::optional<ChunkData<flecs::entity_t>> FromNetwork(const CompressedChunkData&        data, const flecs::world& world);

    }

}

#include "Common/World/Chunk.inl"

#endif
