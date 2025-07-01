//
// Created by Alex on 27/08/2024.
//

#include "Common/World/Block.h"

namespace Mcc
{
	Block::Block(std::string name) : mName(std::move(name))
	{
	}

	bool operator==(const Block& lhs, const Block& rhs)
	{
		return lhs.mName == rhs.mName;
	}

	bool operator!=(const Block& lhs, const Block& rhs)
	{
		return !(lhs == rhs);
	}

	BlockInstance::BlockInstance(const Block& block) : mBlock(block)
	{
	}

	const Block& BlockInstance::GetBlock() const
	{
		return mBlock;
	}

	bool operator==(const BlockInstance& lhs, const BlockInstance& rhs)
	{
		return lhs.mBlock == rhs.mBlock;
	}

	bool operator!=(const BlockInstance& lhs, const BlockInstance& rhs)
	{
		return !(lhs == rhs);
	}
}