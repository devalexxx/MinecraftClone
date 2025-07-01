//
// Created by Alex on 30/06/2025.
//

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>

namespace Mcc
{

	template<typename Archive>
	void BitArray::serialize(Archive& ar)
	{
		ar(mData, mSize, mStride, mValuesPer64, mMask);
	}

}