/**
 * @file
 * @brief Contains the definition of predicates.
 */

#ifndef FOL_PREDICATE_HPP
#define FOL_PREDICATE_HPP

#include "name.hpp"
#include "not.hpp"
#include "pretty_printer.hpp"
#include "traits.hpp"

#include <ostream>
#include <tuple>

namespace fol {

template<typename NameT, typename... Args>
struct Predicate {
	private:
	template<typename Name2, std::size_t... Idx>
	static constexpr Predicate<Name2, Args...> fromNameImpl(const Name2& n, const std::tuple<Args...>& t,
	                                                        std::index_sequence<Idx...>)
			noexcept(std::is_nothrow_constructible_v<Predicate<Name2, Args...>, const Name2&, const Args&...>) {
		return {n, std::get<Idx>(t)...};
	}
	
	public:
	static_assert(IsName<NameT>::value, "First template argument must be a name!");
	static_assert((IsTerm<Args>::value && ...), "All template arguments from the second on have to be terms!");
	
	using VariableCount = std::integral_constant<std::size_t, (0 + ... + Args::VariableCount::value)>;
	
	NameT N;
	std::tuple<Args...> A;
	
	using Arity = std::integral_constant<std::size_t, sizeof...(Args)>;
	
	constexpr Predicate(void) = default;
	
	constexpr Predicate(NameT n, Args... a)
			noexcept(std::is_nothrow_move_constructible_v<NameT> &&
			         (std::is_nothrow_move_constructible_v<Args> && ...) &&
			         noexcept(std::make_tuple(std::move(a)...))) : N(std::move(n)),
			A(std::make_tuple(std::move(a)...)) {
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
	
	constexpr auto simplified(void) const { return *this; }
	
	constexpr auto negate(void) const { return Not{*this}; }
	
	constexpr auto toNegationNormalForm(void) const { return *this; }
	
	template<typename Name2>
	static constexpr Predicate<Name2, Args...> fromName(const Name2& n, const std::tuple<Args...>& t)
			noexcept(std::is_nothrow_constructible_v<Predicate<Name2, Args...>, const Name2&, const Args&...>) {
		return fromNameImpl(n, t, std::index_sequence_for<Args...>());
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Predicate& p) {
		os<<p.N;
		if constexpr ( sizeof...(Args) >= 1 ) {
			os<<'('<<p.A<<')';
		} //if constexpr ( sizeof...(Args) >= 1 )
		return os;
	}
};

template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
constexpr bool operator==(const Predicate<Name1T, Args1...>& p1, const Predicate<Name2T, Args2...>& p2) noexcept {
	return sizeof...(Args1) == sizeof...(Args2) && p1.N == p2.N && p1.A == p2.A;
}

template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
constexpr bool operator!=(const Predicate<Name1T, Args1...>& p1, const Predicate<Name2T, Args2...>& p2) noexcept {
	return !(p1 == p2);
}

template<typename NameT, typename... Args>
struct PrettyPrinter<Predicate<NameT, Args...>> {
	const Predicate<NameT, Args...>& P;
	
	PrettyPrinter(const Predicate<NameT, Args...>& p, const int = -1) : P(p) {
		return;
	}
	
	std::ostream& prettyPrint(std::ostream& os) const {
		return os<<P;
	}
};

} //namespace fol

#endif
