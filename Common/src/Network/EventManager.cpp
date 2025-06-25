//
// Created by Alex on 11/09/2024.
//

#include "Common/Network/EventManager.h"

namespace Mcc
{

	NetworkEventManager::NetworkEventManager() :
		EventManager<NetworkEventTag>()
	{
		PacketList::ForEachType<HandlerMapper>({ *this });
	}

	void NetworkEventManager::DispatchPacket(const ENetPeer* peer, const ENetPacket* packet)
	{
		PacketInputStream 		   stream(packet);
		cereal::BinaryInputArchive archive(stream);

		size_t index;
		archive(index);

		if (index < PacketList::Count)
			mHandlers[index](peer, packet, *this, std::move(archive));
		else
			Dispatch<MalformedPacketEvent>({ packet });
	}

}