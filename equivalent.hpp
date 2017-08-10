/**
 * @file
 * @brief Contains the equivalence.
 */

#ifndef FOL_EQUIVALENT_HPP
#define FOL_EQUIVALENT_HPP

#include "traits.hpp"

#include <ostream>

namespace fol {

template<typename T1, typename T2>
struct Equivalent {
	static_assert(IsFormula<T1>::value && IsFormula<T2>::value, "Both parameters have to be formulas!");
	T1 t1;
	T2 t2;
	
	friend std::ostream& operator<<(std::ostream& os, const Equivalent& e) {
		return os<<e.t1<<" -> "<<e.t2;
	}
};

template<typename T1, typename T2>
Equivalent(T1, T2) -> Equivalent<T1, T2>;

template<typename T11, typename T12, typename T21, typename T22>
constexpr bool operator==(const Equivalent<T11, T12>& e1, const Equivalent<T21, T22>& e2) noexcept {
	return e1.t1 == e2.t1 && e1.t2 == e2.t2;
}

template<typename T11, typename T12, typename T21, typename T22>
constexpr bool operator!=(const Equivalent<T11, T12>& e1, const Equivalent<T21, T22>& e2) noexcept {
	return !(e1 == e2);
}

} //namespace fol

#endif
