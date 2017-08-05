/**
 * @file
 * @brief Contains the exists quantifier.
 */

#ifndef FOL_EXISTS_HPP
#define FOL_EXISTS_HPP

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
		return os<<'E'<<var<<": "<<f;
	}
};

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator==(const Exists<Var1, Form1>& e1, const Exists<Var2, Form2>& e2) noexcept {
	return e1.v == e2.v && e1.f == e2.f;
}

template<typename Var1, typename Form1, typename Var2, typename Form2>
constexpr bool operator!=(const Exists<Var1, Form1>& e1, const Exists<Var2, Form2>& e2) noexcept {
	return !(e1 == e2);
}

} //namespace fol

#endif
