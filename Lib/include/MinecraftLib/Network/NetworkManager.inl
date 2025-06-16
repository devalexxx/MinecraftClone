
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

		auto packet = CreatePacket(stream.GetBuffer(), flag);
		if (packet)
		{
			return enet_peer_send(peer, channel, packet);
		}
		else
		{
			return EXIT_FAILURE;
		}
	}

}