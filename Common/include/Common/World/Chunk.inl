//
// Created by Alex on 27/08/2024.
//

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/tuple.hpp>

namespace Mcc
{

    template<typename Archive, typename T>
    void serialize(Archive& ar, ChunkData<T>& data)
	{
	    ar(data.palette, data.mapping);
	}

    template<typename Archive>
    void serialize(Archive& ar, CompressedChunkData& data)
    {
        ar(data.data);
    }

}