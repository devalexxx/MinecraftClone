// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_UTILS_EVENT_H
#define MCC_COMMON_UTILS_EVENT_H

namespace Mcc
{

    template<typename Tag>
    struct BaseEvent
    {
        using TypeId = unsigned short;
        static TypeId sTypeCount;
        static TypeId GetNextType();
    };

    template<typename Tag, typename T>
    struct Event : BaseEvent<Tag>
    {
        using typename BaseEvent<Tag>::TypeId;
        static TypeId GetType();

        T event;
    };

}

#include <Common/Utils/Event.inl>

#endif