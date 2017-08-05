/**
 * @file
 * @brief Contains the function class.
 */

#ifndef FOL_FUNCTION_HPP
#define FOL_FUNCTION_HPP

#include "helper.hpp"
#include "name.hpp"
#include "traits.hpp"

#include <ostream>
#include <tuple>
#include <type_traits>

namespace fol {

template<typename NameT, typename... Args>
struct Function {
	static_assert(IsName<NameT>::value, "First template argument must be a name!");
	static_assert((IsTerm<Args>::value && ...), "All template arguments from the second on have to be terms!");
	
	NameT N;
	std::tuple<Args...> A;
	
	constexpr Function(void) = default;
	
	constexpr Function(NameT n, std::tuple<Args...> t = {})
			noexcept(std::is_nothrow_move_constructible_v<NameT> &&
			         std::is_nothrow_move_constructible_v<std::tuple<Args...>>) : N(std::move(n)), A(std::move(t)) {
		return;
	}
	
	constexpr auto prev(void) const
			noexcept(noexcept(Function::fromName(std::declval<NameT>(), std::declval<std::tuple<Args...>>()))) {
		return fromName(N.prev(), A);
	}
	
	constexpr auto next(void) const
			noexcept(noexcept(Function::fromName(std::declval<NameT>(), std::declval<std::tuple<Args...>>()))) {
		return fromName(N.next(), A);
	}
	
	template<typename Name2>
	static constexpr Function<Name2, Args...> fromName(const Name2& n, const std::tuple<Args...>& t)
			noexcept(std::is_nothrow_constructible_v<Function<Name2, Args...>>) {
		return {n, t};
	}
	
	template<typename Term, std::enable_if_t<IsTerm<Term>::value>* = nullptr>
	constexpr auto append(Term t) const {
		return Function<NameT, Args..., Term>{N, std::tuple_cat(A, std::tuple{std::move(t)})};
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Function& f) {
		os<<f.N;
		if constexpr ( sizeof...(Args) >= 1 ) {
			os<<'('<<f.A<<')';
		} //if constexpr ( sizeof...(Args) >= 1 )
		return os;
	}
};

template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
constexpr bool operator==(const Function<Name1T, Args1...> f1, const Function<Name2T, Args2...> f2) noexcept {
	return sizeof...(Args1) == sizeof...(Args2) && f1.N == f2.N && f1.A == f2.A;
}

template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
constexpr bool operator!=(const Function<Name1T, Args1...> f1, const Function<Name2T, Args2...> f2) noexcept {
	return !(f1 == f2);
}

}

#endif
