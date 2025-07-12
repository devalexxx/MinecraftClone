//
// Created by Alex on 02/09/2024.
//

#ifndef MCC_CLIENT_CLIENT_APPLICATION_H
#define MCC_CLIENT_CLIENT_APPLICATION_H

#include "Client/ClientNetworkManager.h"
#include "Client/Graphics/Window/Window.h"

#include "Common/Application.h"
#include "Common/Utils/ThreadPool.h"

namespace Mcc
{

	class ClientApplication : public Application
	{
		public:
			ClientApplication();
			ClientApplication(int argc, char** argv);

			int Run() override;

		private:
			ClientNetworkManager mNetworkManager;
			Window mWindow;
	        ThreadPool mThreadPool;
	};

}

#endif
