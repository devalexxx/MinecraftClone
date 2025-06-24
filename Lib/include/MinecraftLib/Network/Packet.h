//
// Created by Alex on 05/09/2024.
//

#ifndef MINECRAFT_PACKET_H
#define MINECRAFT_PACKET_H

#include <MinecraftLib/Network/PacketStream.h>
#include <MinecraftLib/Network/NetworkID.h>
#include <MinecraftLib/Utils/TypeList.h>
#include <MinecraftLib/PlayerInput.h>
#include <MinecraftLib/World/Time.h>
#include <MinecraftLib/Module/WorldEntity/Component.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/queue.hpp>
#include <cereal/types/vector.hpp>

#include <flecs.h>

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

	using PacketList = TypeList<
		struct OnConnection,
		struct OnPlayerInput,
		struct OnEntitiesCreated,
		struct OnEntitiesDestroyed,
		struct OnEntitiesUpdated
	>;

	struct EntityState
	{
			NetworkID id;

			Position position;
			Rotation rotation;

			std::unordered_map<std::string, std::string> extra;
	};

	struct PlayerInfo
	{
			NetworkID id;
	};

	struct ServerInfo
	{
			TickRate tickRate;
	};

	template<typename Archive>
	void serialize(Archive& ar, EntityState& packet);
	template<typename Archive>
	void serialize(Archive& ar, PlayerInfo& packet);
	template<typename Archive>
	void serialize(Archive& ar, ServerInfo& packet);

	struct OnConnection
	{
			PlayerInfo playerInfo;
			ServerInfo serverInfo;

			std::vector<EntityState> initialStates;
	};

	struct OnPlayerInput
	{
			PlayerInput input;
	};

	struct OnEntitiesCreated
	{
			std::vector<EntityState> states;
	};

	struct OnEntitiesDestroyed
	{
			std::vector<NetworkID> ids;
	};

	struct OnEntitiesUpdated
	{
			std::vector<EntityState> states;
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

}

#include <MinecraftLib/Network/Packet.inl>

#endif
