/**
 * @file
 * @brief Contains some type traits for the FOL-types.
 */

#ifndef FOL_TRAITS_HPP
#define FOL_TRAITS_HPP

#include "forward.hpp"

#include <type_traits>

namespace fol {

struct IsTerm : std::false_type { };

template<char... Name>
struct IsTerm<Variable<Name>> : std::true_type { };

template<>
struct IsTerm<RtVariable> : std::true_type { };

} //namespace fol

#endif
