//
// Created by Alex on 27/08/2024.
//

#ifndef MCC_COMMON_WORLD_CHUNK_H
#define MCC_COMMON_WORLD_CHUNK_H

#include "Common/Utils/BitArray.h"

#include <Hexis/Core/EnumArray.h>

#include <glm/glm.hpp>

#include <flecs.h>

#include <vector>

namespace Mcc
{
	enum class BlockFace;

	class Chunk
	{
		public:
			using Palette = std::vector<flecs::entity_t>;

			static constexpr unsigned char Size   = 15;
			static constexpr unsigned char Height = 31;

			Chunk();
			Chunk(flecs::entity_t filler);
			Chunk(Palette palette, BitArray mapping);

			[[nodiscard]] flecs::entity_t Get(glm::ivec3 position) const;
			[[nodiscard]] Hx::EnumArray<BlockFace, flecs::entity_t> GetNeighbors(glm::ivec3 position) const;

			void Set(glm::uvec3 position, flecs::entity_t entity);

			[[nodiscard]] const Palette&  GetPalette() const;
			[[nodiscard]] const BitArray& GetMapping() const;

			template<typename Archive>
			void serialize(Archive& ar);

		private:
			Palette  mPalette;
			BitArray mMapping;

			static size_t IndexFromPosition(glm::uvec3 position) ;
	};

}

#include "Common/World/Chunk.inl"

#endif
