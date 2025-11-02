//
// Created by Alex on 05/09/2024.
//

#include <cereal/archives/binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/queue.hpp>
#include <cereal/types/vector.hpp>

namespace Mcc
{

	template<typename Archive>
	void serialize(Archive& ar, EntityState& packet)
	{
		ar(packet.handle, packet.transform, packet.extra);
	}

    template<typename Archive>
    void serialize(Archive& ar, OnChunk& packet)
	{
        ar(packet.handle, packet.position, packet.data);
	}

    template<typename Archive>
    void serialize(Archive& ar, OnBlock& packet)
    {
	    ar(packet.handle, packet.meta, packet.color, packet.type);
    }

    template<typename Archive>
	void serialize(Archive& ar, OnPlayerInput& packet)
	{
		ar(packet.input);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnEntitiesCreated& packet)
	{
		ar(packet.states);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnEntitiesDestroyed& packet)
	{
		ar(packet.handles);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnEntitiesUpdated& packet)
	{
		ar(packet.states);
	}

}