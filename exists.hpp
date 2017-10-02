/**
 * @file
 * @brief Contains the exists quantifier.
 */

#ifndef FOL_EXISTS_HPP
#define FOL_EXISTS_HPP

#include "forward.hpp"

#include "forall.hpp"
#include "pretty_printer.hpp"
#include "traits.hpp"

#include <ostream>

namespace fol {

template<typename Var, typename Form>
struct Exists {
	static_assert(IsVariable<Var>::value, "The first parameter has to be a variable!");
	static_assert(IsFormula<Form>::value, "The secnd parameter has to be a formula!");
	Var V;
	Form F;
	
	using VariableCount = std::integral_constant<std::size_t, 1 + Form::VariableCount::value>;
//	using VariableArray = decltype(ArraySet<1, Var>{} + typename Form::VariableArray{});
	
	constexpr auto simplified(void) const {
		auto form = F.simplified();
		return Exists<Var, std::decay_t<decltype(form)>>{V, form};
	}
	
	constexpr auto negate(void) const {
		return ForAll{V, Not<Form>{F}.toNegationNormalForm()};
	}
	
	constexpr auto toNegationNormalForm(void) const {
		auto form = F.toNegationNormalForm();
		return Exists<Var, std::decay_t<decltype(form)>>{V, form};
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Exists& e) {
		return os<<'E'<<e.V<<": "<<e.F;
	}
};

template<typename Var, typename Form>
Exists(Var, Form) -> Exists<Var, Form>;

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator==(const Exists<Var1, Form1>& e1, const Exists<Var2, Form2>& e2) noexcept {
	return e1.V == e2.V && e1.F == e2.F;
}

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator!=(const Exists<Var1, Form1>& e1, const Exists<Var2, Form2>& e2) noexcept {
	return !(e1 == e2);
}

template<typename Var, typename Form>
struct PrettyPrinter<Exists<Var, Form>> {
	const Exists<Var, Form>& E;
	const int Index;
	
	PrettyPrinter(const Exists<Var, Form>& e, const int index = 0) : E{e}, Index{index} {
		return;
	}
	
	std::ostream& prettyPrint(std::ostream& os) const {
		const bool withParanthesis = !IsQuantifier<Form>::value;
		os<<'E'<<E.V;
		if ( withParanthesis ) {
			os<<": "<<PrettyParanthesis[static_cast<std::size_t>(Index)].first;
		} //if ( withParanthesis )
		os<<PrettyPrinter<Form>{E.F, withParanthesis ? (Index + 1) % static_cast<int>(PrettyParanthesis.size()) : Index};
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].second;
		} //if ( withParanthesis )
		return os;
	}
};

} //namespace fol

#endif
