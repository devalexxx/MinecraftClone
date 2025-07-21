//
// Created by Alex on 02/09/2024.
//

#ifndef MCC_CLIENT_CLIENT_APPLICATION_H
#define MCC_CLIENT_CLIENT_APPLICATION_H

#include "Client/ClientNetworkManager.h"
#include "Client/Graphics/Window/Window.h"
#include "ClientSettings.h"

#include "Common/Application.h"
#include "Common/Utils/ThreadPool.h"

#define SENSITIVITY_DEFAULT      0.001
#define FOV_DEFAULT              90
#define RENDER_DISTANCE_DEFAULT  8
#define PRELOAD_DISTANCE_DEFAULT 12

namespace Mcc
{

	class ClientApplication : public Application
	{
		public:
			ClientApplication();
			ClientApplication(int argc, char** argv);

			int Run() override;

		private:
	        ClientSettings       mSettings;
			ClientNetworkManager mNetworkManager;
			Window               mWindow;
	        ThreadPool           mThreadPool;
	};

}

#endif
