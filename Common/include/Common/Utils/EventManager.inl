// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Common/Utils/Assert.h"
#include "Common/Utils/Event.h"

#include <fmt/base.h>

namespace Mcc
{

    template<typename Tag>
    template<typename T>
    void EventManager<Tag>::Dispatch(T event)
    {
        Event<Tag, T> wrapper { {}, std::move(event) };

        auto type = decltype(wrapper)::GetType();
        MCC_ASSERT(type <= mEventHandlers.size(), "Trying to dispatch an unregistered event");

        if (mEventHandlers.size() > type)
        {
            for (auto& handler: mEventHandlers[type])
            {
                if (handler)
                {
                    handler(wrapper);
                }
            }
        }
        else
        {
            mEventHandlers.push_back({});
        }
    }

    template<typename Tag>
    template<typename T>
    auto EventManager<Tag>::Subscribe(EventHandler<T> handler) -> HandlerId
    {
        auto typeId = Event<Tag, T>::GetType();
        MCC_ASSERT(typeId <= mEventHandlers.size(), "Trying to subscribe to an unregistered event");

        if (mEventHandlers.size() <= typeId)
        {
            mEventHandlers.push_back({});
        }
        mEventHandlers[typeId].push_back(HandlerFunctor<T> { std::move(handler) });

        return (typeId << 16) + (mEventHandlers[typeId].size() - 1);
    }

    template<typename Tag>
    void EventManager<Tag>::Withdraw(const HandlerId id)
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