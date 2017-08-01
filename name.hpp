/**
 * @file
 * @brief Defines the name class.
 */

#ifndef FOL_NAME_HPP
#define FOL_NAME_HPP

#include <algorithm>
#include <array>
#include <ostream>
#include <stdexcept>
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

constexpr bool prevWrap(const char c) noexcept {
	return toLower(c) == 'a' || c == '0';
}

constexpr bool nextWrap(const char c) noexcept {
	return toLower(c) == 'z' || c == '9';
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
		if constexpr ( details::prevWrap(lastChar) ) {
			static_assert(Length::value > 1, "There is no previous name to \"a\" or \"A\"!");
			
			/* At least MinGW 7.1 does try to calculate Char<Length::value-2>::value even though the assert fired. So
			 * add another if and a wrong return value. This way at least the compiler terminates. */
			if constexpr ( Length::value <= 1 ) {
				return;
			} //if constexpr ( Length::value <= 1 )
			else {
				constexpr auto nextToLastChar = Char<Length::value-2>::value;
				if constexpr ( details::prevWrap(nextToLastChar) ) {
					return details::namePrevHelper<Length::value-2, 'z'>(*this);
				} //if constexpr ( details::prevWrap(nextToLastChar) )
				else {
					return details::namePrevHelper<Length::value-2, nextToLastChar-1, 'z'>(*this);
				} //else -> if constexpr ( details::prevWrap(nextToLastChar) )
			} //else -> if constexpr ( Length::value <= 1 )
		} //if constexpr ( details::prevWrap(lastChar) )
		else {
			return details::namePrevHelper<Length::value-1, lastChar-1>(*this);
		} //else -> if constexpr ( details::prevWrap(lastChar) )
	}
	
	constexpr bool operator==(const Name) const noexcept { return true; }
	constexpr bool operator!=(const Name) const noexcept { return false; }
	
	template<char... String2>
	constexpr bool operator==(const Name<String2...>) const noexcept { return false; }
	template<char... String2>
	constexpr bool operator!=(const Name<String2...>) const noexcept { return true; }
};

template<char... String, std::enable_if_t<sizeof...(String)>=1>* = nullptr>
std::ostream& operator<<(std::ostream& os, const Name<String...>) noexcept {
	(os<<...<<String);
	return os;
}

class RtName {
	std::string Name;
	public:
	RtName(const char c) : Name{&c, 1} { return; }
	RtName(std::string name) : Name{std::move(name)} {
		if ( Name.empty() ) {
			throw std::invalid_argument{"Variable name must not be empty!"};
		} //if ( Name.empty() )
		return;
	}
	
	RtName prev(void) const & {
		return RtName{*this}.prev();
	}
	
	RtName prev(void) && {
		char& lastChar = Name.back();
		if ( details::prevWrap(lastChar) ) {
			if ( Name.size() <= 1 ) {
				throw std::domain_error{"There is no previous name to \"a\" or \"A\"!"};
			} //if ( Name.size() <= 1 )
			char& nextToLastChar = Name[Name.size() - 2];
			if ( details::prevWrap(nextToLastChar) ) {
				nextToLastChar = 'z';
				Name.erase(Name.size() - 1);
			} //if ( details::prevWrap(nextToLastChar) )
			else {
				--nextToLastChar;
				lastChar = 'z';
			} //else -> if ( details::prevWrap(nextToLastChar) )
		} //if ( details::prevWrap(lastChar) )
		else {
			--lastChar;
		} //else -> if ( details::prevWrap(lastChar) )
		return std::move(*this);
	}
	
	friend bool operator==(const RtName& n1, const RtName& n2) noexcept {
		return n1.Name == n2.Name;
	}
	
	friend bool operator!=(const RtName& n1, const RtName& n2) noexcept {
		return !(n1 == n2);
	}
	
	friend std::ostream& operator<<(std::ostream& os, const RtName& n) noexcept {
		return os<<n.Name;
	}
	
	template<char... String>
	friend bool compare(const fol::Name<String...>, const RtName& name) noexcept {
		auto str = std::array{String...};
		return std::equal(str.begin(), str.end(), name.Name.begin(), name.Name.end());
	}
};

} //namespace fol

#endif
