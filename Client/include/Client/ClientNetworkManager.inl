
namespace Mcc
{

	template<typename T>
	int ClientNetworkManager::Send(T data, enet_uint32 flag, enet_uint8 channel)
	{
		return NetworkManager::Send<T>(mPeer, std::move(data), flag, channel);
	}

}