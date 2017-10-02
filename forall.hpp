/**
 * @file
 * @brief Contains the forall quantifier.
 */

#ifndef FOL_FORALL_HPP
#define FOL_FORALL_HPP

#include "forward.hpp"

#include "exists.hpp"
#include "pretty_printer.hpp"
#include "traits.hpp"

#include <ostream>

namespace fol {

template<typename Var, typename Form>
struct ForAll {
	static_assert(IsVariable<Var>::value, "The first parameter has to be a variable!");
	static_assert(IsFormula<Form>::value, "The secnd parameter has to be a formula!");
	Var V;
	Form F;
	
	using VariableCount = std::integral_constant<std::size_t, 1 + Form::VariableCount::value>;
//	using VariableArray = decltype(ArraySet<1, Var>{} + typename Form::VariableArray{});
	
	constexpr auto simplified(void) const {
		auto form = F.simplified();
		return ForAll<Var, std::decay_t<decltype(form)>>{V, form};
	}
	
	constexpr auto negate(void) const {
		return Exists{V, Not<Form>{F}.toNegationNormalForm()};
	}
	
	constexpr auto toNegationNormalForm(void) const {
		auto form = F.toNegationNormalForm();
		return ForAll<Var, std::decay_t<decltype(form)>>{V, form};
	}
	
	friend std::ostream& operator<<(std::ostream& os, const ForAll& f) {
		return os<<'A'<<f.V<<": "<<f.F;
	}
};

template<typename Var, typename Form>
ForAll(Var, Form) -> ForAll<Var, Form>;

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator==(const ForAll<Var1, Form1>& f1, const ForAll<Var2, Form2>& f2) noexcept {
	return f1.V == f2.V && f1.F == f2.F;
}

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator!=(const ForAll<Var1, Form1>& f1, const ForAll<Var2, Form2>& f2) noexcept {
	return !(f1 == f2);
}

template<typename Var, typename Form>
struct PrettyPrinter<ForAll<Var, Form>> {
	const ForAll<Var, Form>& FA;
	const int Index;
	
	PrettyPrinter(const ForAll<Var, Form>& fa, const int index = 0) : FA{fa}, Index{index} {
		return;
	}
	
	std::ostream& prettyPrint(std::ostream& os) const {
		const bool withParanthesis = !IsQuantifier<Form>::value;
		os<<'A'<<FA.V;
		if ( withParanthesis ) {
			os<<": "<<PrettyParanthesis[static_cast<std::size_t>(Index)].first;
		} //if ( withParanthesis )
		os<<PrettyPrinter<Form>{FA.F, withParanthesis ? (Index + 1) % static_cast<int>(PrettyParanthesis.size()) : Index};
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].second;
		} //if ( withParanthesis )
		return os;
	}
};

} //namespace fol

#endif
