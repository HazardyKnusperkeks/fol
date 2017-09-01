/**
 * @file
 * @brief Contains the ArraySet.
 */

#ifndef ARRAY_SET_HPP
#define ARRAY_SET_HPP

#include "constexpr_algorithm.hpp"
#include "constexpr_iterator.hpp"
#include "constexpr_variant.hpp"

#include <array>
#include <type_traits>
#include <utility>

template<std::size_t N, typename... Types>
class ArraySet;

namespace details {
template<typename... Types>
struct TypeHolder { };

template<std::size_t N, typename... Types>
constexpr ArraySet<N, Types...> toArraySet(const std::integral_constant<std::size_t, N>,
                                           const TypeHolder<Types...>) noexcept {
	return {};
}

template<typename... Types>
constexpr TypeHolder<Types...> getCommonType(const TypeHolder<Types...>, const TypeHolder<>) noexcept {
	return {};
}

template<typename... Types1, typename Type, typename... Types2>
constexpr auto getCommonType(const TypeHolder<Types1...>, const TypeHolder<Type, Types2...>) noexcept {
	if constexpr ( (std::is_same_v<Type, Types1> || ... ) ) {
		return getCommonType(TypeHolder<Types1...>{}, TypeHolder<Types2...>{});
	} //if constexpr ( (std::is_same_v<Type, Types1> || ... ) )
	else {
		return getCommonType(TypeHolder<Types1..., Type>{}, TypeHolder<Types2...>{});
	} //else -> if constexpr ( (std::is_same_v<Type, Types1> || ... ) )
}

template<std::size_t N1, typename... Types1, std::size_t N2, typename... Types2>
constexpr auto getCommonType(const ArraySet<N1, Types1...>&, const ArraySet<N2, Types2...>&) noexcept {
	return toArraySet(std::integral_constant<std::size_t, N1 + N2>{},
	                  getCommonType(TypeHolder<Types1...>{}, TypeHolder<Types2...>{}));
}
} //namespace details

template<std::size_t N, typename... Types>
class ArraySet {
	std::array<ConstexprVariant<Types...>, N> Data;
	std::size_t Index = 0;
	const decltype(Data.begin()) Begin = Data.begin();
	decltype(Data.begin()) End = Data.begin();
	
	constexpr void makeUnique(void) {
		auto iter = Begin;
		constexprStd::advance(iter, 1);
		while ( iter != End ) {
			if ( constexprStd::find(Begin, iter, *iter) != iter ) {
				//Found a duplicate!
				--End;
				--Index;
				*iter = std::move(*End);
			} //if ( constexprStd::find(Begin, iter, *iter) != iter )
			else {
				++iter;
			} //else -> if ( constexprStd::find(Begin, iter, *iter) != iter )
		} //while ( iter != End )
		return;
	}
	
	public:
	class iterator {
		private:
		using Base = typename std::array<ConstexprVariant<Types...>, N>::iterator;
		Base Iter;
		
		public:
		using difference_type   = typename std::iterator_traits<Base>::difference_type;
		using value_type        = typename std::iterator_traits<Base>::value_type;
		using pointer           = typename std::iterator_traits<Base>::pointer;
		using reference         = typename std::iterator_traits<Base>::reference;
		using iterator_category = typename std::iterator_traits<Base>::iterator_category;
		
		constexpr iterator(void) noexcept(std::is_nothrow_default_constructible_v<Base>) = default;
		
		constexpr iterator(Base iter) noexcept(std::is_nothrow_move_constructible_v<Base>) :
				Iter{std::move(iter)} {
			return;
		}
		
		constexpr reference operator*(void) const noexcept(noexcept(*Iter)) {
			return *Iter;
		}
		
		constexpr pointer operator->(void) const noexcept(noexcept(std::declval<iterator>().operator*())) {
			return std::addressof(operator*());
		}
		
		constexpr reference operator[](const difference_type n) const
				noexcept(noexcept(std::declval<iterator>() + n) && noexcept(std::declval<iterator>().operator*())) {
			return *(*this + n);
		}
		
		constexpr iterator& operator++(void) noexcept(noexcept(++Iter)) {
			++Iter;
			return *this;
		}
		
		constexpr iterator operator++(const int) noexcept(std::is_nothrow_copy_constructible_v<iterator> &&
		                                                  noexcept(++std::declval<iterator>())) {
			iterator copy{*this};
			++*this;
			return copy;
		}
		
		constexpr iterator& operator--(void) noexcept(noexcept(--Iter)) {
			--Iter;
			return *this;
		}
		
		constexpr iterator operator--(const int) noexcept(std::is_nothrow_copy_constructible_v<iterator> &&
		                                                  noexcept(--std::declval<iterator>())) {
			iterator copy{*this};
			--*this;
			return copy;
		}
		
		constexpr iterator& operator+=(const difference_type n) noexcept(noexcept(Iter += n)) {
			Iter += n;
			return *this;
		}
		
		constexpr iterator& operator-=(const difference_type n) noexcept(noexcept(Iter -= n)) {
			Iter -= n;
			return *this;
		}
		
		constexpr iterator operator+(const difference_type n) const
				noexcept(noexcept(Iter + n) && std::is_nothrow_constructible_v<iterator, decltype(Iter + n)>) {
			return Iter + n;
		}
		
		constexpr iterator operator-(const difference_type n) const
				noexcept(noexcept(Iter - n) && std::is_nothrow_constructible_v<iterator, decltype(Iter - n)>) {
			return Iter - n;
		}
		
		friend constexpr iterator operator+(const difference_type n, iterator iter) noexcept(noexcept(iter += n)) {
			return iter += n;
		}
		
		constexpr difference_type operator-(const iterator& iter) const noexcept(noexcept(Iter - iter.Iter)) {
			return Iter - iter.Iter;
		}
		
		friend constexpr bool operator==(const iterator& iter1, const iterator& iter2)
				noexcept(noexcept(iter1.Iter == iter2.Iter)) {
			return iter1.Iter == iter2.Iter;
		}
		
		friend constexpr bool operator!=(const iterator& iter1, const iterator& iter2)
				noexcept(noexcept(iter1 == iter2)) {
			return !(iter1 == iter2);
		}
		
		friend constexpr bool operator<(const iterator& iter1, const iterator& iter2)
				noexcept(noexcept(iter1.Iter < iter2.Iter)) {
			return iter1.Iter < iter2.Iter;
		}
		
		friend constexpr bool operator<=(const iterator& iter1, const iterator& iter2)
				noexcept(noexcept(iter1 > iter2)) {
			return !(iter1.Iter > iter2.Iter);
		}
		
		friend constexpr bool operator>(const iterator& iter1, const iterator& iter2)
				noexcept(noexcept(iter1.Iter > iter2.Iter)) {
			return iter1.Iter > iter2.Iter;
		}
		
		friend constexpr bool operator>=(const iterator& iter1, const iterator& iter2)
				noexcept(noexcept(iter1 < iter2)) {
			return !(iter1.Iter < iter2.Iter);
		}
		
		friend class const_iterator;
	};
	
	class const_iterator {
		private:
		using Base = typename std::array<ConstexprVariant<Types...>, N>::const_iterator;
		Base Iter;
		
		public:
		using difference_type   = typename std::iterator_traits<Base>::difference_type;
		using value_type        = typename std::iterator_traits<Base>::value_type;
		using pointer           = typename std::iterator_traits<Base>::pointer;
		using reference         = typename std::iterator_traits<Base>::reference;
		using iterator_category = typename std::iterator_traits<Base>::iterator_category;
		
		constexpr const_iterator(void) noexcept(std::is_nothrow_default_constructible_v<Base>) = default;
		
		constexpr const_iterator(Base iter) noexcept(std::is_nothrow_move_constructible_v<Base>) :
				Iter{std::move(iter)} {
			return;
		}
		
		constexpr const_iterator(const iterator iter)
				noexcept(std::is_nothrow_constructible_v<Base, iterator::Base>) : Iter{iter.Iter} {
			return;
		}
		
		constexpr const_iterator(iterator&& iter)
				noexcept(std::is_nothrow_constructible_v<Base, std::add_rvalue_reference_t<typename iterator::Base>>) :
				Iter{std::move(iter.Iter)} {
			return;
		}
		
		constexpr reference operator*(void) const noexcept(noexcept(*Iter)) {
			return *Iter;
		}
		
		constexpr pointer operator->(void) const noexcept(noexcept(std::declval<const_iterator>().operator*())) {
			return std::addressof(operator*());
		}
		
		constexpr reference operator[](const difference_type n) const
		noexcept(noexcept(std::declval<iterator>() + n) && noexcept(std::declval<const_iterator>().operator*())) {
			return *(*this + n);
		}
		
		constexpr const_iterator& operator++(void) noexcept(noexcept(++Iter)) {
			++Iter;
			return *this;
		}
		
		constexpr const_iterator operator++(const int) noexcept(std::is_nothrow_copy_constructible_v<const_iterator> &&
		                                                        noexcept(++std::declval<const_iterator>())) {
			const_iterator copy{*this};
			++*this;
			return copy;
		}
		
		constexpr const_iterator& operator--(void) noexcept(noexcept(--Iter)) {
			--Iter;
			return *this;
		}
		
		constexpr const_iterator operator--(const int) noexcept(std::is_nothrow_copy_constructible_v<const_iterator> &&
		                                                        noexcept(--std::declval<const_iterator>())) {
			const_iterator copy{*this};
			--*this;
			return copy;
		}
		
		constexpr const_iterator& operator+=(const difference_type n) noexcept(noexcept(Iter += n)) {
			Iter += n;
			return *this;
		}
		
		constexpr const_iterator& operator-=(const difference_type n) noexcept(noexcept(Iter -= n)) {
			Iter -= n;
			return *this;
		}
		
		constexpr const_iterator operator+(const difference_type n) const
				noexcept(noexcept(Iter + n) && std::is_nothrow_constructible_v<const_iterator, decltype(Iter + n)>) {
			return Iter + n;
		}
		
		constexpr const_iterator operator-(const difference_type n) const
				noexcept(noexcept(Iter - n) && std::is_nothrow_constructible_v<const_iterator, decltype(Iter - n)>) {
			return Iter - n;
		}
		
		friend constexpr const_iterator operator+(const difference_type n, const_iterator iter)
				noexcept(noexcept(iter += n)) {
			return iter += n;
		}
		
		constexpr difference_type operator-(const const_iterator& iter) const noexcept(noexcept(Iter - iter.Iter)) {
			return Iter - iter.Iter;
		}
		
		friend constexpr bool operator==(const const_iterator& iter1, const const_iterator& iter2)
				noexcept(noexcept(iter1.Iter == iter2.Iter)) {
			return iter1.Iter == iter2.Iter;
		}
		
		friend constexpr bool operator!=(const const_iterator& iter1, const const_iterator& iter2)
				noexcept(noexcept(iter1 == iter2)) {
			return !(iter1 == iter2);
		}
		
		friend constexpr bool operator<(const const_iterator& iter1, const const_iterator& iter2)
				noexcept(noexcept(iter1.Iter < iter2.Iter)) {
			return iter1.Iter < iter2.Iter;
		}
		
		friend constexpr bool operator<=(const const_iterator& iter1, const const_iterator& iter2)
				noexcept(noexcept(iter1 > iter2)) {
			return !(iter1.Iter > iter2.Iter);
		}
		
		friend constexpr bool operator>(const const_iterator& iter1, const const_iterator& iter2)
				noexcept(noexcept(iter1.Iter > iter2.Iter)) {
			return iter1.Iter > iter2.Iter;
		}
		
		friend constexpr bool operator>=(const const_iterator& iter1, const const_iterator& iter2)
				noexcept(noexcept(iter1 < iter2)) {
			return !(iter1.Iter < iter2.Iter);
		}
	};
	
	using value_type = typename decltype(Data)::value_type;
	
	constexpr ArraySet(void) = default;
	
	constexpr ArraySet(const ArraySet& a) noexcept(std::is_nothrow_copy_constructible_v<decltype(Data)>) :
			Data{a.Data}, Index{a.Index} {
		constexprStd::advance(End, static_cast<std::make_signed_t<decltype(Index)>>(Index));
		return;
	}
	
	constexpr ArraySet(ArraySet&& a) noexcept(std::is_nothrow_move_constructible_v<decltype(Data)>) :
			Data{std::move(a.Data)}, Index{constexprStd::exchange(a.Index, 0u)} {
		constexprStd::advance(End, static_cast<std::make_signed_t<decltype(Index)>>(Index));
		return;
	}
	
	constexpr ArraySet& operator=(const ArraySet& a) noexcept(std::is_nothrow_copy_constructible_v<ArraySet>) {
		ArraySet copy(a);
		using std::swap;
		swap(copy, *this);
		return *this;
	}
	
	constexpr ArraySet& operator=(ArraySet&& a) noexcept(std::is_nothrow_move_assignable_v<decltype(Data)>) {
		Data  = std::move(a.Data);
		Index = constexprStd::exchange(a.Index, 0u);
		a.End = a.Begin;
		End   = Begin;
		constexprStd::advance(End, static_cast<std::make_signed_t<decltype(Index)>>(Index));
		return *this;
	}
	
	template<typename... Args,
	         std::enable_if_t<(std::negation_v<std::is_same<std::decay_t<Args>, ArraySet>> && ...)>* = nullptr>
	constexpr ArraySet(Args&&... args) : Data{std::forward<Args>(args)...}, Index{sizeof...(Args)} {
		static_assert(sizeof...(Args) <= N, "More initializers than elements in the set allowed!");
		constexprStd::advance(End, static_cast<std::make_signed_t<decltype(Index)>>(Index));
		makeUnique();
		return;
	}
	
	constexpr iterator begin(void) noexcept(std::is_nothrow_constructible_v<iterator, decltype(Begin)>) {
		return Begin;
	}
	
	constexpr iterator end(void) noexcept(std::is_nothrow_constructible_v<iterator, decltype(End)>) {
		return End;
	}
	
	constexpr const_iterator begin(void) const
			noexcept(std::is_nothrow_constructible_v<const_iterator, decltype(Begin)>) {
		return Begin;
	}
	
	constexpr const_iterator end(void) const noexcept(std::is_nothrow_constructible_v<const_iterator, decltype(End)>) {
		return End;
	}
	
	constexpr const_iterator cbegin(void) const
			noexcept(std::is_nothrow_constructible_v<const_iterator, decltype(Begin)>) {
		return Begin;
	}
	
	constexpr const_iterator cend(void) const noexcept(std::is_nothrow_constructible_v<const_iterator, decltype(End)>) {
		return End;
	}
	
	constexpr std::size_t size(void) const noexcept {
		return Index;
	}
	
	constexpr bool empty(void) const noexcept {
		return Index == 0;
	}
	
	template<typename Type, std::enable_if_t<std::is_same_v<std::decay_t<Type>, value_type> ||
	                                         (std::is_same_v<std::decay_t<Type>, Types> || ...)>* = nullptr>
	constexpr bool contains(const Type& t) const noexcept {
		return constexprStd::find(Begin, End, t) != End;
	}
	
	template<typename Type, std::enable_if_t<std::is_same_v<std::decay_t<Type>, value_type> ||
	                                         (std::is_same_v<std::decay_t<Type>, Types> || ...)>* = nullptr>
	constexpr ArraySet& insert(Type&& t) {
		if ( !contains(t) ) {
			if ( Index++ == N ) {
				throw std::out_of_range("ArraySet: Trying to insert more values than configured!");
			} //if ( Index++ == N )
			*End = std::forward<Type>(t);
			++End;
		} //if ( !contains(t) )
		return *this;
	}
	
	template<typename Type, std::enable_if_t<std::is_same_v<std::decay_t<Type>, value_type> ||
	                                         (std::is_same_v<std::decay_t<Type>, Types> || ...)>* = nullptr>
	constexpr iterator insert(const iterator, Type&& t) {
		insert(std::forward<Type>(t));
		return End - 1;
	}
	
	template<typename Type, std::enable_if_t<std::is_same_v<std::decay_t<Type>, value_type> ||
	                                         (std::is_same_v<std::decay_t<Type>, Types> || ...)>* = nullptr>
	constexpr ArraySet& remove(const Type& t) {
		auto iter = constexprStd::find(Begin, End, t);
		if ( iter != End ) {
			--Index;
			--End;
			*iter = std::move(*End);
		} //if ( iter != End )
		return *this;
	}
	
	template<std::size_t N2>
	constexpr bool operator==(const ArraySet<N2, Types...>& rhs) const noexcept {
		return constexprStd::is_permutation(begin(), end(), rhs.begin(), rhs.end());
	}
	
	template<std::size_t N2>
	constexpr bool operator!=(const ArraySet<N2, Types...>& rhs) const noexcept {
		return !operator==(rhs);
	}
};

template<std::size_t N1, typename... Types1, std::size_t N2, typename... Types2>
constexpr auto operator+(const ArraySet<N1, Types1...>& set1, const ArraySet<N2, Types2...>& set2) {
	decltype(details::getCommonType(set1, set2)) ret{};
	constexprStd::copy(set1.begin(), set1.end(), constexprStd::inserter(ret, ret.end()));
	constexprStd::copy(set2.begin(), set2.end(), constexprStd::inserter(ret, ret.end()));
	return ret;
}

template<std::size_t N, typename... Types>
class AdaptableArraySet : public ArraySet<N, Types...> {
	using Base = ArraySet<N, Types...>;
	
	public:
	using Base::Base;
	
	constexpr AdaptableArraySet(void) = default;
	
	constexpr AdaptableArraySet(Base base) noexcept(std::is_nothrow_move_constructible_v<Base>) :
			Base{std::move(base)} {
		return;
	}
	
	using Base::contains;
	
	template<typename Type, std::enable_if_t<(std::negation_v<std::is_same<std::decay_t<Type>, Types>> && ...)>* = nullptr>
	constexpr bool contains(const Type&) const noexcept {
		return false;
	}
	
	using Base::remove;
	
	template<typename Type, std::enable_if_t<(std::negation_v<std::is_same<std::decay_t<Type>, Types>> && ...)>* = nullptr>
	constexpr AdaptableArraySet& remove(const Type&) noexcept {
		return *this;
	}
	
	using Base::insert;
	
	template<typename Type, std::enable_if_t<(std::negation_v<std::is_same<std::decay_t<Type>, Types>> && ...)>* = nullptr>
	[[nodiscard]] constexpr AdaptableArraySet<N, Types..., Type> insert(Type&& t) {
		AdaptableArraySet<N, Types..., Type> ret;
		constexprStd::copy(Base::cbegin(), Base::cend(), constexprStd::inserter(ret, ret.end()));
		return ret.insert(std::forward<Type>(t));
	}
};

void arraySetRuntimeChecks(void);

#endif
