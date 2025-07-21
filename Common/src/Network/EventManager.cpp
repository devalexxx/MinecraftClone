//
// Created by Alex on 11/09/2024.
//

#include "Common/Network/EventManager.h"

#include "Common/Utils/Benchmark.h"
#include "Common/Utils/Logging.h"
#include "zlib.h"

namespace Mcc
{

	NetworkEventManager::NetworkEventManager() :
		EventManager<NetworkEventTag>()
	{
		PacketList::Apply<HandlerMapper>({ *this });
	}

	void NetworkEventManager::DispatchPacket(const ENetPeer* peer, const ENetPacket* packet)
	{
        size_t length;
	    std::memcpy(&length, packet->data, sizeof(length));

	    std::vector<uint8_t> buffer(length);
	    if (MCC_BENCH_TIME(Uncompression, uncompress)(buffer.data(),reinterpret_cast<uLongf*>(&length), packet->data + sizeof(length), packet->dataLength - sizeof(length)) != Z_OK)
	    {
	        MCC_LOG_ERROR("Failed to uncompress data");
	    }

	    PacketInputStream          stream (reinterpret_cast<char*>(buffer.data()), length);
	    cereal::BinaryInputArchive archive(stream);

		size_t index;
		archive(index);

		if (index < PacketList::Count)
			mHandlers[index](peer, packet, *this, std::move(archive));
		else
			Dispatch<MalformedPacketEvent>({ packet });
	}

}