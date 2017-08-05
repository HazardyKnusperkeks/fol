/**
 * @file
 * @brief Defines the Variables.
 */

#ifndef FOL_VARIABLE_HPP
#define FOL_VARIABLE_HPP

#include "name.hpp"

#include <array>
#include <ostream>

namespace fol {

template<char c, char... String>
struct Variable {
	Name<c,String...> N;
	
	constexpr auto prev(void) const noexcept {
		return fromName(N.prev());
	}
	
	constexpr auto next(void) const noexcept {
		return fromName(N.next());
	}
	
	template<char... String2>
	static constexpr Variable<String2...> fromName(const Name<String2...>) noexcept {
		return {};
	}
};

template<char... String1, char... String2>
constexpr bool operator==(const Variable<String1...> v1, const Variable<String2...> v2) noexcept {
	return v1.N == v2.N;
}

template<char... String1, char... String2>
constexpr bool operator!=(const Variable<String1...> v1, const Variable<String2...> v2) noexcept {
	return !(v1 == v2);
}

template<char... String>
std::ostream& operator<<(std::ostream& os, const Variable<String...> v) {
	return os<<v.N;
}

struct RtVariable {
	RtName Name;
	
	RtVariable(const char c) : Name{c} { return; }
	RtVariable(std::string name) : Name{std::move(name)} {
		return;
	}
	RtVariable(RtName name) : Name{std::move(name)} {
		return;
	}
	
	RtVariable prev(void) const & {
		return {Name.prev()};
	}
	
	RtVariable prev(void) && {
		return {std::move(Name).prev()};
	}
	
	RtVariable next(void) const & {
		return {Name.next()};
	}
	
	RtVariable next(void) && {
		return {std::move(Name).next()};
	}
	
	friend bool operator==(const RtVariable& v1, const RtVariable& v2) noexcept {
		return v1.Name == v2.Name;
	}
	
	friend bool operator!=(const RtVariable& v1, const RtVariable& v2) noexcept {
		return !(v1 == v2);
	}
	
	template<char... String>
	friend bool operator==(const RtVariable& v1, const Variable<String...> v2) noexcept {
		return v2.N == v1.Name;
	}
	
	template<char... String>
	friend bool operator==(const Variable<String...> v1, const RtVariable& v2) noexcept {
		return v2 == v1;
	}
	
	template<char... String>
	friend bool operator!=(const RtVariable& v1, const Variable<String...> v2) noexcept {
		return !(v1 == v2);
	}
	
	template<char... String>
	friend bool operator!=(const Variable<String...> v1, const RtVariable& v2) noexcept {
		return !(v2 == v1);
	}
	
	friend std::ostream& operator<<(std::ostream& os, const RtVariable& v) noexcept {
		return os<<v.Name;
	}
};

} //namespace fol

#endif
