//
// Created by Alex on 27/08/2024.
//

#ifndef MCC_COMMON_WORLD_BLOCK_H
#define MCC_COMMON_WORLD_BLOCK_H

#include <string>

namespace Mcc
{

	class Block
	{
		public:
			Block(std::string name);

			friend bool operator==(const Block& lhs, const Block& rhs);
			friend bool operator!=(const Block& lhs, const Block& rhs);

		private:
			std::string mName;
	};

	class BlockInstance
	{
		public:
			BlockInstance(const Block& block);

			[[nodiscard]] const Block& GetBlock() const;

			friend bool operator==(const BlockInstance& lhs, const BlockInstance& rhs);
			friend bool operator!=(const BlockInstance& lhs, const BlockInstance& rhs);

		private:
			const Block& mBlock;
	};

}

#endif
