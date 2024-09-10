//
// Created by Alex on 09/09/2024.
//

#ifndef MINECRAFT_TYPELIST_H
#define MINECRAFT_TYPELIST_H

#include <tuple>
#include <functional>

namespace Mcc
{

	template<typename... Types>
	struct TypeList
	{
		private:
			using Self = std::tuple<Types...>;

			template<size_t Index, typename ToFind, typename Current, typename... Rest>
			static constexpr size_t GetIndex();

			template<typename C, typename T, typename... Rest>
			static constexpr void ApplyForEachType(C&& cb);

		public:
			static constexpr size_t Count = sizeof...(Types);

			template<typename T>
			static constexpr size_t IndexOf = GetIndex<0, T, Types...>();

			template<size_t Index>
			using TypeAt = std::tuple_element<Index, Self>;

			template<typename C>
			static constexpr void ForEachType(C&& cb);
	};


}

#include <MinecraftLib/Utils/TypeList.inl>

#endif
