/**
 * @file
 * @brief Defines the Variables.
 */

#ifndef FOL_VARIABLE_HPP
#define FOL_VARIABLE_HPP

#include "name.hpp"

#include <ostream>
#include <string>

namespace fol {

template<char... String>
class Variable : public Name<String...> { };

template<char c, char... String>
std::ostream& operator<<(std::ostream& os, const fol::Variable<c, String...> v) noexcept {
	print(os, v);
	return os;
}

} //namespace fol

#endif
