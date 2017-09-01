/**
 * @file
 * @brief Checks array_set.hpp for self-containment.
 * 
 */

#include "array_set.hpp"

#include <cassert>

using SetType = ArraySet<10, int, bool, char>;

static_assert(SetType{} == SetType{});
static_assert(SetType{5, true, 7, 'h'}.contains(true));
static_assert(SetType{5, true, 7, 'h'}.contains('h'));
static_assert(SetType{5, true, 7, 'h'}.contains(5));
static_assert(!SetType{5, true, 7, 'h'}.contains(false));
static_assert(!SetType{5, true, 7, 'h'}.contains(6));
static_assert(!SetType{5, true, 7, 'h'}.contains('c'));
static_assert(SetType{5, true, 7, 'h'} == SetType{5, true, 7, 'h'});
static_assert(SetType{5, true, 7, 'h'} == SetType{7, true, 5, 'h'});
static_assert(SetType{5, true, 7, 'h'} == SetType{true, 7, 'h', 5});
static_assert(SetType{5, true, 7, 'h'} == SetType{5, 'h', true, 7});
static_assert(SetType{5, true, 7, 'h'} == SetType{5, true, 7, 'h', 7});
static_assert(SetType{5, true, 7, 'h'} != SetType{5, true, 7, 'h', 6});
static_assert(SetType{5, true, 7, 'h'}.insert(8)     == SetType{5, true, 7, 'h', 8});
static_assert(SetType{5, true, 7, 'h'}.insert(false) == SetType{5, true, 7, 'h', false});
static_assert(SetType{5, true, 7, 'h'}.insert(true)  == SetType{5, true, 7, 'h'});
static_assert(SetType{5, true, 7, 'h'}.insert('9')   == SetType{5, true, 7, 'h', '9'});
static_assert(SetType{5, true, 7, 'h'}.remove('9')   == SetType{5, true, 7, 'h'});
static_assert(SetType{5, true, 7, 'h'}.remove('h')   == SetType{5, true, 7});
static_assert(SetType{5, true, 7, 'h'}.remove(7)     == SetType{5, true, 'h'});
static_assert(SetType{5}.remove(5)                   == SetType{});

static_assert(*SetType{5, true, 7, 'h'}.begin() == 5);
static_assert(*(SetType{5, true, 7, 'h'}.begin().operator->()) == 5);
static_assert(*++SetType{5, true, 7, 'h'}.begin() != 5);
static_assert(*++SetType{5, true, 7, 'h'}.begin() == true);
static_assert(*SetType{5, true, 7, 'h'}.begin()++ == 5);
static_assert(*(SetType{5, true, 7, 'h'}.begin() += 3) == 'h');
static_assert(*(SetType{5, true, 7, 'h'}.begin() + 3) == 'h');
static_assert(*(3 + SetType{5, true, 7, 'h'}.begin()) == 'h');
static_assert(SetType{5, true, 7, 'h'}.begin()[3] == 'h');

using Adaptable1 = AdaptableArraySet<10, int, bool>;
using Adaptable2 = AdaptableArraySet<10, int, bool, char>;

static_assert(!Adaptable1{5, true}.contains(7.5));
static_assert(Adaptable1{5, true}.contains(5));
static_assert(Adaptable1{5, true}.remove(7.5) == Adaptable1{5, true});
static_assert(Adaptable1{5, true}.remove('h') == Adaptable1{5, true});
static_assert(Adaptable1{5, true}.remove(5)   == Adaptable1{true});
static_assert(Adaptable1{5, true}.insert(7)   == Adaptable1{5, 7, true});
static_assert(Adaptable1{5, true}.insert('7') == Adaptable2{5, '7', true});

void arraySetRuntimeChecks(void) {
	assert((SetType{} == SetType{}));
	assert((SetType{5, true, 7, 'h'}.contains(true)));
	assert((SetType{5, true, 7, 'h'}.contains('h')));
	assert((SetType{5, true, 7, 'h'}.contains(5)));
	assert((!SetType{5, true, 7, 'h'}.contains(false)));
	assert((!SetType{5, true, 7, 'h'}.contains(6)));
	assert((!SetType{5, true, 7, 'h'}.contains('c')));
	assert((SetType{5, true, 7, 'h'} == SetType{5, true, 7, 'h'}));
	assert((SetType{5, true, 7, 'h'} == SetType{7, true, 5, 'h'}));
	assert((SetType{5, true, 7, 'h'} == SetType{true, 7, 'h', 5}));
	assert((SetType{5, true, 7, 'h'} == SetType{5, 'h', true, 7}));
	assert((SetType{5, true, 7, 'h'} == SetType{5, true, 7, 'h', 7}));
	assert((SetType{5, true, 7, 'h'} != SetType{5, true, 7, 'h', 6}));
	assert((SetType{5, true, 7, 'h'}.insert(8)     == SetType{5, true, 7, 'h', 8}));
	assert((SetType{5, true, 7, 'h'}.insert(false) == SetType{5, true, 7, 'h', false}));
	assert((SetType{5, true, 7, 'h'}.insert(true)  == SetType{5, true, 7, 'h'}));
	assert((SetType{5, true, 7, 'h'}.insert('9')   == SetType{5, true, 7, 'h', '9'}));
	assert((SetType{5, true, 7, 'h'}.remove('9')   == SetType{5, true, 7, 'h'}));
	assert((SetType{5, true, 7, 'h'}.remove('h')   == SetType{5, true, 7}));
	assert((SetType{5, true, 7, 'h'}.remove(7)     == SetType{5, true, 'h'}));
	assert((SetType{5}.remove(5)                   == SetType{}));
	
	assert((*SetType{5, true, 7, 'h'}.begin() == 5));
	assert((*(SetType{5, true, 7, 'h'}.begin().operator->()) == 5));
	assert((*++SetType{5, true, 7, 'h'}.begin() != 5));
	assert((*++SetType{5, true, 7, 'h'}.begin() == true));
	assert((*SetType{5, true, 7, 'h'}.begin()++ == 5));
	assert((*(SetType{5, true, 7, 'h'}.begin() += 3) == 'h'));
	assert((*(SetType{5, true, 7, 'h'}.begin() + 3) == 'h'));
	assert((*(3 + SetType{5, true, 7, 'h'}.begin()) == 'h'));
	assert((SetType{5, true, 7, 'h'}.begin()[3] == 'h'));
	
	assert((!Adaptable1{5, true}.contains(7.5)));
	assert((Adaptable1{5, true}.contains(5)));
	assert((Adaptable1{5, true}.remove(7.5) == Adaptable1{5, true}));
	assert((Adaptable1{5, true}.remove('h') == Adaptable1{5, true}));
	assert((Adaptable1{5, true}.remove(5)   == Adaptable1{true}));
	assert((Adaptable1{5, true}.insert(7)   == Adaptable1{5, 7, true}));
	assert((Adaptable1{5, true}.insert('7') == Adaptable2{5, '7', true}));
	return;
}
