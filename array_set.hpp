/**
 * @file
 * @brief Contains the ArraySet.
 */

#ifndef ARRAY_SET_HPP
#define ARRAY_SET_HPP

#include "constexpr_algorithm.hpp"
#include "constexpr_variant.hpp"

#include <array>

template<std::size_t N, typename... Types>
class ArraySet {
	std::array<ConstexprVariant<Types...>, N> Data;
	std::size_t Index = 0;
	
	public:
	constexpr ArraySet(void) = default;
	
	template<typename... Args>
	constexpr ArraySet(Args&&... args) : Data{std::forward<Args>(args)...}, Index{sizeof...(Args)} {
		static_assert(sizeof...(Args) <= N, "More initializers than elements in the set allowed!");
		return;
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
}

#endif
