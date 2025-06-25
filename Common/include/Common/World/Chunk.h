//
// Created by Alex on 27/08/2024.
//

#ifndef MCC_COMMON_WORLD_CHUNK_H
#define MCC_COMMON_WORLD_CHUNK_H

#include "Common/World/Block.h"

#include <array>

namespace Mcc
{

	class Chunk
	{
		public:
			static constexpr char Size   = 15;
			static constexpr char Height = 31;

		private:
			std::array<Block::TypeId, Size * Size * Height> mBlocks;
	};

}

#endif
