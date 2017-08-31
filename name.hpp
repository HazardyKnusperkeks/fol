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

template<char... String>
struct NameHelper {
	bool ChangePrevious{false};
	
	template<char... String2>
	constexpr NameHelper<String...,String2...> operator+(const NameHelper<String2...> n) noexcept {
		return {ChangePrevious && n.ChangePrevious};
	}
};

template<char... String>
constexpr Name<String...> fromHelper(const NameHelper<String...>) noexcept {
	return {};
}

template<char c>
constexpr auto namePrevImpl(void) noexcept {
	if constexpr ( prevWrap(c) ) {
		return NameHelper<>{true};
	} //if constexpr ( prevWrap(c) )
	else {
		return NameHelper<static_cast<char>(c-1)>{false};
	} //else -> if constexpr ( prevWrap(c) )
}

template<char c1, char c2, char... String>
constexpr auto namePrevImpl(void) noexcept {
	constexpr auto tail = namePrevImpl<c2,String...>();
	if constexpr ( tail.ChangePrevious ) {
		if constexpr ( prevWrap(c1) ) {
			return NameHelper<'z'>{true} + tail;
		} //if constexpr ( prevWrap(c1) )
		else {
			return NameHelper<static_cast<char>(c1-1), 'z'>{} + tail;
		} //else -> if constexpr ( prevWrap(c1) )
	} //if constexpr ( tail.ChangePrevious )
	else {
		return NameHelper<c1>{} + tail;
	} //else -> if constexpr ( tail.ChangePrevious )
}

template<char... String>
constexpr auto namePrev(const Name<String...>) noexcept {
	return fromHelper(namePrevImpl<String...>());
}

template<char c>
constexpr auto nameNextImpl(void) noexcept {
	if constexpr ( nextWrap(c) ) {
		return NameHelper<'a'>{true};
	} //if constexpr ( nextWrap(c) )
	else {
		return NameHelper<static_cast<char>(c+1)>{false};
	} //else -> if constexpr ( nextWrap(c) )
}

template<char c1, char c2, char... String>
constexpr auto nameNextImpl(void) noexcept {
	constexpr auto tail = nameNextImpl<c2,String...>();
	if constexpr ( tail.ChangePrevious ) {
		if constexpr ( nextWrap(c1) ) {
			return NameHelper<'a'>{true} + tail;
		} //if constexpr ( nextWrap(c1) )
		else {
			return NameHelper<static_cast<char>(c1+1)>{} + tail;
		} //else -> if constexpr ( nextWrap(c1) )
	} //if constexpr ( tail.ChangePrevious )
	else {
		return NameHelper<c1>{} + tail;
	} //else -> if constexpr ( tail.ChangePrevious )
}

template<char... String>
constexpr auto nameNext(const Name<String...>) noexcept {
	constexpr auto temp = nameNextImpl<String...>();
	if constexpr ( temp.ChangePrevious ) {
		return fromHelper(NameHelper<'a'>{} + temp);
	} //if constexpr ( temp.ChangePrevious )
	else {
		return fromHelper(temp);
	} //else -> if constexpr ( temp.ChangePrevious )
}
} //namespace details

template<char... String>
class Name {
	static_assert(sizeof...(String) >= 1, "The name must not be empty!");
	
	public:
	template<std::size_t N>
	using Char   = std::integral_constant<char, details::NameChar<N, String...>::c>;
	using Length = std::integral_constant<std::size_t, sizeof...(String)>;
	
	constexpr auto prev(void) const noexcept {
		constexpr auto lastChar = Char<Length::value-1>::value;
		static_assert(Length::value > 1 || !details::prevWrap(lastChar), "There is no previous name to \"a\", \"A\", or \"0\"!");
		
		/* At least MinGW 7.1 does try to calculate Char<Length::value-2>::value even though the assert fired. So
		 * add another if and a wrong return value. This way at least the compiler terminates. */
		if constexpr ( Length::value <= 1 && details::prevWrap(lastChar)) {
			return;
		} //if constexpr ( Length::value <= 1 && details::prevWrap(lastChar))
		else {
			return details::namePrev(*this);
		} //else -> if constexpr ( Length::value <= 1 && details::prevWrap(lastChar))
	}
	
	constexpr auto next(void) const noexcept {
		return details::nameNext(*this);
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

template<typename CharT, CharT... String>
constexpr Name<String...> operator""_name(void) {
	return {};
}

template<char... String>
constexpr Name<String...> operator""_name(void) {
	return {};
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
		{
			char& lastChar = Name.back();
			if ( !details::prevWrap(lastChar) ) {
				--lastChar;
				return std::move(*this);
			} //if ( !details::prevWrap(lastChar) )
		}
		
		if ( Name.size() <= 1 ) {
			throw std::domain_error{"There is no previous name to \"a\", \"A\", or \"0\"!"};
		} //if ( Name.size() <= 1 )
		
		Name.pop_back();
		for ( auto iter = Name.rbegin(), end = Name.rend(); iter != end; ++iter ) {
			if ( details::prevWrap(*iter) ) {
				*iter = 'z';
			} //if ( details::prevWrap(*iter) )
			else {
				--*iter;
				Name.insert(iter.base(), 'z');
				return std::move(*this);
			} //else -> if ( details::prevWrap(*iter) )
		} //for ( auto iter = Name.rbegin(), end = Name.rend(); iter != end; ++iter )
		return std::move(*this);
	}
	
	RtName next(void) const & {
		return RtName{*this}.next();
	}
	
	RtName next(void) && {
		{
			char& lastChar = Name.back();
			if ( !details::nextWrap(lastChar) ) {
				++lastChar;
				return std::move(*this);
			} //if ( !details::nextWrap(lastChar) )
		}
		
		for ( auto iter = Name.rbegin(), end = Name.rend(); iter != end; ++iter ) {
			if ( details::nextWrap(*iter) ) {
				*iter = 'a';
			} //if ( details::nextWrap(*iter) )
			else {
				++*iter;
				return std::move(*this);
			} //else -> if ( details::nextWrap(*iter) )
		} //for ( auto iter = Name.rbegin(), end = Name.rend(); iter != end; ++iter )
		Name.insert(Name.begin(), 'a');
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
	static bool compare(const fol::Name<String...>, const RtName& name) noexcept {
		auto str = std::array{String...};
		return std::equal(str.begin(), str.end(), name.Name.begin(), name.Name.end());
	}
};

template<char... String>
bool operator==(const RtName& rt, const Name<String...> n) noexcept {
	return RtName::compare(n, rt);
}

template<char... String>
bool operator==(const Name<String...> n, const RtName& rt) noexcept {
	return RtName::compare(n, rt);
}

template<char... String>
bool operator!=(const RtName& rt, const Name<String...> n) noexcept {
	return !(rt == n);
}

template<char... String>
bool operator!=(const Name<String...> n, const RtName& rt) noexcept {
	return !(n == rt);
}

} //namespace fol

#endif
