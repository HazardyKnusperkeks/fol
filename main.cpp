#include "asserts.hpp"
#include "variable.hpp"

#include <iostream>

using namespace fol;

int main(void) {
	Variable<'x'> x;
	Variable<'F', 'o', 'o'> foo;
	
	std::cout<<x<<std::endl
	         <<foo<<std::endl;
	return 0;
}
