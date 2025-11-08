// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

namespace Mcc
{

    template<typename Tag>
    BaseEvent<Tag>::TypeId BaseEvent<Tag>::sTypeCount = 0;

    template<typename Tag>
    auto BaseEvent<Tag>::GetNextType() -> TypeId
    {
        return sTypeCount++;
    }

    template<typename Tag, typename T>
    auto Event<Tag, T>::GetType() -> TypeId
    {
        static unsigned short type = BaseEvent<Tag>::GetNextType();
        return type;
    }

}