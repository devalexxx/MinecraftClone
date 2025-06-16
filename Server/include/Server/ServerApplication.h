//
// Created by Alex on 02/09/2024.
//

#ifndef MINECRAFT_SERVERAPPLICATION_H
#define MINECRAFT_SERVERAPPLICATION_H

#include <MinecraftLib/Application.h>
#include <MinecraftLib/Network/EventManager.h>
#include <Server/ServerNetworkManager.h>

#include <enet/enet.h>

namespace cereal
{
	class BinaryInputArchive;
}

namespace Mcc
{

	class ServerApplication : public Application
	{
		public:
			ServerApplication(int argc, char** argv);
			ServerApplication();

			int Run() override;

		private:
			ServerInfo mInfo;

			ServerNetworkManager mNetworkManager;
	};

}

#endif
