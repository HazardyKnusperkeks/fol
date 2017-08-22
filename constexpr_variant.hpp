/**
 * @file
 * @brief Contains a simple constexpr variant.
 * 
 * This implementation is based on GCC's std::variant implementation.
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

template<typename Type, bool = std::is_literal_type_v<Type>>
struct Uninitialized;

template<typename Type>
struct Uninitialized<Type, true> {
	Type Storage;
	
	template<typename... Args>
	constexpr Uninitialized(Args&&... args) : Storage(std::forward<Args>(args)...) {
		return;
	}
	
	constexpr const Type& get(void) const & { return Storage; }
	
	constexpr Type& get(void) & { return Storage; }
	
	constexpr const Type&& get(void) const && { return std::move(Storage); }
	
	constexpr Type&& get(void) && { return std::move(Storage); }
};

template<typename Type>
struct Uninitialized<Type, false> {
	template<typename... Args>
	constexpr Uninitialized(Args&&... args) {
		::new (&Storage) Type(std::forward<Args>(args)...);
		return;
	}
	
	const Type& get() const &
	{ return *Storage._M_ptr(); }
	
	Type& get() &
	{ return *Storage._M_ptr(); }
	
	const Type&& get() const &&
	{ return std::move(*Storage._M_ptr()); }
	
	Type&& get() &&
	{ return std::move(*Storage._M_ptr()); }
	
	unsigned char Storage[sizeof(Type)];
};

template<typename... Types>
union VarUnion { };

template<typename Head, typename... Tail>
union VarUnion<Head, Tail...> {
	Uninitialized<Head> First;
	VarUnion<Tail...> Rest;
	
	constexpr VarUnion(void) : Rest{} {
		return;
	}
	
	template<typename... Args>
	constexpr VarUnion(const std::in_place_index_t<0>, Args&&... args) : First(std::forward<Args>(args)...) {
		return;
	}
	
	template<std::size_t I, typename... Args>
	constexpr VarUnion(const std::in_place_index_t<I>, Args&&... args) :
			Rest(std::in_place_index<I-1>, std::forward<Args>(args)...) {
		return;
	}
	
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
	
	template<typename... Ts>
	static constexpr bool compare(const std::size_t index, const details::VarUnion<Ts...>& u1, const details::VarUnion<Ts...>& u2) noexcept {
		if constexpr ( sizeof...(Ts) == 0 ) {
			//Comparing two default constructed variants.
			return true;
		} //if constexpr ( sizeof...(Ts) == 0 )
		else {
			if ( index == 0 ) {
				return u1.First.Storage == u2.First.Storage;
			} //if ( index == 0 )
			return compare(index - 1, u1.Rest, u2.Rest);
		} //else -> if constexpr ( sizeof...(Ts) == 0 )
	}
	
	public:
	constexpr ConstexprVariant(void) noexcept : U{} {
		return;
	}
	
	template<typename T>
	constexpr ConstexprVariant(T&& t) : U{std::in_place_index<details::IndexOfV<T, Types...>>, std::forward<T>(t)},
	                                    Index{details::IndexOfV<T, Types...>} {
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
	
	friend constexpr bool operator==(const ConstexprVariant& lhs, const ConstexprVariant& rhs) noexcept {
		return lhs.Index == rhs.Index && ConstexprVariant::compare(lhs.Index, lhs.U, rhs.U);
	}
	
	friend constexpr bool operator!=(const ConstexprVariant& lhs, const ConstexprVariant& rhs) noexcept {
		return !(lhs == rhs);
	}
};

#endif
