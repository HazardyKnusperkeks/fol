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
#include <utility>

namespace fol {

template<char... String>
class Name;

namespace details {
constexpr char toLower(const char c) noexcept {
	if ( c >= 'A' && c <= 'Z' ) {
		return static_cast<char>(c - 'A' + 'a');
	} //if ( c >= 'A' && c <= 'Z' )
	return c;
}

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

template<std::size_t N, char c1, char... String>
struct NameChar {
	static constexpr char c = NameChar<N-1, String...>::c;
};

template<char c1, char... String>
struct NameChar<0, c1, String...> {
	static constexpr char c = c1;
};

template<std::size_t N, char... String>
constexpr char nameChar(const Name<String...>) noexcept {
	return NameChar<N, String...>::c;
}

template<char... c, char... String, std::size_t... Idx>
constexpr auto namePrevHelperImpl([[maybe_unused]] const Name<String...> n, const std::index_sequence<Idx...>) noexcept {
	return Name<(nameChar<Idx>(n))...,c...>{};
}

template<std::size_t N, char... c, char... String, typename Idx = std::make_index_sequence<N>>
constexpr auto namePrevHelper(const Name<String...> n) noexcept {
	return namePrevHelperImpl<c...>(n, Idx{});
}
}

template<char... String>
class Name {
	static_assert(sizeof...(String) >= 1, "The name must not be empty!");
	public:
	template<std::size_t N>
	using Char   = std::integral_constant<char, details::NameChar<N, String...>::c>;
	using Length = std::integral_constant<std::size_t, sizeof...(String)>;
	
	constexpr auto prev(void) const noexcept {
		constexpr auto lastChar = Char<Length::value-1>::value;
		if constexpr ( details::toLower(lastChar) == 'a' ) {
			static_assert(Length::value > 1, "There is no previous name to \"a\" or \"A\"!");
			
			/* At least MinGW 7.1 does try to calculate Char<Length::value-2>::value even though the assert fired. So
			 * add another if and a wrong return value. This way at least the compiler terminates. */
			if constexpr ( Length::value <= 1 ) {
				return;
			} //if constexpr ( Length::value <= 1 )
			else {
				constexpr auto nextToLastChar = Char<Length::value-2>::value;
				if constexpr ( details::toLower(nextToLastChar) == 'a' ) {
					return details::namePrevHelper<Length::value-2, 'z'>(*this);
				} //if constexpr ( details::toLower(nextToLastChar) == 'a' )
				else {
					return details::namePrevHelper<Length::value-2, nextToLastChar-1, 'z'>(*this);
				} //else -> if constexpr ( details::toLower(nextToLastChar) == 'a' )
			} //else -> if constexpr ( Length::value <= 1 )
		} //if constexpr ( details::toLower(lastChar) == 'a' )
		else {
			return details::namePrevHelper<Length::value-1, lastChar-1>(*this);
		} //else -> if constexpr ( details::toLower(lastChar) == 'a' )
	}
	
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
