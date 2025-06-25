//
// Created by Alex on 28/08/2024.
//

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
	struct Event : public BaseEvent<Tag>
	{
			using typename BaseEvent<Tag>::TypeId;
			static TypeId GetType();

			T event;
	};

}

#include <Common/Utils/Event.inl>

#endif