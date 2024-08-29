//
// Created by Alex on 28/08/2024.
//

#include <MinecraftLib/Utils/Event.h>

#include <cassert>
#include <fmt/base.h>

namespace Mcc
{

	template<typename Tag>
	template<typename T>
	void EventManager<Tag>::Dispatch(T event)
	{
		Event<Tag, T> wrapper { {}, std::move(event) };
		for (auto& handler: mEventHandlers[decltype(wrapper)::GetType()])
		{
			if (handler)
			{
				handler(wrapper);
			}
		}
	}

	template<typename Tag>
	template<typename T>
	auto EventManager<Tag>::Subscribe(EventHandler<T> handler) -> HandlerId
	{
		auto typeId = Event<Tag, T>::GetType();
		assert(typeId <= mEventHandlers.size());

		if (mEventHandlers.size() <= typeId)
		{
			mEventHandlers.push_back({});
		}
		mEventHandlers[typeId].push_back(HandlerFunctor<T> { std::move(handler) });

		return (typeId << 16) + (mEventHandlers[typeId].size() - 1);
	}

	template<typename Tag>
	void EventManager<Tag>::Withdraw(EventManager::HandlerId id)
	{
		auto typeId    = id >> 16 & 0xf;
		auto handlerId = id & 0x0f;

		if (typeId < mEventHandlers.size())
		{
			if (handlerId < mEventHandlers[typeId].size())
			{
				mEventHandlers[typeId][handlerId] = {};
			}
		}
	}

	template<typename Tag>
	template<typename T>
	void EventManager<Tag>::HandlerFunctor<T>::operator()(const BaseEvent<Tag>& event)
	{
		handler(static_cast<const Event<Tag, T>&>(event).event);
	}

}