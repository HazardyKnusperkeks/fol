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

template<char... String>
class Variable : public Name<String...> {
	public:
	constexpr auto prev(void) const noexcept {
		auto temp = Name<String...>::prev();
		return fromName(temp);
	}
	
	template<char... String2>
	static constexpr Variable<String2...> fromName(const Name<String2...>) noexcept {
		return {};
	}
};

class RtVariable {
	RtName Name;
	
	public:
	RtVariable(const char c) : Name{c} { return; }
	RtVariable(std::string name) : Name{std::move(name)} {
		return;
	}
	RtVariable(RtName name) : Name{std::move(name)} {
		return;
	}
	
	friend bool operator==(const RtVariable& v1, const RtVariable& v2) noexcept {
		return v1.Name == v2.Name;
	}
	
	friend bool operator!=(const RtVariable& v1, const RtVariable& v2) noexcept {
		return !(v1 == v2);
	}
	
	template<char... String>
	friend bool operator==(const RtVariable& v1, const Variable<String...> v2) noexcept {
		return compare(v2, v1.Name);
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
	
	friend std::ostream& operator<<(std::ostream& os, const fol::RtVariable& v) noexcept {
		return os<<v.Name;
	}
};

template<char c, char... String>
std::ostream& operator<<(std::ostream& os, const fol::Variable<c, String...> v) noexcept {
	print(os, v);
	return os;
}

} //namespace fol

#endif
