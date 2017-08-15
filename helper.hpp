/**
 * @file
 * @brief Declares helper functions.
 */

#ifndef FOL_HELPER_HPP
#define FOL_HELPER_HPP

#include "pretty_printer.hpp"

#include <ostream>
#include <tuple>

namespace fol {

namespace details {

template<typename Tuple, std::size_t I>
std::ostream& print(std::ostream& os, const Tuple& t, const char*, const std::index_sequence<I>) {
	return os << std::get<I>(t);
}

template<typename Tuple, std::size_t I1, std::size_t I2, std::size_t... Is>
std::ostream& print(std::ostream& os, const Tuple& t, const char *delimiter, const std::index_sequence<I1, I2, Is...>) {
	os<<std::get<I1>(t)<<delimiter;
	return print(os, t, delimiter, std::index_sequence<I2, Is...>{});
}

template<typename... Ts>
std::ostream& print(std::ostream& os, const std::tuple<Ts...>& t, const char *delimiter) {
	return print(os, t, delimiter, std::index_sequence_for<Ts...>{});
}

template<typename Tuple, std::size_t I>
std::ostream& prettyPrint(std::ostream& os, const Tuple& t, const int index, const char*, const std::index_sequence<I>) {
	return os << PrettyPrinter{std::get<I>(t), index};
}

template<typename Tuple, std::size_t I1, std::size_t I2, std::size_t... Is>
std::ostream& prettyPrint(std::ostream& os, const Tuple& t, const int index, const char *delimiter, const std::index_sequence<I1, I2, Is...>) {
	os<<PrettyPrinter{std::get<I1>(t), index}<<delimiter;
	return prettyPrint(os, t, index, delimiter, std::index_sequence<I2, Is...>{});
}

template<typename... Ts>
std::ostream& prettyPrint(std::ostream& os, const std::tuple<Ts...>& t, const int index, const char *delimiter) {
	return prettyPrint(os, t, index, delimiter, std::index_sequence_for<Ts...>{});
}

}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::tuple<T>& t) {
	return os<<std::get<0>(t);
}

template<typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::tuple<Ts...>& t) {
	return details::print(os, t, ", ", std::index_sequence_for<Ts...>{});
}

}

#endif
