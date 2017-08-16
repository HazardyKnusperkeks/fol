/**
 * @file
 * @brief Contains the equivalence.
 */

#ifndef FOL_EQUIVALENT_HPP
#define FOL_EQUIVALENT_HPP

#include "and.hpp"
#include "implies.hpp"
#include "pretty_printer.hpp"
#include "traits.hpp"

#include <ostream>

namespace fol {

template<typename T1, typename T2>
struct Equivalent {
	static_assert(IsFormula<T1>::value && IsFormula<T2>::value, "Both parameters have to be formulas!");
	T1 t1;
	T2 t2;
	
	constexpr auto simplified(void) const {
		auto leftImplies = Implies{t1, t2}.simplified();
		auto rightImplies = Implies{t2, t1}.simplified();
		return And{leftImplies, rightImplies};
	}
	
	constexpr auto negate(void) const {
		return simplified().negate();
	}
	
	constexpr auto toNegationNormalForm(void) const {
		return simplified().toNegationNormalForm();
	}
	
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

template<typename T1, typename T2>
struct PrettyPrinter<Equivalent<T1, T2>> {
	const Equivalent<T1, T2>& E;
	const int Index;
	
	PrettyPrinter(const Equivalent<T1, T2>& e, int index = -1) : E(e), Index(index) {
		return;
	}
	
	std::ostream& prettyPrint(std::ostream& os) const {
		const bool withParanthesis = Index != -1;
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].first;
		} //if ( withParanthesis )
		const auto nextIndex = (Index + 1) % static_cast<int>(PrettyParanthesis.size());
		os<<PrettyPrinter<T1>{E.t1, nextIndex}<<" <-> "<<PrettyPrinter<T2>{E.t2, nextIndex};
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].second;
		} //if ( withParanthesis )
		return os;
	}
};

} //namespace fol

#endif
