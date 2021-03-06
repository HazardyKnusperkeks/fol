/**
 * @file
 * @brief Defines the equality "operator".
 */

#ifndef FOL_EQUALITY_HPP
#define FOL_EQUALITY_HPP

#include "not.hpp"
#include "pretty_printer.hpp"
#include "traits.hpp"

#include <ostream>
#include <type_traits>

namespace fol {

template<typename T1, typename T2>
struct Equality {
	static_assert(IsTerm<T1>::value && IsTerm<T2>::value, "Equality is only defined for two terms!");
	
	T1 Term1;
	T2 Term2;
	
	using VariableCount = std::integral_constant<std::size_t, T1::VariableCount::value + T2::VariableCount::value>;
//	using VariableArray = decltype(ArraySet<0>{} + typename T1::VariableArray{} + typename T1::VariableArray{});
	
	constexpr Equality(void) = default;
	
	constexpr Equality(T1 t1, T2 t2)
			noexcept(std::is_nothrow_move_constructible_v<T1> && std::is_nothrow_move_constructible_v<T2>) :
			Term1{std::move(t1)}, Term2{std::move(t2)} {
		return;
	}
	
	constexpr auto simplified(void) const { return *this; }
	
	constexpr auto negate(void) const { return Not{*this}; }
	
	constexpr auto toNegationNormalForm(void) const { return *this; }
	
	friend std::ostream& operator<<(std::ostream& os, const Equality& e) {
		return os<<e.Term1<<" = "<<e.Term2;
	}
};

template<typename T1, typename T2>
Equality(T1, T2) -> Equality<T1, T2>;

template<typename T11, typename T12, typename T21, typename T22>
constexpr bool operator==(const Equality<T11, T12>& e1, const Equality<T21, T22>& e2) noexcept {
	return e1.Term1 == e2.Term1 && e1.Term2 == e2.Term2;
}

template<typename T11, typename T12, typename T21, typename T22>
constexpr bool operator!=(const Equality<T11, T12>& e1, const Equality<T21, T22>& e2) noexcept {
	return !(e1 == e2);
}

template<typename T1, typename T2>
struct PrettyPrinter<Equality<T1, T2>> {
	const Equality<T1, T2>& E;
	const int Index;
	
	PrettyPrinter(const Equality<T1, T2>& e, int index = -1) : E{e}, Index{index} {
		return;
	}
	
	std::ostream& prettyPrint(std::ostream& os) const {
		const bool withParanthesis = Index != -1;
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].first;
		} //if ( withParanthesis )
		os<<E;
		if ( withParanthesis ) {
			os<<PrettyParanthesis[static_cast<std::size_t>(Index)].second;
		} //if ( withParanthesis )
		return os;
	}
};

} //namespace fol

#endif
