//
// Created by Alex on 02/09/2024.
//

#ifndef MCC_COMMON_APPLICATION_H
#define MCC_COMMON_APPLICATION_H

#include "Common/CommandLineStore.h"
#include "Common/Network/NetworkHandle.h"

#include <enet/enet.h>
#include <flecs.h>

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
