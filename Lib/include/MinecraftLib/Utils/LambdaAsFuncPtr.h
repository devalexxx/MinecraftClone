//
// Created by Alex on 12/09/2024.
//

#ifndef MINECRAFT_LAMBDAASFUNCPTR_H
#define MINECRAFT_LAMBDAASFUNCPTR_H

#include <utility>

namespace Mcc
{

	template<typename F, int I, typename L, typename R, typename... A>
	inline F LambdaAsFuncPtr(L&& l, R (*)(A...) noexcept(noexcept(std::declval<F>()(std::declval<A>()...))))
	{
		// Fonction statique à usage local pour contenir l'objet lambda
		static thread_local typename std::decay<L>::type l_(std::forward<L>(l));

		// Structure contenant la fonction statique
		struct S
		{
				static R f(A... args) noexcept(noexcept(std::declval<F>()(std::declval<A>()...)))
				{
					return l_(std::forward<A>(args)...);
				}
		};

		return &S::f;
	}

	template<typename F, int I = 0, typename L>
	inline F LambdaAsFuncPtr(L&& l)
	{
		return LambdaAsFuncPtr<F, I>(std::forward<L>(l), F());
	}

}

#endif
