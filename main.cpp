#include "and.hpp"
#include "asserts.hpp"
#include "equality.hpp"
#include "equivalent.hpp"
#include "exists.hpp"
#include "forall.hpp"
#include "function.hpp"
#include "implies.hpp"
#include "not.hpp"
#include "or.hpp"
#include "predicate.hpp"
#include "pretty_printer.hpp"
#include "variable.hpp"

#include <cassert>
#include <iostream>

using namespace fol;

int main(void) {
	constexpr Variable<'x'> x;
	constexpr Variable<'F', 'o', 'o'> foo;
	
	constexpr auto bar = foo.prev();
	
	constexpr Function<Name<'f', '1'>> f1;
	constexpr Function<Name<'f', '2'>, Variable<'y'>> f2;
	constexpr Function<Name<'f', '3'>, Variable<'x'>, Variable<'y'>> f3;
	constexpr Function<Name<'f', '4'>, Variable<'x'>, std::decay_t<decltype(f2)>> f4;
	
	constexpr auto f5 = f4.next().append(f4);
	
	std::cout<<x<<' '<<sizeof(x)<<std::endl
	         <<foo<<' '<<sizeof(foo)<<std::endl
	         <<bar<<' '<<sizeof(bar)<<std::endl
	         <<f1<<' '<<sizeof(f1)<<std::endl
	         <<f2<<' '<<sizeof(f2)<<std::endl
	         <<f3<<' '<<sizeof(f3)<<std::endl
	         <<f4<<' '<<sizeof(f4)<<std::endl
	         <<f5<<' '<<sizeof(f5)<<std::endl
	         ;
	
	constexpr Predicate<Name<'p', '1'>> p1;
	constexpr Predicate<Name<'p', '2'>, Variable<'y'>> p2;
	constexpr Predicate<Name<'p', '3'>, Variable<'x'>, Variable<'y'>> p3;
	constexpr Predicate<Name<'p', '4'>, Variable<'x'>, std::decay_t<decltype(f2)>> p4;
	
	std::cout<<p1<<' '<<sizeof(p1)<<std::endl
	         <<p2<<' '<<sizeof(p2)<<std::endl
	         <<p3<<' '<<sizeof(p3)<<std::endl
	         <<p4<<' '<<sizeof(p4)<<std::endl
	         ;
	
	constexpr Equality<Variable<'x'>, Variable<'y'>> e;
	std::cout<<e<<' '<<sizeof(e)<<std::endl;
	
	constexpr And<Not<std::decay_t<decltype(p1)>>, std::decay_t<decltype(p2)>> a;
	std::cout<<a<<' '<<sizeof(a)<<std::endl;
	
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
	
	Equality<RtVariable, RtVariable> rE{{'x'}, {"y"}};
	std::cout<<rE<<' '<<sizeof(rE)<<std::endl;
	
	Function<RtName> mF1{{"f1"}};
	Function<Name<'f', '2'>, RtVariable> mF2{{}, {"y"}};
	Function<RtName, RtVariable, Variable<'y'>> mF3{{"f3"}, {"x"}, {}};
	Function<Name<'f', '4'>, Variable<'x'>, std::decay_t<decltype(mF2)>> mF4{{}, {}, {mF2}};
	
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
	
	Predicate<RtName> mP1{{"p1"}};
	Predicate<Name<'p', '2'>, RtVariable> mP2{{}, {"y"}};
	Predicate<RtName, RtVariable, Variable<'y'>> mP3{{"p3"}, {"x"}, {}};
	Predicate<Name<'p', '4'>, Variable<'x'>, std::decay_t<decltype(mF2)>> mP4{{}, {}, {mF2}};
	
	std::cout<<mP1<<' '<<sizeof(mP1)<<std::endl
	         <<mP2<<' '<<sizeof(mP2)<<std::endl
	         <<mP3<<' '<<sizeof(mP3)<<std::endl
	         <<mP4<<' '<<sizeof(mP4)<<std::endl
	         ;
	
	Equality<Variable<'x'>, RtVariable> mE{{}, {'y'}};
	std::cout<<mE<<' '<<sizeof(mE)<<std::endl;
	
	And<Not<std::decay_t<decltype(mP1)>>, std::decay_t<decltype(mP2)>> mA{{{mP1}}, {mP2}};
	std::cout<<mA<<' '<<sizeof(mA)<<std::endl;
	assert(a == mA);
	
	static_assert(x == Variable<'x'>{});
	constexpr auto y = Variable<'y'>{};
	constexpr auto z = Variable<'z'>{};
	
	constexpr auto lovesPred = [](auto... par) noexcept {
			return Predicate{Name<'L', 'o', 'v', 'e', 's'>{}, par...};
		};
	
	constexpr auto leftImplication = Implies{Predicate{Name<'A', 'n', 'i', 'm', 'a', 'l'>{}, y}, lovesPred(x, y)};
	constexpr auto innerFormula = Implies{ForAll{y, leftImplication}, Exists{y, lovesPred(y, x)}};
	
	constexpr auto formula = ForAll{x, innerFormula};
	constexpr auto simplified = formula.simplified();
	constexpr auto nnf = formula.toNegationNormalForm();
	
	constexpr auto andOr = And{lovesPred(x), Or{lovesPred(y), lovesPred(z)}};
	constexpr auto orAnd = Or{And{lovesPred(x), lovesPred(y)}, lovesPred(z)};
	
	std::cout<<std::endl
	         <<"   ====   Formula   ===="<<std::endl
	         <<std::endl
	         <<"AndOr: "<<andOr<<" -- "<<PrettyPrinter{andOr}<<std::endl
	         <<"OrAnd: "<<orAnd<<" -- "<<PrettyPrinter{orAnd}<<std::endl
	         <<std::endl
	         <<"Normal:     "<<PrettyPrinter{formula}<<std::endl
	         <<"Simplified: "<<PrettyPrinter{simplified}<<std::endl
	         <<"NNF:        "<<PrettyPrinter{nnf}<<std::endl;
	return 0;
}
