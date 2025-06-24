//
// Created by Alex on 11/09/2024.
//

#pragma once

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
