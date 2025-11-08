// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

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