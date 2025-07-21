//
// Created by Alex on 11/09/2024.
//

#include "Server/ServerNetworkManager.h"

#include "Common/CommandLineStore.h"

#include <charconv>

namespace Mcc
{

	ServerNetworkManager::ServerNetworkManager(const CommandLineStore& cmdLineStore) :
		NetworkManager(DEFAULT_HOST, DEFAULT_PORT), mMaxPeer(32)
	{
		CommandLineStore::OptParameter param;
		if ((param = cmdLineStore.GetParameter("host").or_else([&]{ return cmdLineStore.GetParameter("h"); })).has_value())
			enet_address_set_host(&mAddr, param->data());

		if ((param = cmdLineStore.GetParameter("port").or_else([&]{ return cmdLineStore.GetParameter("p"); })).has_value())
			std::from_chars(param->data(), param->data() + param->size(), mAddr.port);

		if ((param = cmdLineStore.GetParameter("peers")).has_value())
			std::from_chars(param->data(), param->data() + param->size(), mMaxPeer);
	}

	int ServerNetworkManager::Setup()
	{
		return CreateHost(&mAddr, mMaxPeer, 2, 0, 0);;
	}

}