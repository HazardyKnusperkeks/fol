/**
 * @file
 * @brief Contains a simple constexpr variant.
 */

#ifndef CONSTEXPR_VARIANT_HPP
#define CONSTEXPR_VARIANT_HPP

#include <type_traits>

namespace details {
template<typename T, typename... Types>
struct IndexOf : std::integral_constant<std::size_t, 0> {};

template<typename T, typename... Types>
inline constexpr size_t IndexOfV = IndexOf<T, Types...>::value;

template<typename T, typename Head, typename... Tail>
struct IndexOf<T, Head, Tail...> : std::integral_constant<size_t,
                                                          std::is_same_v<T, Head> ? 0 : IndexOfV<T, Tail...> + 1> { };

template<typename... Types>
union VarUnion { };

template<typename Head, typename... Tail>
union VarUnion<Head, Tail...> {
	Head First;
	VarUnion<Tail...> Rest;
	
	constexpr VarUnion(void) = default;
	
	template<typename... Args>
	constexpr VarUnion(const std::in_place_index_t<0>, Args&&... args) : First(std::forward<Args>(args)...) {
		return;
	}
	
	template<std::size_t I, typename... Args>
	constexpr VarUnion(const std::in_place_index_t<I>, Args&&... args) :
		Rest(std::in_place_index<I-1>, std::forward<Args>(args)...) { }
	
	template<std::size_t I>
	constexpr auto get(void) const {
		return 0;
}
};

template<std::size_t I, typename... Types>
constexpr auto get(const std::in_place_index_t<I>, const VarUnion<Types...>& v) {
	if constexpr ( I == 0 ) {
		return v.First;
	}
	else {
		return get(std::in_place_index<I-1>, v.Rest);
	}
}
}

template<typename... Types>
class ConstexprVariant {
	details::VarUnion<Types...> U;
	std::size_t Index = sizeof...(Types);
	
	public:
	constexpr ConstexprVariant(void) : U() {
		return;
	}
	
	template<typename T>
	constexpr ConstexprVariant(T&& t) : U(std::in_place_index<details::IndexOfV<T, Types...>>, std::forward<T>(t)),
	                                    Index(details::IndexOfV<T, Types...>) {
		return;
	}
	
	constexpr std::size_t index(void) const noexcept {
		return Index;
	}
	
	template<std::size_t I>
	constexpr auto get(void) const {
		return details::get(std::in_place_index<I>, U);
	}
	
	template<typename T>
	constexpr T get(void) const {
		return get<details::IndexOfV<T, Types...>>();
	}
};

#endif
