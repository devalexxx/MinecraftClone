// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_UTILS_EVENT_MANAGER_H
#define MCC_COMMON_UTILS_EVENT_MANAGER_H

#include <functional>
#include <vector>

namespace Mcc
{

    template<typename Tag>
    struct BaseEvent;

    template<typename Tag>
    class EventManager
    {
      public:
        template<typename T>
        using EventHandler        = std::function<void(const T&)>;
        using GenericEventHandler = std::function<void(const BaseEvent<Tag>&)>;
        using HandlerId           = unsigned int;

        template<typename T>
        void Dispatch(T event);

        template<typename T>
        HandlerId Subscribe(EventHandler<T> handler);

        void Withdraw(EventManager::HandlerId id);

      protected:
        std::vector<std::vector<GenericEventHandler>> mEventHandlers;

      private:
        template<typename T>
        struct HandlerFunctor
        {
            EventHandler<T> handler;

            void operator()(const BaseEvent<Tag>& event);
        };
    };

}

#include <Common/Utils/EventManager.inl>

#endif