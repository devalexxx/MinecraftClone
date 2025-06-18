//
// Created by Alex on 05/09/2024.
//

#ifndef MINECRAFT_PACKET_H
#define MINECRAFT_PACKET_H

#include <MinecraftLib/Component/Transform.h>
#include <MinecraftLib/Network/PacketStream.h>
#include <MinecraftLib/Utils/TypeList.h>
#include <MinecraftLib/PlayerInput.h>

#include <vector>

#include "MinecraftLib/World/Time.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/queue.hpp>
#include <cereal/types/vector.hpp>
#include <flecs.h>

namespace Mcc
{

	template<typename T>
	struct From
	{
			const ENetPeer* peer;
			T packet;
	};

	using PacketList = TypeList<
	    struct OnPlayerInfo,
		struct OnServerInfo,
		struct OnPlayerInput,
		struct OnWorldEntitiesCreated,
		struct OnEntitiesDestroyed,
		struct OnEntitiesUpdated
	>;

	struct EntityState
	{
			flecs::entity_t entity {};

			Position position {};
			Rotation rotation {};

			std::optional<unsigned short> lastInputApplied;
	};

	struct PlayerInfo
	{
			flecs::entity_t entity;
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

	struct OnPlayerInfo
	{
			PlayerInfo info;
	};

	struct OnServerInfo
	{
			ServerInfo info;
	};

	struct OnPlayerInput
	{
			PlayerInput input;
	};

	struct OnWorldEntitiesCreated
	{
			std::vector<EntityState> states;
	};

	struct OnEntitiesDestroyed
	{
			std::vector<flecs::entity_t> entities;
	};

	struct OnEntitiesUpdated
	{
			std::vector<EntityState> states;
	};

	template<typename Archive>
	void serialize(Archive& ar, OnPlayerInfo& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnServerInfo& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnPlayerInput& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnWorldEntitiesCreated& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnEntitiesDestroyed& packet);
	template<typename Archive>
	void serialize(Archive& ar, OnEntitiesUpdated& packet);

}

#include <MinecraftLib/Network/Packet.inl>

#endif
