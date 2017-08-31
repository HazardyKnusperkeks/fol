/**
 * @file
 * @brief Checks constexpr_variant.hpp for self-containment.
 * 
 */

#include "constexpr_variant.hpp"

static_assert(ConstexprVariant<int, char>{7}   == ConstexprVariant<int, char>{7});
static_assert(ConstexprVariant<int, char>{'7'} == ConstexprVariant<int, char>{'7'});
static_assert(ConstexprVariant<int, char>{7}   != ConstexprVariant<int, char>{'7'});
static_assert(ConstexprVariant<int, char>{7}   == ConstexprVariant<char, int>{7});
static_assert(ConstexprVariant<int, char>{'7'} == ConstexprVariant<char, int>{'7'});
static_assert(ConstexprVariant<int, char>{'7'} != ConstexprVariant<char, int>{7});
static_assert(ConstexprVariant<int, char>{'7'} != ConstexprVariant<char, int>{});
static_assert(ConstexprVariant<int, char>{}    == ConstexprVariant<char, int>{});
static_assert(ConstexprVariant<char, int>{'7'} == ConstexprVariant<int, char>{'7'});
static_assert(ConstexprVariant<char, int>{'7'} != ConstexprVariant<int, char>{7});
static_assert(ConstexprVariant<char, int>{'7'} != ConstexprVariant<int, char>{});
static_assert(ConstexprVariant<char, int>{}    == ConstexprVariant<int, char>{});
