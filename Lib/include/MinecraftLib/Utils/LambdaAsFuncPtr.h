//
// Created by Alex on 12/09/2024.
//

#ifndef MINECRAFT_LAMBDAASFUNCPTR_H
#define MINECRAFT_LAMBDAASFUNCPTR_H

#include <utility>

namespace Mcc
{

	template <typename F, int I, typename L, typename R, typename ...A>
	inline F LambdaAsFuncPtr(L&& l, R (*)(A...) noexcept(noexcept(std::declval<F>()(std::declval<A>()...))))
	{
		static thread_local L l_(::std::forward<L>(l));
		static thread_local bool full;

		if (full)
		{
			l_.~L();

			new (static_cast<void*>(&l_)) L(std::forward<L>(l));
		}
		else
		{
			full = true;
		}

		struct S
		{
				static R f(A... args) noexcept(noexcept(std::declval<F>()(std::forward<A>(args)...)))
				{
					return l_(std::forward<A>(args)...);
				}
		};

		return &S::f;
	}

	template <typename F, int I = 0, typename L>
	inline F LambdaAsFuncPtr(L&& l)
	{
		return LambdaAsFuncPtr<F, I>(std::forward<L>(l), F());
	}

}

#endif
