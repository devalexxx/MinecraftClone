// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Hexis/Core/TypeName.h"

namespace Mcc
{

    template<typename Tag, typename T>
    EventTypeID Event<Tag, T>::GetType()
    {
        return _::GetTypeID(Hx::TypeName<Tag>(), Hx::TypeName<T>());
    }

}