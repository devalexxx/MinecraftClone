
#include <cereal/archives/binary.hpp>
#include <cereal/types/optional.hpp>

#include <fmt/base.h>

namespace Mcc
{

	template<typename Archive>
	void serialize(Archive& ar, EntityState& packet)
	{
		ar(packet.entity, packet.position, packet.rotation, packet.lastInputApplied);
	}

	template<typename Archive>
	void serialize(Archive& ar, PlayerInfo& packet)
	{
		ar(packet.entity);
	}

	template<typename Archive>
	void serialize(Archive& ar, ServerInfo& packet)
	{
		ar(packet.tickRate);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnPlayerInfo& packet)
	{
		ar(packet.info);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnServerInfo& packet)
	{
		ar(packet.info);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnPlayerInput& packet)
	{
		ar(packet.input);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnWorldEntitiesCreated& packet)
	{
		ar(packet.states);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnEntitiesDestroyed& packet)
	{
		ar(packet.entities);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnEntitiesUpdated& packet)
	{
		ar(packet.states);
	}

}