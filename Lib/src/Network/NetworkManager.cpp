//
// Created by Alex on 11/09/2024.
//

#include "MinecraftLib/Utils/Logging.h"
#include <MinecraftLib/Network/Event.h>
#include <MinecraftLib/Network/NetworkManager.h>

namespace Mcc
{

	NetworkManager::NetworkManager(const char* host, unsigned short port) :
		mAddr(), mHost(nullptr)
	{
		if (enet_initialize())
		{
			MCC_LOG_ERROR("Failed to initialize Enet");
			exit(EXIT_FAILURE);
		}
		atexit(enet_deinitialize);

		enet_address_set_host(&mAddr, host);
		mAddr.port = port;
	}

	NetworkManager::~NetworkManager()
	{
		if (mHost)
			enet_host_destroy(mHost);
	}

	void NetworkManager::Poll()
	{
		ENetEvent event;
		while (enet_host_service(mHost, &event, 0) > 0)
		{
			switch (event.type)
			{
				case ENET_EVENT_TYPE_CONNECT: {
					Dispatch<ConnectEvent>({ event.peer });
					break;
				}
				case ENET_EVENT_TYPE_DISCONNECT: {
					Dispatch<DisconnectEvent>({ event.peer });
					break;
				}
				case ENET_EVENT_TYPE_RECEIVE: {
					DispatchPacket(event.peer, event.packet);
					enet_packet_destroy(event.packet);
					break;
				}
				default:
					break;
			}
		}
	}

	ENetPacket* NetworkManager::CreatePacket(ByteSpan&& data, enet_uint32 flag)
	{
		return enet_packet_create(data.data(), data.size(), flag);
	}

	int NetworkManager::CreateHost(const ENetAddress* addr, size_t peers, size_t channels, enet_uint32 in, enet_uint32 out)
	{
		mHost = enet_host_create(addr, peers, channels, in, out);
		if (!mHost)
		{
			MCC_LOG_ERROR("Failed to create ENet host");
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}

}