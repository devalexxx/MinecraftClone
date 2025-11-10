// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Common/Utils/Event.h"

#include <unordered_map>

namespace Mcc::_
{

    EventTypeID GetTypeID(const std::string& tag, const std::string& type)
    {
        static std::unordered_map<std::string, EventTypeID> tagStore {};
        static std::unordered_map<std::string, EventTypeID> typeStore {};

        const auto merged = tag + type;
        if (typeStore.contains(merged))
        {
            return typeStore[merged];
        }

        if (tagStore.contains(tag))
        {
            return typeStore[merged] = ++tagStore[tag];
        }

        tagStore[tag]     = 0;
        typeStore[merged] = 0;
        return 0;
    }

}
