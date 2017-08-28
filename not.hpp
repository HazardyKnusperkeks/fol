/**
 * @file
 * @brief Contains the negation class.
 */

#ifndef FOL_NOT_HPP
#define FOL_NOT_HPP

#include "array_set.hpp"
#include "pretty_printer.hpp"
#include "traits.hpp"

#include <ostream>

namespace fol {

template<typename T>
struct Not {
	static_assert(IsFormula<T>::value, "The negation must contain a formula!");
	T t;
	
	using VariableCount = typename T::VariableCount;
	using VariableArray = typename T::VariableArray;
	
	constexpr auto simplified(void) const {
		if constexpr ( IsNot<T>::value ) {
			return t.t.simplified();
		} //if constexpr ( IsNot<T>::value )
		else {
			auto inner = t.simplified();
			return Not<std::decay_t<decltype(inner)>>{inner};
		} //else -> if constexpr ( IsNot<T>::value )
	}
	
	constexpr auto negate(void) const {
		return t.toNegationNormalForm();
	}
	
	constexpr auto toNegationNormalForm(void) const {
		return t.negate();
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Not& n) {
		return os<<'-'<<n.t;
	}
};

template<typename T>
Not(T) -> Not<T>;

template<typename T1, typename T2>
constexpr bool operator==(const Not<T1>& n1, const Not<T2>& n2) noexcept {
	return n1.t == n2.t;
}

template<typename T1, typename T2>
constexpr bool operator!=(const Not<T1>& n1, const Not<T2>& n2) noexcept {
	return !(n1 == n2);
}

template<typename T>
struct PrettyPrinter<Not<T>> {
	const Not<T>& N;
	const int Index;
	
	PrettyPrinter(const Not<T>& n, int index = -1) : N(n), Index(index) {
		return;
	}
	
	std::ostream& prettyPrint(std::ostream& os) const {
		os<<'-'<<PrettyPrinter<T>{N.t, std::max(0, Index)};
		return os;
	}
};

} //namespace fol

#endif
