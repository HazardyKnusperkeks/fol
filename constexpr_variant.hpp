/**
 * @file
 * @brief Contains a simple constexpr variant.
 * 
 * This implementation is based on GCC's std::variant implementation.
 */

#ifndef CONSTEXPR_VARIANT_HPP
#define CONSTEXPR_VARIANT_HPP

#include <exception>
#include <type_traits>

template<typename... Types>
class ConstexprVariant;

namespace details {
template<typename T>
struct IsVariant : std::false_type { };

template<typename... Types>
struct IsVariant<ConstexprVariant<Types...>> : std::true_type { };

template<typename T, typename... Types>
struct IndexOf : std::integral_constant<std::size_t, 0> {};

template<typename T, typename... Types>
inline constexpr size_t IndexOfV = IndexOf<T, Types...>::value;

template<typename T, typename Head, typename... Tail>
struct IndexOf<T, Head, Tail...> : std::integral_constant<size_t,
	std::is_same_v<std::decay_t<T>, Head> ? 0 : IndexOfV<T, Tail...> + 1> { };

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
	{ return *reinterpret_cast<const Type*>(Storage); }
	
	Type& get() &
	{ return *reinterpret_cast<Type*>(Storage); }
	
	const Type&& get() const &&
	{ return std::move(*reinterpret_cast<const Type*>(Storage)); }
	
	Type&& get() &&
	{ return std::move(*reinterpret_cast<Type*>(Storage)); }
	
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
};

template<std::size_t I, typename... Types>
constexpr decltype(auto) get(const std::in_place_index_t<I>, const VarUnion<Types...>& v) {
	if constexpr ( I == 0 ) {
		return v.First.get();
	}
	else {
		return get(std::in_place_index<I-1>, v.Rest);
	}
}

template<typename Variant, std::size_t Idx>
constexpr void erasedDtor(Variant&& v) {
	auto&& element = get(std::in_place_index<Idx>, std::forward<Variant>(v).U);
	using Type = std::remove_reference_t<decltype(element)>;
	element.~Type();
	return;
}

template<bool triviallyDestructible, typename... Types>
struct VariantStorage;

template<typename... Types>
struct VariantStorage<false, Types...> {
	VarUnion<Types...> U;
	std::size_t Index = sizeof...(Types);
	
	template<std::size_t... Idx>
	static constexpr void (*Vtable[])(const VariantStorage&) = { &erasedDtor<const VariantStorage&, Idx>... };
	
	constexpr VariantStorage(void) = default;
	
	template<std::size_t Idx, typename... Args>
	constexpr VariantStorage(const std::in_place_index_t<Idx>, Args&&... args) : U{std::in_place_index<Idx>,
	                                                                               std::forward<Args>(args)...},
	                                                                             Index{Idx} {
		return;
	}
	
	constexpr VariantStorage(VariantStorage&& v) : U{std::move(v.U)}, Index{v.Index} {
		v.Index = sizeof...(Types);
		return;
	}
	
	constexpr VariantStorage& operator=(VariantStorage&& v) {
		using std::swap;
		swap(U,     v.U);
		swap(Index, v.Index);
		return *this;
	}
	
	template<std::size_t... Idx>
	constexpr void resetImpl(const std::index_sequence<Idx...>) {
		if ( Index != sizeof...(Types) ) {
			Vtable<Idx...>[Index](*this);
		} //if ( Index != sizeof...(Types) )
		return;
	}
	
	constexpr void reset(void) {
		resetImpl(std::index_sequence_for<Types...>{});
		Index = sizeof...(Types);
		return;
	}
	
	~VariantStorage(void) {
		reset();
		return;
	}
};

template<typename... Types>
struct VariantStorage<true, Types...> {
	VarUnion<Types...> U;
	std::size_t Index = sizeof...(Types);
	
	constexpr VariantStorage(void) = default;
	
	template<std::size_t Idx, typename... Args>
	constexpr VariantStorage(const std::in_place_index_t<Idx>, Args&&... args) : U{std::in_place_index<Idx>,
	                                                                               std::forward<Args>(args)...},
	                                                                             Index{Idx} {
		return;
	}
	
	void reset(void) {
		Index = sizeof...(Types);
		return;
	}
};

struct EmptyUnion { };

template<typename Type1, typename Type2, std::enable_if_t<std::negation_v<std::is_same<Type1, Type2>>>* = nullptr>
constexpr bool compare(const Type1&, const Type2&) noexcept {
	return false;
}

template<typename Type>
constexpr bool compare(const Type& t1, const Type& t2) noexcept {
	return t1 == t2;
}

template<typename... Types2>
constexpr bool compare(const EmptyUnion, std::size_t index2, const VarUnion<Types2...>& u2) noexcept {
	if constexpr ( sizeof...(Types2) == 0 ) {
		//Two default constructed variants.
		return true;
	} //if constexpr ( sizeof...(Types2) == 0 )
	else {
		if ( index2 == 0 ) {
			//One default constructed variant, and one that is not.
			return false;
		} //if ( index2 == 0 )
		
		return compare(EmptyUnion{}, index2 - 1, u2.Rest);
	} //else -> if constexpr ( sizeof...(Types2) == 0 )
}

template<typename Type1, typename... Types2>
constexpr bool compare(const Type1& t, const std::size_t index2, const VarUnion<Types2...>& u2) noexcept {
	if constexpr ( sizeof...(Types2) == 0 ) {
		//One default constructed variant, and one that is not.
		return false;
	} //if constexpr ( sizeof...(Types2) == 0 )
	else {
		if ( index2 == 0 ) {
			return compare(t, u2.First.Storage);
		} //if ( index2 == 0 )
		
		return compare(t, index2 - 1, u2.Rest);
	} //else -> if constexpr ( sizeof...(Types2) == 0 )
}

template<typename... Types1, typename... Types2>
constexpr bool compare(const std::size_t index1, const VarUnion<Types1...>& u1,
                       const std::size_t index2, const VarUnion<Types2...>& u2) noexcept {
	if constexpr ( sizeof...(Types1) == 0 ) {
		return compare(EmptyUnion{}, index2, u2);
	} //if constexpr ( sizeof...(Types1) == 0 )
	else {
		if ( index1 == 0 ) {
			return compare(u1.First.Storage, index2, u2);
		} //if ( index1 == 0 )
		
		return compare(index1 - 1, u1.Rest, index2, u2);
	} //else -> if constexpr ( sizeof...(Types1) == 0 )
}

template<typename SetType>
constexpr void setVariant(std::size_t&, VarUnion<>&, const SetType&, const std::size_t = 0) {
	throw std::logic_error{"setVariant types do not match!"};
}

template<typename Type, typename... Types, typename SetType>
constexpr void setVariant(std::size_t& index, VarUnion<Type, Types...>& u, const SetType& t,
                          const std::size_t counter = 0) {
	if constexpr ( std::is_same_v<Type, SetType> ) {
		index = counter;
		u.First.Storage = t;
	} //if constexpr ( std::is_same_v<Type, SetType> )
	else {
		setVariant(index, u.Rest, t, counter + 1);
	} //else -> if constexpr ( std::is_same_v<Type, SetType> )
	return;
}

template<typename... Types1, typename... Types2>
constexpr void setVariant(std::size_t& destIndex, VarUnion<Types1...>& destU,
                          const std::size_t srcIndex, const VarUnion<Types2...>& srcU) {
	if constexpr ( sizeof...(Types2) == 0 ) {
		//Nothing! Never called!
		throw std::logic_error{"Called setVariant from an empty union!"};
	} //if constexpr ( sizeof...(Types2) == 0 )
	else {
		if ( srcIndex == 0 ) {
			setVariant(destIndex, destU, srcU.First.Storage);
		} //if ( srcIndex == 0 )
		else {
			setVariant(destIndex, destU, srcIndex - 1, srcU.Rest);
		} //else -> if ( srcIndex == 0 )
	} //if constexpr ( sizeof...(Types2) == 0 )
	return;
}

} //namespace details

template<typename... Types>
class ConstexprVariant : protected details::VariantStorage<(std::is_trivially_destructible_v<Types> && ...), Types...> {
	using Base = typename details::VariantStorage<(std::is_trivially_destructible_v<Types> && ...), Types...>;
	
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
	constexpr ConstexprVariant(void) = default;
	
	template<typename T, std::enable_if_t<!details::IsVariant<std::decay_t<T>>::value>* = nullptr>
	constexpr ConstexprVariant(T&& t) : Base{std::in_place_index<details::IndexOfV<T, Types...>>, std::forward<T>(t)} {
		return;
	}
	
	constexpr std::size_t index(void) const noexcept {
		return Base::Index;
	}
	
	template<std::size_t I>
	constexpr decltype(auto) get(void) const {
		return details::get(std::in_place_index<I>, Base::U);
	}
	
	template<typename T>
	constexpr decltype(auto) get(void) const {
		return get<details::IndexOfV<T, Types...>>();
	}
	
	template<typename... Types2>
	constexpr operator ConstexprVariant<Types2...>(void) const {
		ConstexprVariant<Types2...> ret;
		if ( index() != sizeof...(Types) ) {
			details::setVariant(ret.Index, ret.U, index(), Base::U);
		} //if ( index() != sizeof...(Types) )
		return ret;
	}
	
	friend constexpr bool operator==(const ConstexprVariant& lhs, const ConstexprVariant& rhs) noexcept {
		return lhs.Index == rhs.Index && ConstexprVariant::compare(lhs.Index, lhs.U, rhs.U);
	}
	
	friend constexpr bool operator!=(const ConstexprVariant& lhs, const ConstexprVariant& rhs) noexcept {
		return !(lhs == rhs);
	}
	
	template<typename... Types1, typename... Types2>
	friend constexpr bool operator==(const ConstexprVariant<Types1...>& lhs, const ConstexprVariant<Types2...>& rhs) noexcept;
	
	template<typename... Types1, typename... Types2>
	friend constexpr bool details::compare(const std::size_t, const ConstexprVariant<Types1...>&,
	                                       const std::size_t, const ConstexprVariant<Types2...>&) noexcept;
	
	template<typename... Types2>
	friend constexpr bool operator!=(const ConstexprVariant<Types...>& lhs, const ConstexprVariant<Types2...>& rhs) noexcept {
		return !(lhs == rhs);
	}
	
	template<typename... Types2>
	friend class ConstexprVariant;
};

template<typename... Types1, typename... Types2>
constexpr bool operator==(const ConstexprVariant<Types1...>& lhs, const ConstexprVariant<Types2...>& rhs) noexcept {
	return details::compare(lhs.Index, lhs.U, rhs.Index, rhs.U);
}

namespace tests {
static_assert(ConstexprVariant<int, char>{7}   == ConstexprVariant<int, char>{7});
static_assert(ConstexprVariant<int, char>{'7'} == ConstexprVariant<int, char>{'7'});
static_assert(ConstexprVariant<int, char>{7}   != ConstexprVariant<int, char>{'7'});
static_assert(ConstexprVariant<int, char>{7}   == ConstexprVariant<char, int>{7});
static_assert(ConstexprVariant<int, char>{'7'} == ConstexprVariant<char, int>{'7'});
static_assert(ConstexprVariant<int, char>{'7'} != ConstexprVariant<char, int>{7});
static_assert(ConstexprVariant<int, char>{'7'} != ConstexprVariant<char, int>{});
static_assert(ConstexprVariant<int, char>{}    == ConstexprVariant<char, int>{});
static_assert(ConstexprVariant<char, int>{'7'} == ConstexprVariant<int, char>{'7'});
static_assert(ConstexprVariant<char, int>{'7'} != ConstexprVariant<int, char>{7});
static_assert(ConstexprVariant<char, int>{'7'} != ConstexprVariant<int, char>{});
static_assert(ConstexprVariant<char, int>{}    == ConstexprVariant<int, char>{});
} //namespace tests


#endif
