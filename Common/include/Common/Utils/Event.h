// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_UTILS_EVENT_H
#define MCC_COMMON_UTILS_EVENT_H

#include "Common/Export.h"

#include <string>

namespace Mcc
{

    using EventTypeID = unsigned short;

    template<typename Tag>
    struct BaseEvent {};

    template<typename Tag, typename T>
    struct Event : BaseEvent<Tag>
    {
        static EventTypeID GetType();

        T event;
    };

    namespace _
    {

        MCC_LIB_API EventTypeID GetTypeID(const std::string& tag, const std::string& type);

    }

}

#include <Common/Utils/Event.inl>

#endif