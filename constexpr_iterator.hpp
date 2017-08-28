/**
 * @file
 * @brief Contains the constexpr variants of <iterator>
 */

#ifndef CONSTEXPR_ITERATOR_HPP
#define CONSTEXPR_ITERATOR_HPP

#include <iterator>
#include <type_traits>

namespace constexprStd {

template<typename Container>
class insert_iterator {
	private:
	using IterType   = typename Container::iterator;
	using CValueType = typename Container::value_type;
	
	protected:
	Container *C;
	IterType Iter;
	
	public:
	using value_type 		= void;
	using difference_type 	= void;
	using pointer 			= void;
	using reference         = void;
	using iterator_category = std::output_iterator_tag;
	using container_type    = Container;
	
	constexpr insert_iterator(Container& c, IterType iter) noexcept(std::is_nothrow_move_constructible_v<IterType>) :
			C{std::addressof(c)}, Iter{std::move(iter)} {
		return;
	}
	
	constexpr insert_iterator& operator=(const CValueType& value)
			noexcept(noexcept(Iter = C->insert(Iter, value)) && noexcept(++Iter)) {
		Iter = C->insert(Iter, value);
		++Iter;
		return *this;
	}
	
	constexpr insert_iterator& operator=(CValueType&& value)
			noexcept(noexcept(Iter = C->insert(Iter, std::move(value))) && noexcept(++Iter)) {
		Iter = C->insert(Iter, std::move(value));
		++Iter;
		return *this;
	}
	
	constexpr insert_iterator& operator*(void) noexcept {
		return *this;
	}
	
	constexpr insert_iterator& operator++(void) noexcept {
		return *this;
	}
	
	constexpr insert_iterator& operator++(const int) noexcept {
		return *this;
	}
};

template<typename Container>
constexpr insert_iterator<Container> inserter(Container& c, const typename Container::iterator iter) {
	return {c, iter};
}

} //namespace constexprStd

#endif
