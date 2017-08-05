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
	
	friend std::ostream& operator<<(std::ostream& os, const Equivalent& i) {
		return os<<i.t1<<" -> "<<i.t2;
	}
};

template<typename T11, typename T12, typename T21, typename T22>
constexpr bool operator==(const Equivalent<T11, T12>& i1, const Equivalent<T21, T22>& i2) noexcept {
	return i1.t1 == i2.t1 && i1.t2 == i2.t2;
}

template<typename T11, typename T12, typename T21, typename T22>
constexpr bool operator!=(const Equivalent<T11, T12>& i1, const Equivalent<T21, T22>& i2) noexcept {
	return !(t1 == t2);
}

} //namespace fol

#endif
