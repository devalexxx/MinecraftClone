// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include <cereal/types/tuple.hpp>
#include <cereal/types/vector.hpp>

namespace Mcc
{

    template<typename Archive, typename T>
    void serialize(Archive& ar, ChunkData<T>& data)
    {
        ar(data.palette, data.mapping);
    }

    template<typename Archive>
    void serialize(Archive& ar, RLEChunkData& data)
    {
        ar(data.data);
    }

}