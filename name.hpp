/**
 * @file
 * @brief Defines the name class.
 */

#ifndef FOL_NAME_HPP
#define FOL_NAME_HPP

#include <algorithm>
#include <array>
#include <ostream>
#include <string>

namespace fol {

namespace details {
template<char c>
void print(std::ostream& os) {
	os<<c;
	return;
}

template<char c1, char c2, char... String>
void print(std::ostream& os) {
	os<<c1;
	print<c2,String...>(os);
	return;
}
}

template<char... String>
class Name {
	static_assert(sizeof...(String) >= 1, "The name must not be empty!");
	public:
	constexpr bool operator==(const Name) const noexcept { return true; }
	constexpr bool operator!=(const Name) const noexcept { return false; }
	
	template<char... String2>
	constexpr bool operator==(const Name<String2...>) const noexcept { return false; }
	template<char... String2>
	constexpr bool operator!=(const Name<String2...>) const noexcept { return true; }
};

template<char... String>
void print(std::ostream& os, const Name<String...>) noexcept {
	details::print<String...>(os);
	return;
}

template<char... String>
bool compare(const Name<String...>, const std::string& s) noexcept {
	auto str = std::array{String...};
	return std::equal(str.begin(), str.end(), s.begin(), s.end());
}

} //namespace fol

#endif
