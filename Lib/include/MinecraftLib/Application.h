//
// Created by Alex on 02/09/2024.
//

#ifndef MINECRAFT_APPLICATION_H
#define MINECRAFT_APPLICATION_H

#include <MinecraftLib/CommandLineStore.h>
#include <MinecraftLib/Network/NetworkID.h>

#include <flecs.h>
#include <enet/enet.h>

#include <unordered_map>

namespace Mcc
{

	class Application
	{
		public:
			Application(int argc, char** argv);

			virtual int Run() = 0;

		protected:
			flecs::world     mWorld;
			CommandLineStore mCmdLineStore;
	};

}

#endif
