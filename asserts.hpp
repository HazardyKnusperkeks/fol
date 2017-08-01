/**
 * @file
 * @brief Contains some static asserts as test for the library.
 */

#ifndef FOL_ASSERTS_HPP
#define FOL_ASSERTS_HPP

#include "variable.hpp"

namespace fol {

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


} //namespace fol

#endif
