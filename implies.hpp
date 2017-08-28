/**
 * @file
 * @brief Contains the implication.
 */

#ifndef FOL_IMPLIES_HPP
#define FOL_IMPLIES_HPP

#include "array_set.hpp"
#include "not.hpp"
#include "or.hpp"
#include "pretty_printer.hpp"
#include "traits.hpp"

#include <ostream>

namespace fol {

template<typename T1, typename T2>
struct Implies {
	static_assert(IsFormula<T1>::value && IsFormula<T2>::value, "Both parameters have to be formulas!");
	T1 t1;
	T2 t2;
	
	using VariableCount = std::integral_constant<std::size_t, T1::VariableCount::value + T2::VariableCount::value>;
	using VariableArray = decltype(ArraySet<0>{} + typename T1::VariableArray{} + typename T1::VariableArray{});
	
	constexpr auto simplified(void) const {
		return Or<Not<T1>, T2>{{t1}, t2}.simplified();
	}
	
	constexpr auto negate(void) const {
		return simplified().negate();
	}
	
	constexpr auto toNegationNormalForm(void) const {
		return simplified().toNegationNormalForm();
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Implies& i) {
		return os<<i.t1<<" -> "<<i.t2;
	}
};

template<typename T1, typename T2>
Implies(T1, T2) -> Implies<T1, T2>;

template<typename T11, typename T12, typename T21, typename T22>
constexpr bool operator==(const Implies<T11, T12>& i1, const Implies<T21, T22>& i2) noexcept {
	return i1.t1 == i2.t1 && i1.t2 == i2.t2;
}

template<typename T11, typename T12, typename T21, typename T22>
constexpr bool operator!=(const Implies<T11, T12>& i1, const Implies<T21, T22>& i2) noexcept {
	return !(i1 == i2);
}

template<typename T1, typename T2>
struct PrettyPrinter<Implies<T1, T2>> {
	const Implies<T1, T2>& I;
	const int Index;
	
	PrettyPrinter(const Implies<T1, T2>& i, int index = -1) : I(i), Index(index) {
		return;
	}
	
	std::ostream& prettyPrint(std::ostream& os) const {
		const bool withParanthesis = Index != -1;
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].first;
		} //if ( withParanthesis )
		const auto nextIndex = (Index + 1) % static_cast<int>(PrettyParanthesis.size());
		os<<PrettyPrinter<T1>{I.t1, nextIndex}<<" -> "<<PrettyPrinter<T2>{I.t2, nextIndex};
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].second;
		} //if ( withParanthesis )
		return os;
	}
};

} //namespace fol

#endif
