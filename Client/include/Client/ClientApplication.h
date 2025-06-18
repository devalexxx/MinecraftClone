//
// Created by Alex on 02/09/2024.
//

#ifndef MINECRAFT_CLIENTAPPLICATION_H
#define MINECRAFT_CLIENTAPPLICATION_H

#include <Client/ClientNetworkManager.h>
#include <Client/Graphics/Window/Window.h>
#include <Client/System/RenderSystem.h>

#include <MinecraftLib/Application.h>
#include <MinecraftLib/Network/Packet.h>

#include <enet/enet.h>


#include "MinecraftLib/Component/Input.h"
#include <istream>
#include <streambuf>

namespace Mcc
{

	class ClientApplication : public Application
	{
		public:
			ClientApplication();
			ClientApplication(int argc, char** argv);

			void SetupWorld();

			int Run() override;

		private:
			ClientNetworkManager mNetworkManager;

			bool 	   mHasPlayerInfo;
			PlayerInfo mPlayerInfo;
			bool 	   mHasServerInfo;
			ServerInfo mServerInfo;

			Window mWindow;

			PlayerInput 	 mInput;
			PlayerInputQueue mInputQueue;

			RenderSystem mRenderSystem;
	};

}

#endif
