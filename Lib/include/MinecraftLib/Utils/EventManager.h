//
// Created by Alex on 28/08/2024.
//

#pragma once

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
			using EventHandler 		  = std::function<void(const T&)>;
			using GenericEventHandler = std::function<void(const BaseEvent<Tag>&)>;
			using HandlerId 			  = unsigned int;

			template<typename T>
			void Dispatch(T event);

			template<typename T>
			HandlerId Subscribe(EventHandler<T> handler);

			void UnSubscribe(HandlerId id);

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

#include <MinecraftLib/Utils/EventManager.inl>