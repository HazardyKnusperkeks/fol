/**
 * @file
 * @brief Contains the negation class.
 */

#ifndef FOL_NOT_HPP
#define FOL_NOT_HPP

#include "traits.hpp"

#include <ostream>

namespace fol {

template<typename T>
struct Not {
	static_assert(IsFormula<T>::value, "The negation must contain a formula!");
	T t;
	
	friend std::ostream& operator<<(std::ostream& os, const Not& n) {
		return os<<'-'<<n.t;
	}
	
	constexpr bool operator==(const Not& n) noexcept {
		return t == n.t;
	}
	
	constexpr bool operator!=(const Not& n) noexcept {
		return !operator==(n);
	}
};

template<typename T1, typename T2>
constexpr bool operator==(const Not<T1>, const Not<T2>) noexcept {
	return false;
}

template<typename T1, typename T2>
constexpr bool operator!=(const Not<T1>, const Not<T2>) noexcept {
	return true;
}

}

#endif
