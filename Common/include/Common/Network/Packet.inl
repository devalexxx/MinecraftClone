#include <cereal/archives/binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/string.hpp>

#include <fmt/base.h>

namespace Mcc
{

	template<typename Archive>
	void serialize(Archive& ar, EntityState& packet)
	{
		ar(packet.id, packet.transform, packet.extra);
	}

	template<typename Archive>
	void serialize(Archive& ar, PlayerInfo& packet)
	{
		ar(packet.id);
	}

	template<typename Archive>
	void serialize(Archive& ar, ServerInfo& packet)
	{
		ar(packet.tickRate);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnConnection& packet)
	{
		ar(packet.playerInfo, packet.serverInfo, packet.initialStates);
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
		ar(packet.ids);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnEntitiesUpdated& packet)
	{
		ar(packet.states);
	}

}