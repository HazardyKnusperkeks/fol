/**
 * @file
 * @brief Contains some type traits for the FOL-types.
 */

#ifndef FOL_TRAITS_HPP
#define FOL_TRAITS_HPP

#include "forward.hpp"

#include <type_traits>

namespace fol {

template<typename T>
struct IsName : std::false_type { };

template<char... String>
struct IsName<Name<String...>> : std::true_type { };

template<>
struct IsName<RtName> : std::true_type { };

template<typename T>
struct IsTerm : std::false_type { };

template<char... Name>
struct IsTerm<Variable<Name...>> : std::true_type { };

template<>
struct IsTerm<RtVariable> : std::true_type { };

template<typename Name, typename... Args>
struct IsTerm<Function<Name, Args...>> : std::true_type { };

} //namespace fol

#endif
