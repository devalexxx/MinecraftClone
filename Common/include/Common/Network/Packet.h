//
// Created by Alex on 05/09/2024.
//

#ifndef MCC_COMMON_NETWORK_PACKET_H
#define MCC_COMMON_NETWORK_PACKET_H

#include "Common/Network/PacketStream.h"
#include "Common/Network/NetworkID.h"
#include "Common/Utils/TypeList.h"
#include "Common/PlayerInput.h"
#include "Common/World/Time.h"
#include "Common/Module/WorldEntity/Component.h"

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

			Transform transform;

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

#include <Common/Network/Packet.inl>

#endif