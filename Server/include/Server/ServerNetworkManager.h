//
// Created by Alex on 11/09/2024.
//

#ifndef MINECRAFT_SERVERNETWORKMANAGER_H
#define MINECRAFT_SERVERNETWORKMANAGER_H

#include <MinecraftLib/Network/NetworkManager.h>

namespace Mcc
{

	class CommandLineStore;

	class ServerNetworkManager : public NetworkManager
	{
		public:
			ServerNetworkManager(const CommandLineStore& cmdLineStore);

			int Setup() override;

			template<typename T>
			int Send(ENetPeer* peer, T data, enet_uint32 flag, enet_uint8 channel) const;
			template<typename T>
			void Broadcast(T data, enet_uint32 flag, enet_uint8 channel) const;

		private:
			size_t mMaxPeer;

			friend class ServerApplication;
	};

}

#include <Server/ServerNetworkManager.inl>

#endif
