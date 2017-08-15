/**
 * @file
 * @brief Contains the and.
 */

#ifndef FOL_AND_HPP
#define FOL_AND_HPP

#include "helper.hpp"
#include "pretty_printer.hpp"
#include "traits.hpp"

#include <ostream>
#include <tuple>

namespace fol {

template<typename... Ts>
struct And {
	static_assert((IsFormula<Ts>::value && ...), "All parameters have to be formulas!");
	
	std::tuple<Ts...> ts;
	
	constexpr And(void) = default;
	constexpr And(Ts... t) noexcept((std::is_nothrow_move_constructible_v<Ts> && ...) &&
	                                noexcept(std::make_tuple(std::move(t)...))) : ts(std::make_tuple(std::move(t)...)) {
		return;
	}
	
	friend std::ostream& operator<<(std::ostream& os, const And& a) {
		return details::print(os, a.ts, " & ");
	}
};

template<typename... T1s, typename... T2s>
constexpr bool operator==(const And<T1s...>& a1, const And<T2s...>& a2) noexcept {
	return sizeof...(T1s) == sizeof...(T2s) && a1.ts == a2.ts;
}

template<typename... T1s, typename... T2s>
constexpr bool operator!=(const And<T1s...>& a1, const And<T2s...>& a2) noexcept {
	return !(a1 == a2);
}

template<typename... Ts>
struct PrettyPrinter<And<Ts...>> {
	const And<Ts...>& A;
	const int Index;
	
	PrettyPrinter(const And<Ts...>& a, int index = -1) : A(a), Index(index) {
		return;
	}
	
	std::ostream& prettyPrint(std::ostream& os) const {
		const bool withParanthesis = Index != -1;
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].first;
		} //if ( withParanthesis )
		const auto nextIndex = (Index + 1) % static_cast<int>(PrettyParanthesis.size());
		details::prettyPrint(os, A.ts, nextIndex, " & ");
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].second;
		} //if ( withParanthesis )
		return os;
	}
};

} //namespace fol

#endif
