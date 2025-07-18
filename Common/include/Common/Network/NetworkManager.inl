#include "Common/Utils/Logging.h"

namespace Mcc
{

	template<typename T>
	int NetworkManager::Send(ENetPeer* peer, T data, enet_uint32 flag, enet_uint8 channel)
	{
		PacketOutputStream stream;
		cereal::BinaryOutputArchive archive(stream);

		size_t type = PacketList::IndexOf<T>;
		archive(type);
		archive(data);

        if (const auto packet = CreatePacket(stream.GetBuffer(), flag))
        {
            return enet_peer_send(peer, channel, packet);
        }

        return EXIT_FAILURE;
    }

}