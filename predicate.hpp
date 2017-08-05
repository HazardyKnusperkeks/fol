/**
 * @file
 * @brief Contains the definition of predicates.
 */

#ifndef FOL_PREDICATE_HPP
#define FOL_PREDICATE_HPP

#include "name.hpp"
#include "traits.hpp"

#include <ostream>
#include <tuple>

namespace fol {

template<typename NameT, typename... Args>
struct Predicate {
	static_assert(IsName<NameT>::value, "First template argument must be a name!");
	static_assert((IsTerm<Args>::value && ...), "All template arguments from the second on have to be terms!");
	
	NameT N;
	std::tuple<Args...> A;
	
	using Arity = std::integral_constant<std::size_t, sizeof...(Args)>;
	
	constexpr Predicate(void) = default;
	
	constexpr Predicate(NameT n, std::tuple<Args...> t = {})
			noexcept(std::is_nothrow_move_constructible_v<NameT> &&
			         std::is_nothrow_move_constructible_v<std::tuple<Args...>>) : N(std::move(n)), A(std::move(t)) {
		return;
	}
	
	constexpr auto prev(void) const
			noexcept(noexcept(Predicate::fromName(std::declval<NameT>(), std::declval<std::tuple<Args...>>()))) {
		return fromName(N.prev(), A);
	}
	
	constexpr auto next(void) const
			noexcept(noexcept(Predicate::fromName(std::declval<NameT>(), std::declval<std::tuple<Args...>>()))) {
		return fromName(N.next(), A);
	}
	
	template<typename Name2>
	static constexpr Predicate<Name2, Args...> fromName(const Name2& n, const std::tuple<Args...>& t)
			noexcept(std::is_nothrow_constructible_v<Predicate<Name2, Args...>>) {
		return {n, t};
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Predicate& p) {
		os<<p.N;
		if constexpr ( sizeof...(Args) >= 1 ) {
			os<<'('<<p.A<<')';
		} //if constexpr ( sizeof...(Args) >= 1 )
		return os;
	}
	
	template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
	friend constexpr bool operator==(const Predicate<Name1T, Args1...> p1, const Predicate<Name2T, Args2...> p2) noexcept;
	
	template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
	friend constexpr bool operator!=(const Predicate<Name1T, Args1...> p1, const Predicate<Name2T, Args2...> p2) noexcept;
};

template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
constexpr bool operator==(const Predicate<Name1T, Args1...> p1, const Predicate<Name2T, Args2...> p2) noexcept {
	return p1.N == p2.N && p1.A == p2.A;
}

template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
constexpr bool operator!=(const Predicate<Name1T, Args1...> p1, const Predicate<Name2T, Args2...> p2) noexcept {
	return !(p1 == p2);
}

} //namespace fol

#endif
