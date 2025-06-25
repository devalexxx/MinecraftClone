//
// Created by Alex on 11/09/2024.
//

#ifndef MCC_COMMON_NETWORK_NETWORK_MANAGER_H
#define MCC_COMMON_NETWORK_NETWORK_MANAGER_H

#include "Common/Network/EventManager.h"

#include <enet/enet.h>

#include <string>

#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 50505

namespace Mcc
{

	class NetworkManager : public NetworkEventManager
	{
		public:
			NetworkManager(const char* host, unsigned short port);
			~NetworkManager();

			void Poll();

			virtual int Setup() = 0;

		protected:
			static ENetPacket* CreatePacket(ByteSpan&& data, enet_uint32 flag);

			template<typename T>
			static int Send(ENetPeer* peer, T data, enet_uint32 flag, enet_uint8 channel);

		protected:
			int CreateHost(const ENetAddress* addr, size_t peers, size_t channels, enet_uint32 in, enet_uint32 out);

			ENetAddress mAddr;
			ENetHost*   mHost;
	};

}

#include <Common/Network/NetworkManager.inl>

#endif