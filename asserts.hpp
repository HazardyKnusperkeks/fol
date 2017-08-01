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

static_assert(Variable<'w'>{}.next()                == Variable<'x'>{});
static_assert(Variable<'x', 'a'>{}.next()           == Variable<'x', 'b'>{});
static_assert(Variable<'w', 'z'>{}.next()           == Variable<'x', 'a'>{});
static_assert(Variable<'x', 'b', 'b'>{}.next()      == Variable<'x', 'b', 'c'>{});
static_assert(Variable<'x', 'x', 'z'>{}.next()      == Variable<'x', 'y', 'a'>{});
static_assert(Variable<'z'>{}.next()                == Variable<'a', 'a'>{});
static_assert(Variable<'z', 'z'>{}.next()           == Variable<'a', 'a', 'a'>{});
static_assert(Variable<'z', 'z', 'z'>{}.next()      == Variable<'a', 'a', 'a', 'a'>{});
static_assert(Variable<'a', '9'>{}.next()           == Variable<'b', 'a'>{});


} //namespace fol

#endif
