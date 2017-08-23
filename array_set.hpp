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
	
	public:
	constexpr ArraySet(void) = default;
	
	template<typename... Args>
	constexpr ArraySet(Args&&... args) : Data{std::forward<Args>(args)...}, Index{sizeof...(Args)} {
		static_assert(sizeof...(Args) <= N, "More initializers than elements in the set allowed!");
		constexprAlgo::advance(End, static_cast<std::make_signed_t<decltype(Index)>>(Index));
		return;
	}
	
	template<typename Type, std::enable_if_t<(std::is_same_v<Type, Types> || ...)>* = nullptr>
	constexpr bool contains(const Type& t) const noexcept {
		return constexprAlgo::find(Begin, End, t) != End;
	}
	
	template<std::size_t N2>
	constexpr bool operator==(const ArraySet<N2, Types...>& rhs) const noexcept {
		return constexprAlgo::equal(Data.begin(), Data.begin() + Index, rhs.Data.begin(), rhs.Data.begin() + rhs.Index);
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

}

#endif
