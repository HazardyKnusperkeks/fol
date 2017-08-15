/**
 * @file
 * @brief Contains the exists quantifier.
 */

#ifndef FOL_EXISTS_HPP
#define FOL_EXISTS_HPP

#include "pretty_printer.hpp"
#include "traits.hpp"

#include <ostream>

namespace fol {

template<typename Var, typename Form>
struct Exists {
	static_assert(IsVariable<Var>::value, "The first parameter has to be a variable!");
	static_assert(IsFormula<Form>::value, "The secnd parameter has to be a formula!");
	Var v;
	Form f;
	
	friend std::ostream& operator<<(std::ostream& os, const Exists& e) {
		return os<<'E'<<e.v<<": "<<e.f;
	}
};

template<typename Var, typename Form>
Exists(Var, Form) -> Exists<Var, Form>;

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator==(const Exists<Var1, Form1>& e1, const Exists<Var2, Form2>& e2) noexcept {
	return e1.v == e2.v && e1.f == e2.f;
}

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator!=(const Exists<Var1, Form1>& e1, const Exists<Var2, Form2>& e2) noexcept {
	return !(e1 == e2);
}

template<typename Var, typename Form>
struct PrettyPrinter<Exists<Var, Form>> {
	const Exists<Var, Form>& E;
	const int Index;
	
	PrettyPrinter(const Exists<Var, Form>& e, const int index = 0) : E(e), Index(index) {
		return;
	}
	
	std::ostream& prettyPrint(std::ostream& os) const {
		const bool withParanthesis = !IsQuantifier<Form>::value;
		os<<'E'<<E.v;
		if ( withParanthesis ) {
			os<<": "<<PrettyParanthesis[static_cast<std::size_t>(Index)].first;
		} //if ( withParanthesis )
		os<<PrettyPrinter<Form>{E.f, withParanthesis ? (Index + 1) % static_cast<int>(PrettyParanthesis.size()) : Index};
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].second;
		} //if ( withParanthesis )
		return os;
	}
};

} //namespace fol

#endif
