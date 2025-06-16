//
// Created by Alex on 02/09/2024.
//

#include <MinecraftLib/Application.h>

namespace Mcc
{

	Application::Application(int argc, char** argv) :
		mIsRunning(false), mCmdLineStore(argc, argv)
	{}

}