//
// Created by Alex on 27/08/2024.
//

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>

namespace Mcc
{

	template<typename Archive>
	void Chunk::serialize(Archive& ar)
	{
		ar(mPalette, mMapping);
	}

}