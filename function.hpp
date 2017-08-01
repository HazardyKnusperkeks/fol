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

template<typename NameT, typename... Args>
class Function {
	static_assert(IsName<NameT>::value, "First template argument must be a name!");
	static_assert((IsTerm<Args>::value && ...), "All template arguments from the second on have to be terms!");
	
	NameT N;
	std::tuple<Args...> A;
	
	friend std::ostream& operator<<(std::ostream& os, const Function& f) {
		os<<f.N;
		if constexpr ( sizeof...(Args) >= 1 ) {
			os<<'('<<f.A<<')';
		} //if constexpr ( sizeof...(Args) >= 1 )
		return os;
	}
	
	template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
	friend constexpr bool operator==(const Function<Name1T, Args1...> f1, const Function<Name2T, Args2...> f2) noexcept;
	
	template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
	friend constexpr bool operator!=(const Function<Name1T, Args1...> f1, const Function<Name2T, Args2...> f2) noexcept;
};

template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
constexpr bool operator==(const Function<Name1T, Args1...> f1, const Function<Name2T, Args2...> f2) noexcept {
	return f1.N == f2.N && f1.A == f2.A;
}

template<typename Name1T, typename Name2T, typename... Args1, typename... Args2>
constexpr bool operator!=(const Function<Name1T, Args1...> f1, const Function<Name2T, Args2...> f2) noexcept {
	return !(f1 == f2);
}

}

#endif
