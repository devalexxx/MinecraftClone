#include <cereal/archives/binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/queue.hpp>
#include <cereal/types/vector.hpp>

#include <fmt/base.h>

namespace Mcc
{

	template<typename Archive>
	void serialize(Archive& ar, EntityState& packet)
	{
		ar(packet.id, packet.transform, packet.extra);
	}

	template<typename Archive>
	void serialize(Archive& ar, BlockDesc& packet)
	{
		ar(packet.id, packet.meta);
	}

	template<typename Archive>
	void serialize(Archive& ar, ChunkDesc& packet)
	{
		ar(packet.id, packet.position, packet.data);
	}

	template<typename Archive>
	void serialize(Archive& ar, InitialState& packet)
	{
		ar(packet.entities, packet.blocks, packet.chunks);
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
		ar(packet.playerInfo, packet.serverInfo, packet.initialState);
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

	template<typename Archive>
	void serialize(Archive& ar, OnBlocksCreated& packet)
	{
		ar(packet.blocks);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnBlocksDestroyed& packet)
	{
		ar(packet.ids);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnBlocksUpdated& packet)
	{
		ar(packet.blocks);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnChunksCreated& packet)
	{
		ar(packet.chunks);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnChunksDestroyed& packet)
	{
		ar(packet.ids);
	}

	template<typename Archive>
	void serialize(Archive& ar, OnChunksUpdated& packet)
	{
		ar(packet.chunks);
	}

}