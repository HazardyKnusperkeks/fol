#include "asserts.hpp"
#include "variable.hpp"

#include <cassert>
#include <iostream>

using namespace fol;

int main(void) {
	Variable<'x'> x;
	Variable<'F', 'o', 'o'> foo;
	
	auto bar = foo.prev();
	
	std::cout<<x<<' '<<sizeof(x)<<std::endl
	         <<foo<<' '<<sizeof(foo)<<std::endl
	         <<bar<<std::endl;
	
	volatile char cX{'x'};
	
	RtVariable rtX{cX}, rtX2{"x"}, rtY{'y'}, rtFoo{"Foo"};
	assert(rtX == x);
	assert(x == rtX);
	assert(rtFoo == foo);
	assert(foo == rtFoo);
	assert(rtY != x);
	assert(x != rtY);
	assert(rtX != rtY);
	assert(rtX == rtX2);
	assert(rtX == cX);
	assert(cX == rtX);
	assert(rtFoo != cX);
	assert(cX != rtFoo);
/*	assert(x == cX);
	assert(cX == x);
	assert(foo != cX);
	assert(cX != foo);*/
	
	assert(RtVariable{"x"}.prev()   == RtVariable{"w"});
	assert(RtVariable{"xb"}.prev()  == RtVariable{"xa"});
	assert(RtVariable{"xa"}.prev()  == RtVariable{"wz"});
	assert(RtVariable{"xbc"}.prev() == RtVariable{"xbb"});
	assert(RtVariable{"xya"}.prev() == RtVariable{"xxz"});
	assert(RtVariable{"aa"}.prev()  == RtVariable{"z"});
	assert(RtVariable{"aaa"}.prev() == RtVariable{"az"});
	try {
		RtVariable{"a"}.prev();
		assert(false);
	} //try
	catch ( std::domain_error& ) {
		
	} //catch ( std::domain_error& )
	catch ( ... ) {
		assert(false);
	} //catch ( ... )
	
	auto rtBar = rtFoo.prev();
	
	std::cout<<std::endl
	         <<"   ====   Runtime   ===="<<std::endl
	         <<rtX<<' '<<sizeof(rtX)<<std::endl
	         <<rtFoo<<' '<<sizeof(rtFoo)<<std::endl
	         <<rtBar<<std::endl;
	return 0;
}
