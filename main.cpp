#include "asserts.hpp"
#include "variable.hpp"

#include <cassert>
#include <iostream>

using namespace fol;

int main(void) {
	Variable<'x'> x;
	Variable<'F', 'o', 'o'> foo;
	
	std::cout<<x<<std::endl
	         <<foo<<std::endl;
	
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
	
	std::cout<<std::endl
	         <<"   ====   Runtime   ===="<<std::endl
	         <<rtX<<std::endl
	         <<rtFoo<<std::endl;
	return 0;
}
