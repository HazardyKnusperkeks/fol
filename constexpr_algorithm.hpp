/**
 * @file
 * @brief Defines constexpr variants of the <algorithm>s.
 */

#ifndef CONSTEXPR_ALGORITHM_HPP
#define CONSTEXPR_AGLROTIHM_HPP

#include <iterator>
#include <type_traits>

namespace constexprAlgo {

namespace details {

template<typename InputIterator>
constexpr auto distance(InputIterator first, const InputIterator last, const std::input_iterator_tag) noexcept {
	typename std::iterator_traits<InputIterator>::difference_type n = 0;
	for ( ; first != last; ++first, ++n ) {
		
	} //for ( ; first != last; ++first, ++n )
	return n;
}

template<typename RandomAccessIterator>
constexpr auto distance(RandomAccessIterator first, const RandomAccessIterator last,
                        const std::random_access_iterator_tag) noexcept {
	return last - first;
}

} //namesapce details

template<typename InputIterator>
constexpr auto distance(InputIterator first, const InputIterator last) noexcept {
	return details::distance(first, last, typename std::iterator_traits<InputIterator>::iterator_category{});
}

template<typename IterT1, typename IterT2>
constexpr bool equal(IterT1 first1, const IterT1 last1, IterT2 first2) {
	for ( ; first1 != last1; ++first1, ++first2 ) {
		if ( !(*first1 == *first2) ) {
			return false;
		} //if ( !(*first1 == *first2) )
	} //for ( ; first1 != last1; ++first1, ++first2 )
	return true;
}

template<typename IterT1, typename IterT2>
constexpr bool equal(IterT1 first1, const IterT1 last1, IterT2 first2, const IterT2 last2) {
	using RATag   = std::random_access_iterator_tag;
	using Cat1    = typename std::iterator_traits<IterT1>::iterator_category;
	using Cat2    = typename std::iterator_traits<IterT2>::iterator_category;
	using RAIters = std::conjunction<std::is_same<Cat1, RATag>, std::is_same<Cat2, RATag>>;
	
	if ( RAIters::value ) {
		auto d1 = distance(first1, last1);
		auto d2 = distance(first2, last2);
		if ( d1 != d2 ) {
			return false;
		} //if ( d1 != d2 )
		return equal(first1, last1, first2);
	} //if ( RAIters::value )
	
	for ( ; first1 != last1 && first2 != last2; ++first1, ++first2 ) {
		if ( !(*first1 == *first2) ) {
			return false;
		} //if ( !(*first1 == *first2) )
	} //for ( ; first1 != last1 && first2 != last2; ++first1, ++first2 )
	return first1 == last1 && first2 == last2;
}

} //namespace constexprAlgo

#endif
