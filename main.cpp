#include "asserts.hpp"
#include "function.hpp"
#include "variable.hpp"

#include <cassert>
#include <iostream>

using namespace fol;

int main(void) {
	Variable<'x'> x;
	Variable<'F', 'o', 'o'> foo;
	
	auto bar = foo.prev();
	
	Function<Name<'f', '1'>> f1;
	Function<Name<'f', '2'>, Variable<'y'>> f2;
	Function<Name<'f', '3'>, Variable<'x'>, Variable<'y'>> f3;
	Function<Name<'f', '4'>, Variable<'x'>, decltype(f2)> f4;
	
	auto f5 = f4.next().append(f4);
	
	std::cout<<x<<' '<<sizeof(x)<<std::endl
	         <<foo<<' '<<sizeof(foo)<<std::endl
	         <<bar<<' '<<sizeof(bar)<<std::endl
	         <<f1<<' '<<sizeof(f1)<<std::endl
	         <<f2<<' '<<sizeof(f2)<<std::endl
	         <<f3<<' '<<sizeof(f3)<<std::endl
	         <<f4<<' '<<sizeof(f4)<<std::endl
	         <<f5<<' '<<sizeof(f5)<<std::endl
	         ;
	
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
	
	assert(RtVariable{"x"}.prev()    == RtVariable{"w"});
	assert(RtVariable{"xb"}.prev()   == RtVariable{"xa"});
	assert(RtVariable{"xa"}.prev()   == RtVariable{"wz"});
	assert(RtVariable{"xbc"}.prev()  == RtVariable{"xbb"});
	assert(RtVariable{"xya"}.prev()  == RtVariable{"xxz"});
	assert(RtVariable{"aa"}.prev()   == RtVariable{"z"});
	assert(RtVariable{"aaa"}.prev()  == RtVariable{"zz"});
	assert(RtVariable{"aaaa"}.prev() == RtVariable{"zzz"});
	assert(RtVariable{"a0"}.prev()   == RtVariable{"z"});
	try {
		RtVariable{"a"}.prev();
		assert(false);
	} //try
	catch ( std::domain_error& ) {
		
	} //catch ( std::domain_error& )
	catch ( ... ) {
		assert(false);
	} //catch ( ... )

	assert(RtVariable{"w"}.next()    == RtVariable{"x"});
	assert(RtVariable{"xa"}.next()   == RtVariable{"xb"});
	assert(RtVariable{"wz"}.next()   == RtVariable{"xa"});
	assert(RtVariable{"xbb"}.next()  == RtVariable{"xbc"});
	assert(RtVariable{"xxz"}.next()  == RtVariable{"xya"});
	assert(RtVariable{"z"}.next()    == RtVariable{"aa"});
	assert(RtVariable{"zz"}.next()   == RtVariable{"aaa"});
	assert(RtVariable{"zzz"}.next()  == RtVariable{"aaaa"});
	assert(RtVariable{"a9"}.next()   == RtVariable{"ba"});
	
	auto rtBar = rtFoo.prev();
	
	std::cout<<std::endl
	         <<"   ====   Runtime   ===="<<std::endl
	         <<std::endl
	         <<rtX<<' '<<sizeof(rtX)<<std::endl
	         <<rtFoo<<' '<<sizeof(rtFoo)<<std::endl
	         <<rtBar<<' '<<sizeof(rtBar)<<std::endl
	         ;
	
	Function<RtName> mF1{{"f1"}};
	Function<Name<'f', '2'>, RtVariable> mF2{{}, {{"y"}}};
	Function<RtName, RtVariable, Variable<'y'>> mF3{{"f3"}, {{"x"}, {}}};
	Function<Name<'f', '4'>, Variable<'x'>, decltype(mF2)> mF4{{}, {{}, {mF2}}};
	
	auto mF5 = mF4.next().append(mF4);
	
	std::cout<<std::endl
	         <<"   ====    Mixed    ===="<<std::endl
	         <<std::endl
	         <<mF1<<' '<<sizeof(mF1)<<std::endl
	         <<mF2<<' '<<sizeof(mF2)<<std::endl
	         <<mF3<<' '<<sizeof(mF3)<<std::endl
	         <<mF4<<' '<<sizeof(mF4)<<std::endl
	         <<mF5<<' '<<sizeof(mF5)<<std::endl
	         ;
	return 0;
}
