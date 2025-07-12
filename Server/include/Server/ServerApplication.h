//
// Created by Alex on 02/09/2024.
//

#ifndef MCC_SERVER_SERVER_APPLICATION_H
#define MCC_SERVER_SERVER_APPLICATION_H

#include "Server/ServerNetworkManager.h"

#include "Common/Application.h"
#include "Common/Network/EventManager.h"
#include "Common/Utils/ThreadPool.h"

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

	        ThreadPool mThreadPool;
	};

}

#endif
