/**
 * @file
 * @brief Contains the function class.
 */

#ifndef FOL_FUNCTION_HPP
#define FOL_FUNCTION_HPP

#include "helper.hpp"
#include "name.hpp"
#include "traits.hpp"

#include <ostream>
#include <tuple>

namespace fol {

template<typename Name, typename... Args>
class Function {
	static_assert(IsName<Name>::value, "First template argument must be a name!");
	static_assert((IsTerm<Args>::value && ...), "All template arguments from the second on have to be terms!");
	
	Name N;
	std::tuple<Args...> A;
	
	friend std::ostream& operator<<(std::ostream& os, const Function& f) {
		os<<f.N;
		if constexpr ( sizeof...(Args) >= 1 ) {
			os<<'('<<f.A<<')';
		} //if constexpr ( sizeof...(Args) >= 1 )
		return os;
	}
};

}

#endif
