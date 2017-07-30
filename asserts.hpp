/**
 * @file
 * @brief Contains some static asserts as test for the library.
 */

#ifndef FOL_ASSERTS_HPP
#define FOL_ASSERTS_HPP

#include "variable.hpp"

namespace fol {

static_assert(Variable<'x'>{} == Variable<'x'>{});
static_assert(!(Variable<'x'>{} != Variable<'x'>{}));
static_assert(Variable<'x'>{} != Variable<'y'>{});
static_assert(!(Variable<'x'>{} == Variable<'y'>{}));

} //namespace fol

#endif
