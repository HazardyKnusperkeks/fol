/**
 * @file
 * @brief Declares helper functions.
 */

#ifndef FOL_HELPER_HPP
#define FOL_HELPER_HPP

#include <ostream>
#include <tuple>

namespace fol {

namespace details {

template<typename Tuple, std::size_t I>
std::ostream& print(std::ostream& os, const Tuple& t, const std::index_sequence<I>) {
	return os << std::get<I>(t);
}
template<typename Tuple, std::size_t I1, std::size_t I2, std::size_t... Is>
std::ostream& print(std::ostream& os, const Tuple& t, const std::index_sequence<I1, I2, Is...>) {
	os<<std::get<I1>(t)<<", ";
	return print(os, t, std::index_sequence<I2, Is...>{});
}

}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::tuple<T>& t) {
	return os<<std::get<0>(t);
}

template<typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::tuple<Ts...>& t) {
	return details::print(os, t, std::index_sequence_for<Ts...>{});
}

}

#endif
