/**
 * @file
 * @brief Contains some type traits for the FOL-types.
 */

#ifndef FOL_TRAITS_HPP
#define FOL_TRAITS_HPP

#include "forward.hpp"

#include <type_traits>

namespace fol {

struct IsAtom : std::false_type { };

template<char... Name>
struct IsAtom<Variable<Name>> : std::true_type { };

} //namespace fol

#endif
