/**
 * @file
 * @brief Contains some static asserts as test for the library.
 */

#ifndef FOL_ASSERTS_HPP
#define FOL_ASSERTS_HPP

#include "function.hpp"
#include "predicate.hpp"
#include "variable.hpp"

#include <type_traits>

namespace fol {

static_assert(std::is_nothrow_default_constructible_v<Variable<'x'>>);
static_assert(std::is_nothrow_default_constructible_v<Variable<'x', 'y'>>);
static_assert(std::is_nothrow_move_constructible_v<Variable<'x'>>);
static_assert(std::is_nothrow_move_constructible_v<Variable<'x', 'y'>>);

static_assert(Variable<'x'>{}   == Variable<'x'>{});
static_assert(!(Variable<'x'>{} != Variable<'x'>{}));
static_assert(Variable<'x'>{}   != Variable<'y'>{});
static_assert(!(Variable<'x'>{} == Variable<'y'>{}));

static_assert(Variable<'x'>{}.prev()                == Variable<'w'>{});
static_assert(Variable<'x', 'b'>{}.prev()           == Variable<'x', 'a'>{});
static_assert(Variable<'x', 'a'>{}.prev()           == Variable<'w', 'z'>{});
static_assert(Variable<'x', 'b', 'c'>{}.prev()      == Variable<'x', 'b', 'b'>{});
static_assert(Variable<'x', 'y', 'a'>{}.prev()      == Variable<'x', 'x', 'z'>{});
static_assert(Variable<'a', 'a'>{}.prev()           == Variable<'z'>{});
static_assert(Variable<'a', 'a', 'a'>{}.prev()      == Variable<'z', 'z'>{});
static_assert(Variable<'a', 'a', 'a', 'a'>{}.prev() == Variable<'z', 'z', 'z'>{});
static_assert(Variable<'a', '0'>{}.prev()           == Variable<'z'>{});
static_assert(noexcept(std::declval<Variable<'b'>>().next()));

static_assert(Variable<'w'>{}.next()                == Variable<'x'>{});
static_assert(Variable<'x', 'a'>{}.next()           == Variable<'x', 'b'>{});
static_assert(Variable<'w', 'z'>{}.next()           == Variable<'x', 'a'>{});
static_assert(Variable<'x', 'b', 'b'>{}.next()      == Variable<'x', 'b', 'c'>{});
static_assert(Variable<'x', 'x', 'z'>{}.next()      == Variable<'x', 'y', 'a'>{});
static_assert(Variable<'z'>{}.next()                == Variable<'a', 'a'>{});
static_assert(Variable<'z', 'z'>{}.next()           == Variable<'a', 'a', 'a'>{});
static_assert(Variable<'z', 'z', 'z'>{}.next()      == Variable<'a', 'a', 'a', 'a'>{});
static_assert(Variable<'a', '9'>{}.next()           == Variable<'b', 'a'>{});
static_assert(noexcept(std::declval<Variable<'b'>>().prev()));

static_assert(std::is_nothrow_constructible_v<Function<Name<'f'>>, Name<'f'>>);
static_assert(std::is_nothrow_constructible_v<Function<Name<'f'>, Variable<'x'>>, Name<'f'>>);
static_assert(std::is_nothrow_default_constructible_v<std::tuple<Variable<'x'>>>);
static_assert(std::is_nothrow_move_constructible_v<std::tuple<Variable<'x'>>>);
static_assert(std::is_nothrow_copy_constructible_v<std::tuple<Variable<'x'>>>);

//This does not hold as of C++17
//static_assert(std::is_nothrow_constructible_v<std::tuple<Variable<'x'>>, Variable<'x'>>);
//static_assert(std::is_nothrow_constructible_v<Function<Name<'f'>, Variable<'x'>>, Name<'f'>, Variable<'x'>>);

static_assert(Function<Name<'g'>>{}.prev() ==
              Function<Name<'f'>>{});
static_assert(Function<Name<'g'>, Variable<'x'>>{}.prev() ==
              Function<Name<'f'>, Variable<'x'>>{});
static_assert(Function<Name<'g'>, Variable<'x'>, Variable<'x'>>{}.prev() ==
              Function<Name<'f'>, Variable<'x'>, Variable<'x'>>{});
static_assert(noexcept(std::declval<Function<Name<'f'>>>().prev()));
static_assert(noexcept(std::declval<Function<Name<'f'>>>().next()));

//Because of the not nothrow constructors of tuple this can't be nothrow either.
//static_assert(noexcept(std::declval<Function<Name<'f'>, Variable<'x'>>>().prev()));
//static_assert(noexcept(std::declval<Function<Name<'f'>, Variable<'x'>>>().next()));

static_assert(Function<Name<'f'>, Variable<'x'>>{}.append(Variable<'y'>{}) ==
              Function<Name<'f'>, Variable<'x'>, Variable<'y'>>{});

static_assert(std::is_nothrow_constructible_v<Predicate<Name<'f'>>, Name<'f'>>);
static_assert(std::is_nothrow_constructible_v<Predicate<Name<'f'>, Variable<'x'>>, Name<'f'>>);
static_assert(std::is_nothrow_default_constructible_v<std::tuple<Variable<'x'>>>);
static_assert(std::is_nothrow_move_constructible_v<std::tuple<Variable<'x'>>>);
static_assert(std::is_nothrow_copy_constructible_v<std::tuple<Variable<'x'>>>);

//This does not hold as of C++17
//static_assert(std::is_nothrow_constructible_v<std::tuple<Variable<'x'>>, Variable<'x'>>);
//static_assert(std::is_nothrow_constructible_v<Predicate<Name<'f'>, Variable<'x'>>, Name<'f'>, Variable<'x'>>);

static_assert(Predicate<Name<'q'>>{}.prev() ==
              Predicate<Name<'p'>>{});
static_assert(Predicate<Name<'q'>, Variable<'x'>>{}.prev() ==
              Predicate<Name<'p'>, Variable<'x'>>{});
static_assert(Predicate<Name<'q'>, Variable<'x'>, Variable<'x'>>{}.prev() ==
              Predicate<Name<'p'>, Variable<'x'>, Variable<'x'>>{});
static_assert(noexcept(std::declval<Predicate<Name<'p'>>>().prev()));
static_assert(noexcept(std::declval<Predicate<Name<'p'>>>().next()));

} //namespace fol

#endif
