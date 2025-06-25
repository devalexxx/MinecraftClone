//
// Created by Alex on 11/09/2024.
//

#ifndef MCC_COMMON_NETWORK_EVENT_H
#define MCC_COMMON_NETWORK_EVENT_H

#include <enet/enet.h>

namespace Mcc
{

	struct ConnectEvent
	{
			ENetPeer* peer;
	};

	struct DisconnectEvent
	{
			ENetPeer* peer;
	};

	struct MalformedPacketEvent
	{
			const ENetPacket* packet;
	};

}

#endif