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
struct IsVariable : std::false_type { };

template<char... Name>
struct IsVariable<Variable<Name...>> : std::true_type { };

template<>
struct IsVariable<RtVariable> : std::true_type { };

template<typename T>
struct IsTerm : std::conditional_t<IsVariable<T>::value, std::true_type, std::false_type> { };

template<typename Name, typename... Args>
struct IsTerm<Function<Name, Args...>> : std::true_type { };

template<typename T>
struct IsAtom : std::false_type { };

template<typename Name, typename... Args>
struct IsAtom<Predicate<Name, Args...>> : std::true_type { };

template<typename T1, typename T2>
struct IsAtom<Equality<T1, T2>> : std::true_type { };

template<typename T>
struct IsFormula : std::conditional_t<IsAtom<T>::value, std::true_type, std::false_type> { };

static_assert(IsFormula<Predicate<RtName>>::value);
static_assert(!IsFormula<Function<RtName>>::value);

template<typename T>
struct IsFormula<Not<T>> : std::true_type { };

template<typename... Ts>
struct IsFormula<And<Ts...>> : std::true_type { };

template<typename... Ts>
struct IsFormula<Or<Ts...>> : std::true_type { };

template<typename T1, typename T2>
struct IsFormula<Implies<T1, T2>> : std::true_type { };

template<typename T1, typename T2>
struct IsFormula<Equivalent<T1, T2>> : std::true_type { };

template<typename Var, typename Form>
struct IsFormula<Exists<Var, Form>> : std::true_type { };

template<typename Var, typename Form>
struct IsFormula<ForAll<Var, Form>> : std::true_type { };

template<typename T>
struct IsQuantifier : std::false_type { };

template<typename Var, typename Form>
struct IsQuantifier<Exists<Var, Form>> : std::true_type { };

template<typename Var, typename Form>
struct IsQuantifier<ForAll<Var, Form>> : std::true_type { };

} //namespace fol

#endif
