
#include "Event.h"
namespace Mcc
{

	template<typename T>
	void NetworkEventManager::HandlerMapper::operator()()
	{
		constexpr size_t index = PacketList::IndexOf<T>;

		mEventManager.mHandlers[index] = [](const ENetPeer* peer, const ENetPacket* packet, NetworkEventManager& eventManager, cereal::BinaryInputArchive&& archive)
		{
			T data {};
			try
			{
				archive(data);
			}
			catch (std::exception& exc)
			{
				eventManager.Dispatch<MalformedPacketEvent>({ packet });
			}

			eventManager.Dispatch<T>(data);
			eventManager.Dispatch<From<T>>({ peer, std::move(data)});
		};
	}

}