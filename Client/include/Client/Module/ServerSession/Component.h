//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_CLIENT_MODULE_SERVER_SESSION_COMPONENT_H
#define MCC_CLIENT_MODULE_SERVER_SESSION_COMPONENT_H

#include "Common/Network/Packet.h"

#include <vector>

namespace Mcc
{

	enum class ServerConnectionState
	{
		Pending, Connected, Error
	};

}

#endif
