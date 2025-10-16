//
// Created by Alex on 02/09/2024.
//

#ifndef MCC_SERVER_SERVER_APPLICATION_H
#define MCC_SERVER_SERVER_APPLICATION_H

#include "Server/ServerNetworkManager.h"
#include "ServerSettings.h"

#include "Common/Application.h"
#include "Common/Utils/ThreadPool.h"

#define TICK_RATE_DEFAULT 20
#define TICK_RATE_MAX     60
#define TICK_RATE_MIN     20

#define RENDER_DISTANCE_DEFAULT 24
#define RENDER_DISTANCE_MAX     64
#define RENDER_DISTANCE_MIN     8

#define USER_SPEED_DEFAULT 25

namespace Mcc
{

	class ServerApplication : public Application
	{
		public:
			ServerApplication(int argc, char** argv);
			ServerApplication();

			int Run() override;

		private:
			ServerSettings       mSettings;
			ServerNetworkManager mNetworkManager;
	        ThreadPool           mThreadPool;
	};

}

#endif
