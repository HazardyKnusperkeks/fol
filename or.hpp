/**
 * @file
 * @brief Contains the or.
 */

#ifndef FOL_OR_HPP
#define FOL_OR_HPP

#include "helper.hpp"
#include "traits.hpp"

#include <ostream>
#include <tuple>

namespace fol {

template<typename... Ts>
struct Or {
	static_assert((IsFormula<Ts>::value && ...), "All parameters have to be formulas!");
	
	std::tuple<Ts...> ts;
	
	friend std::ostream& operator<<(std::ostream& os, const Or& a) {
		return details::print(os, a.ts, " | ");
	}
};

template<typename... T1s, typename... T2s>
constexpr bool operator==(const Or<T1s...>& a1, const Or<T2s...>& a2) noexcept {
	return sizeof...(T1s) == sizeof...(T2s) && a1.ts == a2.ts;
}

template<typename... T1s, typename... T2s>
constexpr bool operator!=(const Or<T1s...>& a1, const Or<T2s...>& a2) noexcept {
	return !(a1 == a2);
}

} //namespace fol

#endif
