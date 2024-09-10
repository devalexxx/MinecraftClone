
namespace Mcc
{

	template<typename... Types>
	template<size_t Index, typename ToFind, typename Current, typename... Rest>
	constexpr size_t TypeList<Types...>::GetIndex()
	{
		if constexpr (std::is_same_v<ToFind, Current>)
			return Index;
		else
			return GetIndex<Index + 1, ToFind, Rest...>();
	}

	template<typename... Types>
	template<typename C, typename T, typename... Rest>
	constexpr void TypeList<Types...>::ApplyForEachType(C&& cb)
	{
		cb.template operator()<T>();
		if constexpr (sizeof...(Rest) > 0)
			ApplyForEachType<C, Rest...>(std::forward<C>(cb));
	}

	template<typename... Types>
	template<typename T>
	constexpr void TypeList<Types...>::ForEachType(T&& cb)
	{
		ApplyForEachType<T, Types...>(std::forward<T>(cb));
	}

}