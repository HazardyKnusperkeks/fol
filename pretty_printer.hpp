/**
 * @file
 * @brief Defines the pretty printer interface.
 */

#ifndef PRETTY_PRINTER_HPP
#define PRETTY_PRINTER_HPP

#include <ostream>
#include <utility>
#include <vector>

namespace fol {

std::vector<std::pair<char, char>> PrettyParanthesis = {{'(', ')'}, {'[', ']'}, {'{', '}'}};

template<typename T>
struct PrettyPrinter { };

template<typename T>
PrettyPrinter(T) -> PrettyPrinter<T>;

template<typename T>
PrettyPrinter(T, int) -> PrettyPrinter<T>;

template<typename T>
std::ostream& operator<<(std::ostream& os, const PrettyPrinter<T>& pp) {
	return pp.prettyPrint(os);
}

} //namespace fol

#endif
