/**
 * @file
 * @brief Contains the ArraySet.
 */

#ifndef ARRAY_SET_HPP
#define ARRAY_SET_HPP

#include "constexpr_algorithm.hpp"
#include "constexpr_variant.hpp"

#include <array>
#include <type_traits>

template<std::size_t N, typename... Types>
class ArraySet {
	std::array<ConstexprVariant<Types...>, N> Data;
	std::size_t Index = 0;
	const decltype(Data.begin()) Begin = Data.begin();
	decltype(Data.begin()) End = Data.begin();
	
	constexpr void makeUnique(void) {
		auto iter = Begin;
		constexprAlgo::advance(iter, 1);
		while ( iter != End ) {
			if ( constexprAlgo::find(Begin, iter, *iter) != iter ) {
				//Found a duplicate!
				--End;
				--Index;
				*iter = std::move(*End);
			} //if ( constexprAlgo::find(Begin, iter, *iter) != iter )
			else {
				++iter;
			} //else -> if ( constexprAlgo::find(Begin, iter, *iter) != iter )
		} //while ( iter != End )
		return;
	}
	
	public:
	constexpr ArraySet(void) = default;
	
	template<typename... Args,
	         std::enable_if_t<(std::negation_v<std::is_same<std::decay_t<Args>, ArraySet>> && ...)>* = nullptr>
	constexpr ArraySet(Args&&... args) : Data{std::forward<Args>(args)...}, Index{sizeof...(Args)} {
		static_assert(sizeof...(Args) <= N, "More initializers than elements in the set allowed!");
		constexprAlgo::advance(End, static_cast<std::make_signed_t<decltype(Index)>>(Index));
		makeUnique();
		return;
	}
	
	template<typename Type, std::enable_if_t<(std::is_same_v<Type, Types> || ...)>* = nullptr>
	constexpr bool contains(const Type& t) const noexcept {
		return constexprAlgo::find(Begin, End, t) != End;
	}
	
	template<std::size_t N2>
	constexpr bool operator==(const ArraySet<N2, Types...>& rhs) const noexcept {
		return constexprAlgo::is_permutation(Begin, End, rhs.Begin, rhs.End);
	}
	
	template<std::size_t N2>
	constexpr bool operator!=(const ArraySet<N2, Types...>& rhs) const noexcept {
		return !operator==(rhs);
	}
};

namespace tests {
using SetType = ArraySet<10, int, bool, char>;

static_assert(SetType{} == SetType{});
static_assert(SetType{5, true, 7, 'h'}.contains(true));
static_assert(SetType{5, true, 7, 'h'}.contains('h'));
static_assert(SetType{5, true, 7, 'h'}.contains(5));
static_assert(!SetType{5, true, 7, 'h'}.contains(false));
static_assert(!SetType{5, true, 7, 'h'}.contains(6));
static_assert(!SetType{5, true, 7, 'h'}.contains('c'));
static_assert(SetType{5, true, 7, 'h'} == SetType{5, true, 7, 'h'});
static_assert(SetType{5, true, 7, 'h'} == SetType{7, true, 5, 'h'});
static_assert(SetType{5, true, 7, 'h'} == SetType{true, 7, 'h', 5});
static_assert(SetType{5, true, 7, 'h'} == SetType{5, 'h', true, 7});

}

#endif
