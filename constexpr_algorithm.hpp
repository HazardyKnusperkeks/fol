/**
 * @file
 * @brief Defines constexpr variants of the <algorithm>s.
 */

#ifndef CONSTEXPR_ALGORITHM_HPP
#define CONSTEXPR_AGLROTIHM_HPP

#include <iterator>
#include <type_traits>

namespace constexprAlgo {

template<typename InputIterator>
constexpr auto distance(const InputIterator first, const InputIterator last);

namespace details {

namespace ops {
template<typename Compare>
struct IterCompIter {
	Compare Comp;
	
	explicit constexpr IterCompIter(Compare comp) : Comp{std::move(comp)} {
		return;
	}
	
	template<typename Iterator1, typename Iterator2>
	constexpr bool operator()(const Iterator1 it1, const Iterator2 it2) {
		return Comp(*it1, *it2);
	}
};

template<typename Compare, typename Iterator>
struct IterCompToIter {
	Compare Comp;
	Iterator Iter;
	
	constexpr IterCompToIter(Compare comp, Iterator iter) : Comp{std::move(comp)}, Iter{std::move(iter)} {
		return;
	}
	
	template<typename Iterator2>
	constexpr bool operator()(Iterator2 iter2) const {
		return Comp(*iter2, *Iter);
	}
};

template<typename Iterator1>
struct IterEqualsIter {
	Iterator1 Iter1;
	
	constexpr explicit IterEqualsIter(Iterator1 iter) : Iter1{std::move(iter)} {
		return;
	}
	
	template<typename Iterator2>
	constexpr bool operator()(const Iterator2 iter2) const {
		return *iter2 == *Iter1;
	}
};

struct IterEqualToIter {
	template<typename Iterator1, typename Iterator2>
	constexpr bool operator()(const Iterator1 it1, const Iterator2 it2) const {
		return *it1 == *it2;
	}
};

template<typename Compare, typename Iterator>
constexpr auto iterCompIter(IterCompIter<Compare> comp, Iterator it) {
	return IterCompToIter<Compare, Iterator>{std::move(comp.Comp), std::move(it)};
}

template<typename Compare>
constexpr auto iterCompIter(Compare comp) {
	return IterCompIter<Compare>{std::move(comp)};
}

template<typename Iterator>
constexpr auto iterCompIter(const IterEqualToIter, Iterator it) {
	return IterEqualsIter<Iterator>{std::move(it)};
}
} //namespace ops

template<typename InputIterator, typename Distance>
constexpr void advance(InputIterator& i, Distance n, const std::input_iterator_tag) {
	while ( n-- ) {
		++i;
	} //while ( n-- )
	return;
}

template<typename BidirectionalIterator, typename Distance>
constexpr void advance(BidirectionalIterator& i, Distance n, const std::bidirectional_iterator_tag) {
	if ( n > 0 ) {
		while ( n-- ) {
			++i;
		} //while ( n-- )
	} //if ( n > 0 )
	else {
		while ( n++ ) {
			--i;
		} //while ( n++ )
	} //else -> if ( n > 0 )
	return;
}

template<typename RandomAccessIterator, typename Distance>
constexpr void advance(RandomAccessIterator& i, const Distance n, std::random_access_iterator_tag) {
	i += n;
	return;
}

template<typename InputIterator, typename Predicate>
constexpr auto count_if(InputIterator first, const InputIterator last, const Predicate pred) {
	typename std::iterator_traits<InputIterator>::difference_type n = 0;
	for ( ; first != last; ++first ) {
		if ( pred(first) ) {
			++n;
		} //if ( pred(first) )
	} //for ( ; first != last; ++first )
	return n;
}

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

template<typename InputIterator, typename Predicate>
constexpr InputIterator find_if(InputIterator first, const InputIterator last, const Predicate pred,
                                const std::input_iterator_tag) {
	while ( first != last && !pred(first) ) {
		++first;
	} //while ( first != last && !pred(first) )
	return first;
}

template<typename RandomAccessIterator, typename Predicate>
constexpr RandomAccessIterator find_if(RandomAccessIterator first, const RandomAccessIterator last,
                                       const Predicate pred, const std::random_access_iterator_tag) {
	typename std::iterator_traits<RandomAccessIterator>::difference_type trip_count = (last - first) >> 2;
	
	for ( ; trip_count > 0; --trip_count ) {
		if ( pred(first) ) {
			return first;
		} //if ( pred(first) )
		++first;
		
		if ( pred(first) ) {
			return first;
		} //if ( pred(first) )
		++first;
		
		if ( pred(first) ) {
			return first;
		} //if ( pred(first) )
		++first;
		
		if ( pred(first) ) {
			return first;
		} //if ( pred(first) )
		++first;
	} //for ( ; trip_count > 0; --trip_count )
	
	switch ( last - first ) {
		case 3 : {
			if ( pred(first) ) {
				return first;
			} //if ( pred(first) )
			++first;
		} //case 3
		[[fallthrough]]
		case 2 : {
			if ( pred(first) ) {
				return first;
			} //if ( pred(first) )
			++first;
		} //case 2
		[[fallthrough]]
		case 1 : {
			if ( pred(first) ) {
				return first;
			} //if ( pred(first) )
			++first;
		} //case 1
//		[[fallthrough]]
	} //switch ( last - first )
	return last;
}

template<typename Iterator, typename Predicate>
constexpr Iterator find_if(Iterator first, Iterator last, Predicate pred) {
	return find_if(std::move(first), std::move(last), std::move(pred),
	               typename std::iterator_traits<Iterator>::iterator_category{});
}

template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
constexpr bool is_permutation(ForwardIterator1 first1, const ForwardIterator1 last1,
                              ForwardIterator2 first2, const ForwardIterator2 last2,
                              const BinaryPredicate pred) {
	using Cat1 = typename std::iterator_traits<ForwardIterator1>::iterator_category;
	using Cat2 = typename std::iterator_traits<ForwardIterator2>::iterator_category;
	using It1IsRA = std::is_same<Cat1, std::random_access_iterator_tag>;
	using It2IsRA = std::is_same<Cat2, std::random_access_iterator_tag>;
	constexpr bool raIters = It1IsRA::value && It2IsRA::value;
	if ( raIters ) {
		const auto d1 = constexprAlgo::distance(first1, last1);
		const auto d2 = constexprAlgo::distance(first2, last2);
		if ( d1 != d2 ) {
			return false;
		} //if ( d1 != d2 )
	} //if ( raIters )
	
	// Efficiently compare identical prefixes:  O(N) if sequences
	// have the same elements in the same order.
	for ( ; first1 != last1 && first2 != last2; ++first1, ++first2 ) {
		if ( !pred(first1, first2) ) {
			break;
		} //if ( !pred(first1, first2) )
	} //for ( ; first1 != last1 && first2 != last2; ++first1, ++first2 )
	
	if ( raIters ) {
		if ( first1 == last1 ) {
			return true;
		} //if ( first1 == last1 )
	} //if ( raIters )
	else {
		const auto d1 = constexprAlgo::distance(first1, last1);
		const auto d2 = constexprAlgo::distance(first2, last2);
		
		if ( d1 == 0 && d2 == 0 ) {
			return true;
		} //if ( d1 == 0 && d2 == 0 )
		if ( d1 != d2 ) {
			return false;
		} //if ( d1 != d2 )
	} //else -> if ( raIters )
	
	for ( ForwardIterator1 scan = first1; scan != last1; ++scan ) {
		const auto op = ops::iterCompIter(pred, scan);
		if ( scan != find_if(first1, scan, op) ) {
			continue; // We've seen this one before.
		} //if ( scan != find_if(first1, scan, op) )
		
		const auto matches = count_if(first2, last2, op);
		if ( 0 == matches || count_if(scan, last1, std::move(op)) != matches ) {
			return false;
		} //if ( 0 == matches || count_if(scan, last1, std::move(op)) != matches )
	} //for ( ForwardIterator1 scan = first1; scan != last1; ++scan )
	return true;
}

} //namesapce details

template<typename InputIterator, typename Distance>
constexpr void advance(InputIterator& i, const Distance n) {
	typename std::iterator_traits<InputIterator>::difference_type d = n;
	details::advance(i, d, typename std::iterator_traits<InputIterator>::iterator_category{});
	return;
}

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

template<typename InputIter, typename T>
constexpr InputIter find(InputIter begin, const InputIter end, const T& t) {
	for ( ; begin != end; ++begin ) {
		if ( *begin == t ) {
			break;
		} //if ( *begin == t )
	} //for ( ; begin != end; ++begin )
	return begin;
}

} //namespace constexprAlgo

#endif
