
namespace Mcc
{

	template<typename T>
	int ServerNetworkManager::Send(ENetPeer* peer, T data, enet_uint32 flag, enet_uint8 channel)
	{
		return NetworkManager::Send(peer, std::move(data), flag, channel);
	}

	template<typename T>
	void ServerNetworkManager::Broadcast(T data, enet_uint32 flag, enet_uint8 channel)
	{
		PacketOutputStream stream;
		cereal::BinaryOutputArchive archive(stream);

		size_t type = PacketList::IndexOf<T>;
		archive(type);
		archive(data);

		auto packet = CreatePacket(stream.GetBuffer(), flag);
		if (packet)
		{
			enet_host_broadcast(mHost, channel, packet);
		}
	}

}