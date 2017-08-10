/**
 * @file
 * @brief Defines the equality "operator".
 */

#ifndef FOL_EQUALITY_HPP
#define FOL_EQUALITY_HPP

#include "traits.hpp"

#include <ostream>
#include <type_traits>

namespace fol {

template<typename T1, typename T2>
struct Equality {
	static_assert(IsTerm<T1>::value && IsTerm<T2>::value, "Equality is only defined for two terms!");
	
	T1 Term1;
	T2 Term2;
	
	constexpr Equality(void) = default;
	
	constexpr Equality(T1 t1, T2 t2)
			noexcept(std::is_nothrow_move_constructible_v<T1> && std::is_nothrow_move_constructible_v<T2>) :
			Term1(std::move(t1)), Term2(std::move(t2)) {
		return;
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Equality& e) {
		return os<<e.Term1<<" = "<<e.Term2;
	}
};

template<typename T1, typename T2>
Equality(T1, T2) -> Equality<T1, T2>;

}

#endif
