/**
 * @file
 * @brief Contains the forall quantifier.
 */

#ifndef FOL_FORALL_HPP
#define FOL_FORALL_HPP

#include "pretty_printer.hpp"
#include "traits.hpp"

#include <ostream>

namespace fol {

template<typename Var, typename Form>
struct ForAll {
	static_assert(IsVariable<Var>::value, "The first parameter has to be a variable!");
	static_assert(IsFormula<Form>::value, "The secnd parameter has to be a formula!");
	Var v;
	Form f;
	
	friend std::ostream& operator<<(std::ostream& os, const ForAll& f) {
		return os<<'A'<<f.v<<": "<<f.f;
	}
};

template<typename Var, typename Form>
ForAll(Var, Form) -> ForAll<Var, Form>;

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator==(const ForAll<Var1, Form1>& f1, const ForAll<Var2, Form2>& f2) noexcept {
	return f1.v == f2.v && f1.f == f2.f;
}

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator!=(const ForAll<Var1, Form1>& f1, const ForAll<Var2, Form2>& f2) noexcept {
	return !(f1 == f2);
}

template<typename Var, typename Form>
struct PrettyPrinter<ForAll<Var, Form>> {
	const ForAll<Var, Form>& FA;
	const int Index;
	
	PrettyPrinter(const ForAll<Var, Form>& fa, const int index = 0) : FA(fa), Index(index) {
		return;
	}
	
	std::ostream& prettyPrint(std::ostream& os) const {
		const bool withParanthesis = !IsQuantifier<Form>::value;
		os<<'A'<<FA.v;
		if ( withParanthesis ) {
			os<<": "<<PrettyParanthesis[static_cast<std::size_t>(Index)].first;
		} //if ( withParanthesis )
		os<<PrettyPrinter<Form>{FA.f, withParanthesis ? (Index + 1) % static_cast<int>(PrettyParanthesis.size()) : Index};
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].second;
		} //if ( withParanthesis )
		return os;
	}
};

} //namespace fol

#endif
