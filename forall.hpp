/**
 * @file
 * @brief Contains the forall quantifier.
 */

#ifndef FOL_FORALL_HPP
#define FOL_FORALL_HPP

#include "traits.hpp"

#include <ostream>

namespace fol {

template<typename Var, typename Form>
struct ForAll {
	static_assert(IsVariable<Var>::value, "The first parameter has to be a variable!");
	static_assert(IsFormula<Form>::value, "The secnd parameter has to be a formula!");
	Var v;
	Form f;
	
	friend std::ostream& operator<<(std::ostream& os, const ForAll& e) {
		return os<<'E'<<var<<": "<<f;
	}
};

template<typename Var, typename Form>
ForAll(Var, Form) -> ForAll<Var, Form>;

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator==(const ForAll<Var1, Form1>& e1, const ForAll<Var2, Form2>& e2) noexcept {
	return e1.v == e2.v && e1.f == e2.f;
}

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator!=(const ForAll<Var1, Form1>& e1, const ForAll<Var2, Form2>& e2) noexcept {
	return !(e1 == e2);
}

} //namespace fol

#endif
