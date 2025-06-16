//
// Created by Alex on 11/09/2024.
//

#include <Server/ServerNetworkManager.h>

#include <MinecraftLib/CommandLineStore.h>

namespace Mcc
{

	ServerNetworkManager::ServerNetworkManager(const CommandLineStore& cmdLineStore) :
		NetworkManager(DEFAULT_HOST, DEFAULT_PORT), mMaxPeer(32)
	{
		CommandLineStore::OptParameter param;
		if ((param = cmdLineStore.GetParameter("host").or_else([&]{ return cmdLineStore.GetParameter("h"); })).has_value())
			enet_address_set_host(&mAddr, param->get().c_str());

		if ((param = cmdLineStore.GetParameter("port").or_else([&]{ return cmdLineStore.GetParameter("p"); })).has_value())
			mAddr.port = std::stoi(param->get());

		if ((param = cmdLineStore.GetParameter("peers")).has_value())
			mMaxPeer = std::stoul(param->get());
	}

	int ServerNetworkManager::Setup()
	{
		return CreateHost(&mAddr, mMaxPeer, 2, 0, 0);;
	}

}