//
// Created by Alex on 05/09/2024.
//

#ifndef MCC_COMMON_NETWORK_PACKET_H
#define MCC_COMMON_NETWORK_PACKET_H

#include "Common/Module/Entity/Component.h"
#include "Common/Module/Terrain/Component.h"
#include "Common/Network/NetworkHandle.h"
#include "Common/Network/PacketStream.h"
#include "Common/World/Time.h"

#include <Hexis/Core/TypeList.h>

#include <vector>
#include <unordered_map>
#include <string>

namespace Mcc
{

	template<typename T>
	struct From
	{
			const ENetPeer* peer;
			T packet;
	};

	using PacketList = Hx::TypeList<
		struct OnConnection,
		struct OnPlayerInput,
		struct OnEntitiesCreated,
		struct OnEntitiesDestroyed,
		struct OnEntitiesUpdated,
		struct OnBlocksCreated,
		struct OnBlocksDestroyed,
		struct OnBlocksUpdated,
		struct OnChunksCreated,
		struct OnChunksDestroyed,
		struct OnChunksUpdated
	>;

	struct EntityState
	{
			NetworkHandle handle;

			Transform transform;

			std::unordered_map<std::string, std::string> extra;
	};

	struct BlockDesc
	{
			NetworkHandle handle;
			BlockMeta     meta;
	        glm::vec3     color;
			BlockType     type;
	};

	struct ChunkDesc
	{
			NetworkHandle  	    handle;
			ChunkPosition       position;
			CompressedChunkData compressed;
	};

	struct PlayerInfo
	{
			NetworkHandle handle;
	};

	struct ServerInfo
	{
			TickRate tickRate;
	};

	template<typename Archive>
	void serialize(Archive& ar, EntityState& packet);
	template<typename Archive>
	void serialize(Archive& ar, BlockDesc& packet);
	template<typename Archive>
	void serialize(Archive& ar, ChunkDesc& packet);
	template<typename Archive>
	void serialize(Archive& ar, PlayerInfo& packet);
	template<typename Archive>
	void serialize(Archive& ar, ServerInfo& packet);

	struct OnConnection
	{
			PlayerInfo   playerInfo;
			ServerInfo   serverInfo;
	};

	struct OnPlayerInput
	{
			UserInput input;
	};

	struct OnEntitiesCreated
	{
			std::vector<EntityState> states;
	};

	struct OnEntitiesDestroyed
	{
			std::vector<NetworkHandle> handles;
	};

	struct OnEntitiesUpdated
	{
			std::vector<EntityState> states;
	};

	struct OnBlocksCreated
	{
			std::vector<BlockDesc> blocks;
	};

	struct OnBlocksDestroyed
	{
			std::vector<NetworkHandle> handles;
	};

	struct OnBlocksUpdated
	{
			std::vector<BlockDesc> blocks;
	};

	struct OnChunksCreated
	{
			std::vector<ChunkDesc> chunks;
	};

	struct OnChunksDestroyed
	{
			std::vector<NetworkHandle> handles;
	};

	struct OnChunksUpdated
	{
			std::vector<ChunkDesc> chunks;
	};

	template<typename Archive>
	void serialize(Archive& ar, OnConnection& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnPlayerInput& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnEntitiesCreated& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnEntitiesDestroyed& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnEntitiesUpdated& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnBlocksCreated& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnBlocksDestroyed& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnBlocksUpdated& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnChunksCreated& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnChunksDestroyed& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnChunksUpdated& packet);

}

#include <Common/Network/Packet.inl>

#endif