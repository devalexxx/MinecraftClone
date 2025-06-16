//
// Created by Alex on 11/09/2024.
//

#ifndef MINECRAFT_CLIENTNETWORKMANAGER_H
#define MINECRAFT_CLIENTNETWORKMANAGER_H

#include <MinecraftLib/Network/NetworkManager.h>

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
			int Send(T data, enet_uint32 flag, enet_uint8 channel);

		private:
			ENetPeer* mPeer;

	};

}

#include <Client/ClientNetworkManager.inl>

#endif
