//
// Created by Alex on 11/09/2024.
//

#ifndef MCC_CLIENT_CLIENT_NETWORK_MANAGER_H
#define MCC_CLIENT_CLIENT_NETWORK_MANAGER_H

#include "Common/Network/NetworkManager.h"

namespace Mcc
{

	class CommandLineStore;

	class ClientNetworkManager : public NetworkManager
	{
		public:
			ClientNetworkManager(const CommandLineStore& cmdLineStore);
			~ClientNetworkManager();

			int Setup() override;

			int Connect();
			int Disconnect();

			template<typename T>
			int Send(T data, enet_uint32 flag, enet_uint8 channel) const;

		private:
			ENetPeer* mPeer;

	};

}

#include <Client/ClientNetworkManager.inl>

#endif
