//
// Created by Alex on 11/09/2024.
//

#ifndef MCC_COMMON_NETWORK_EVENT_MANAGER_H
#define MCC_COMMON_NETWORK_EVENT_MANAGER_H

#include "Common/Utils/EventManager.h"
#include "Common/Network/Packet.h"

#include <enet/enet.h>

namespace Mcc
{

	struct NetworkEventTag {};

	class NetworkEventManager : public EventManager<NetworkEventTag>
	{
		public:
			NetworkEventManager();

		protected:
			void DispatchPacket(ENetPeer* peer, const ENetPacket* packet);

		private:
			struct HandlerMapper
			{
					template<typename T>
					void operator()() const;

					NetworkEventManager& mEventManager;
			};

			using PacketHandle = std::function<void(ENetPeer* peer, const ENetPacket* packet, NetworkEventManager&, cereal::BinaryInputArchive&&)>;
			std::array<PacketHandle, PacketList::Count> mHandlers;

	};

}

#include <Common/Network/EventManager.inl>

#endif