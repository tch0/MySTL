#ifndef TALGORITHM_HPP
#define TALGORITHM_HPP

#include <iterator>
#include <type_traits>
#include <utility>
#include <functional>
#include <tutility.hpp>
#include <titerator.hpp>
#include <tstl_heap.hpp> // heap algorithms

namespace tstd
{

// iterator category
template<typename Iterator>
inline constexpr bool is_input_iterator_v = std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>::value;
template<typename Iterator>
inline constexpr bool is_forward_iterator_v = std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>::value;
template<typename Iterator>
inline constexpr bool is_bidirectional_iterator_v = std::is_base_of<std::bidirectional_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>::value;
template<typename Iterator>
inline constexpr bool is_random_access_iterator_v = std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>::value;

// static assert for iterator category
#define STATIC_ASSERT_INPUT_ITERATOR(Iterator) \
    static_assert(tstd::is_input_iterator_v<Iterator>)
#define STATIC_ASSERT_FORWARD_ITERATOR(Iterator) \
    static_assert(tstd::is_forward_iterator_v<Iterator>)
#define STATIC_ASSERT_BIDIRECTIONAL_ITERATOR(Iterator) \
    static_assert(tstd::is_bidirectional_iterator_v<Iterator>)
#define STATIC_ASSERT_RANDOM_ACCESS_ITERATOR(Iterator) \
    static_assert(tstd::is_random_access_iterator_v<Iterator>)


//========================================= non-modifying sequence algorithms ======================================================================
// all_of
// complexity: at most last-first application of predicate
template<typename InputIterator, typename UnaryPredicate>
constexpr bool all_of(InputIterator first, InputIterator last, UnaryPredicate p)
{
    for (; first != last; ++first)
    {
        if (!p(*first))
        {
            return false;
        }
    }
    return true;
}
// any_of
// complexity: at most last-first application of predicate
template<typename InputIterator, typename UnaryPredicate>
constexpr bool any_of(InputIterator first, InputIterator last, UnaryPredicate p)
{
    for (; first != last; ++first)
    {
        if (p(*first))
        {
            return true;
        }
    }
    return false;
}
// none_of
// complexity: at most last-first application of predicate
template<typename InputIterator, typename UnaryPredicate>
constexpr bool none_of(InputIterator first, InputIterator last, UnaryPredicate p)
{
    for (; first != last; ++first)
    {
        if (p(*first))
        {
            return false;
        }
    }
    return true;
}

// for_each
// complexity: exactly last-first applications of f
template<typename InputIterator, typename UnaryFunction>
constexpr UnaryFunction for_each(InputIterator first, InputIterator last, UnaryFunction f)
{
    for (; first != last; ++first)
    {
        f(*first);
    }
    return f;
}
// for_each_n
// complexity: exactly n applications of f
template<typename InputIterator,typename Size, typename UnaryFunction>
constexpr InputIterator for_each_n(InputIterator first, Size n, UnaryFunction f)
{
    for (Size i = 0; i < n; ++i, ++first)
    {
        f(*first);
    }
    return f;
}

// count
// complexity: exactly last-first comparisons
template<typename InputIterator, typename T>
constexpr typename std::iterator_traits<InputIterator>::difference_type count(InputIterator first, InputIterator last, const T& value)
{
    typename std::iterator_traits<InputIterator>::difference_type ret = 0;
    for (; first != last; ++first)
    {
        if (*first == value)
        {
            ++ret;
        }
    }
    return ret;
}
// count_if
// complexity: exactly last-first applications of the predicate p
template<class InputIterator, class UnaryPredicate>
constexpr typename std::iterator_traits<InputIterator>::difference_type count_if(InputIterator first, InputIterator last, UnaryPredicate p)
{
    typename std::iterator_traits<InputIterator>::difference_type ret = 0;
    for (; first != last; ++first)
    {
        if (p(*first))
        {
            ++ret;
        }
    }
    return ret;
}

// mismatch
// complexity: at most last1-first1 applications of operator==
template<typename InputIterator1, typename InputIterator2>
constexpr std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) // 1
{
    while (first1 != last1 && *first1 == *first2)
    {
        ++first1;
        ++first2;
    }
    return {first1, first2};
}
// complexity: at most last1-first1 applications of the predicate p
template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
constexpr std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate p) // 2
{
    while (first1 != last1 && p(*first1, *first2))
    {
        ++first1;
        ++first2;
    }
    return {first1, first2};
}
// complexity: at most min(last1-first1, last2-first2) applications of the operator==
template<typename InputIterator1, typename InputIterator2>
constexpr std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) // 3
{
    while (first1 != last1 && first2 != last2 && *first1 == *first2)
    {
        ++first1;
        ++first2;
    }
    return {first1, first2};
}
// complexity: at most min(last1-first1, last2-first2) applications of the predicate p
template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
constexpr std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryPredicate p) // 4
{
    while (first1 != last1 && first2 != last2 && p(*first1, *first2))
    {
        ++first1;
        ++first2;
    }
    return {first1, first2};
}

// find
// complexity: at most last-first applications of operator==
template<typename InputIterator, typename T>
constexpr InputIterator find(InputIterator first, InputIterator last, const T& value)
{
    for (; first != last; ++first)
    {
        if (*first == value)
        {
            return first;
        }
    }
    return last;
}
// find_if
// complexity: at most last-first applications of the predicate p
template<typename InputIterator, typename UnaryPredicate>
constexpr InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate p)
{
    for (; first != last; ++first)
    {
        if (p(*first))
        {
            return first;
        }
    }
    return last;
}
// find_if_not
// complexity: at most last-first applications of the predicate p
template<typename InputIterator, typename UnaryPredicate>
constexpr InputIterator find_if_not(InputIterator first, InputIterator last, UnaryPredicate p)
{
    for (; first != last; ++first)
    {
        if (!p(*first))
        {
            return first;
        }
    }
    return last;
}

// find_end: search for the last occurrence of sequence [s_first, s_last) in the range [first, last)
// complexity: at most S*(N-S-1) comparisons/applications of predicate where S = std::distance(s_first, s_last), N = std::distance(first, last)
template<typename ForwardIterator1, typename ForwardIterator2>
constexpr ForwardIterator1 search(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 s_first, ForwardIterator2 s_last);
template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
constexpr ForwardIterator1 search(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 s_first, ForwardIterator2 s_last, BinaryPredicate p);
template<typename ForwardIterator1, typename ForwardIterator2>
constexpr ForwardIterator1 find_end(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 s_first, ForwardIterator2 s_last) // 1
{
    if (s_first == s_last)
    {
        return last;
    }
    ForwardIterator1 result = last;
    while (true)
    {
        ForwardIterator1 new_result = tstd::search(first, last, s_first, s_last);
        if (new_result == last)
        {
            break;
        }
        else
        {
            result = new_result;
            first = result;
            ++first; // from next element after the beginning of matched subsequence
        }
    }
    return result;
}
template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
constexpr ForwardIterator1 find_end(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 s_first, ForwardIterator2 s_last, BinaryPredicate p) // 2
{
    if (s_first == s_last)
    {
        return last;
    }
    ForwardIterator1 result = last;
    while (true)
    {
        ForwardIterator1 new_result = tstd::search(first, last, s_first, s_last, p);
        if (new_result == last)
        {
            break;
        }
        else
        {
            result = new_result;
            first = result;
            ++first; // from next element after the beginning of matched subsequence
        }
    }
    return result;
}

// find_first_of: search the range [first, last) for any of the elements in the range [s_first, s_last)
// complexity: at most S*N comparisons/applications of predicate where S = std::distance(s_first, s_last), N = std::distance(first, last)
template<typename InputIterator, typename ForwardIterator>
constexpr InputIterator find_first_of(InputIterator first, InputIterator last, ForwardIterator s_first, ForwardIterator s_last) // 1
{
    for (; first != last; ++first)
    {
        for (ForwardIterator it = s_first; it != s_last; ++it)
        {
            if (*first == *it)
            {
                return first;
            }
        }
    }
    return last;
}
template<typename InputIterator, typename ForwardIterator, typename BinaryPredicate>
constexpr InputIterator find_first_of(InputIterator first, InputIterator last, ForwardIterator s_first, ForwardIterator s_last, BinaryPredicate p) // 2
{
    for (; first != last; ++first)
    {
        for (ForwardIterator it = s_first; it != s_last; ++it)
        {
            if (p(*first, *it))
            {
                return first;
            }
        }
    }
    return last;
}

// adjacent_find: searches the range [first, last) for two consecutive equal elements
// complexity: exactly min((result-first)+1, (last-first)-1) applications of operator==/the predicate p
template<typename ForwardIterator>
constexpr ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) // 1
{
    if (first == last)
    {
        return last;
    }
    ForwardIterator next = first;
    ++next;
    for (; next != last; ++next, ++first)
    {
        if (*first == *next)
        {
            return first;
        }
    }
    return last;
}
template<typename ForwardIterator, typename BinaryPredicate>
constexpr ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate p) // 2
{
    if (first == last)
    {
        return last;
    }
    ForwardIterator next = first;
    ++next;
    for (; next != last; ++next, ++first)
    {
        if (p(*first, *next))
        {
            return first;
        }
    }
    return last;
}

// search: search for the first occurrence of sequence [s_first, s_last) in the range [first, last)
// complexity: at most S*N comparisons/applications of predicate where S = std::distance(s_first, s_last), N = std::distance(first, last)
template<typename ForwardIterator1, typename ForwardIterator2>
constexpr ForwardIterator1 search(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 s_first, ForwardIterator2 s_last) // 1
{
    for (;; ++first)
    {
        ForwardIterator1 it = first;
        for (ForwardIterator2 s_it = s_first; ;++it, ++s_it)
        {
            if (s_it == s_last)
            {
                return first;
            }
            if (it == last)
            {
                return last;
            }
            if (!(*it != *s_it))
            {
                break;
            }
        }
    }
    return last;
}
template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
constexpr ForwardIterator1 search(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 s_first, ForwardIterator2 s_last, BinaryPredicate p) // 2
{
    for (;; ++first)
    {
        ForwardIterator1 it = first;
        for (ForwardIterator2 s_it = s_first; ;++it, ++s_it)
        {
            if (s_it == s_last)
            {
                return first;
            }
            if (it == last)
            {
                return last;
            }
            if (!p(*it, *s_it))
            {
                break;
            }
        }
    }
    return last;
}
// complexity: depend on searcher
template<typename ForwardIterator, typename Searcher>
constexpr ForwardIterator search(ForwardIterator first, ForwardIterator last, const Searcher& searcher)
{
    // todo
    return last;
}

// search_n: searches the range [first, last) for the first sequence of count identical elements, each equal to the given value.
// complexity: at most last-first applications of operator==/predicate
template<typename ForwardIterator, typename Size, typename T>
constexpr ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T& value)
{
    if (count <= 0)
    {
        return first;
    }
    for (; first != last; ++first)
    {
        if (!(*first == value))
        {
            continue;
        }
        ForwardIterator candidate = first;
        Size cur_count = 0;
        while (true)
        {
            ++cur_count;
            if (cur_count >= count)
            {
                return candidate; // success
            }
            ++first;
            if (first == last)
            {
                return last; // exhausted the list
            }
            if (!(*last == value))
            {
                break; // too few in a row
            }
        }
    }
    return last;
}
template<typename ForwardIterator, typename Size, typename T, typename BinaryPredicate>
constexpr ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T& value, BinaryPredicate p)
{
    if (count <= 0)
    {
        return first;
    }
    for (; first != last; ++first)
    {
        if (!p(*first, value))
        {
            continue;
        }
        ForwardIterator candidate = first;
        Size cur_count = 0;
        while (true)
        {
            ++cur_count;
            if (cur_count >= count)
            {
                return candidate; // success
            }
            ++first;
            if (first == last)
            {
                return last; // exhausted the list
            }
            if (!p(*last, value))
            {
                break; // too few in a row
            }
        }
    }
    return last;
}

// ======================================== modifying sequence algorithms ==========================================================================

// ======================================== partitioning algorithms ================================================================================

// ======================================== sorting algorithms =====================================================================================

// ======================================== binary search algorithms ===============================================================================

// ======================================== algorithms on sorted ranges ============================================================================

// ======================================== set algorithms (on sorted ranges) ======================================================================

// ======================================== heap algorithms (in <tstl_heap.hpp>) ===================================================================

// ======================================== minimum/maximum algorithms =============================================================================

// ======================================== comparison algorithms ==================================================================================
// equal
// complexity: 1,2: at most last1-first applications of prediacate, 3,4: at most min(last1-first1, last2-first2) applications of predicate
template<typename InputIterator1, typename InputIterator2>
constexpr bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) // 1
{
    for (; first1 != last1; ++first1, ++first2)
    {
        if (!(*first1 == *first2))
        {
            return false;
        }
    }
    return true;
}
template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
constexpr bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate p) // 2
{
    for (; first1 != last1; ++first1, ++first2)
    {
        if (!p(*first1, *first2))
        {
            return false;
        }
    }
    return true;
}
template<typename InputIterator1, typename InputIterator2>
constexpr bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) // 3
{
    for (; first1 != last2 && first2 != last2; ++first1, ++first2)
    {
        if (!(*first1 == *first2))
        {
            return false;
        }
    }
    return true;
}
template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
constexpr bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryPredicate p) // 4
{
    for (; first1 != last2 && first2 != last2; ++first1, ++first2)
    {
        if (!p(*first1, *first2))
        {
            return false;
        }
    }
    return true;
}

// lexicographical_compare: compare two range in lexicographical order, return range1 < range2
// complexity: at most 2*min(M, N) comparisons where M = std::distance(first1, last1), N = std::distance(first2, last2)
template<typename InputIterator1, typename InputIterator2>
constexpr bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) // 1
{
    for (; first1 != last2 && first2 != last2; ++first1, ++first2)
    {
        if (*first1 < *first2)
        {
            return true;
        }
        if (*first2 < *first1)
        {
            return false;
        }
    }
    return (first1 == last1) && (first2 != last2);
}
template<typename InputIterator1, typename InputIterator2, typename Compare>
constexpr bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) // 2
{
    for (; first1 != last2 && first2 != last2; ++first1, ++first2)
    {
        if (comp(*first1, *first2))
        {
            return true;
        }
        if (comp(*first2, *first1))
        {
            return false;
        }
    }
    return (first1 == last1) && (first2 != last2);
}

// lexicographical_compare_three_way: three way compare two ranges in lexicographical order
// complexity: at most min(last1-first1, last2-first2) comparisons
template<typename InputIterator1, typename InputIterator2, typename Compare>
constexpr auto lexicographical_compare_three_way(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
    -> decltype(comp(*first1, *first2))
{
    using ret_t = decltype(comp(*first1, *first2));
    static_assert(std::disjunction_v<
                      std::is_same<ret_t, std::strong_ordering>,
                      std::is_same<ret_t, std::weak_ordering>,
                      std::is_same<ret_t, std::partial_ordering>>,
                  "The return type must be a comparison category type.");
    bool exhaust1 = (first1 == last1);
    bool exhaust2 = (first2 == last2);
    for (; !exhaust1 && !exhaust2; exhaust1 = (++first1 == last1), exhaust2 = (++first2 == last2))
        if (auto c = comp(*first1, *first2); c != 0)
            return c;
    return !exhaust1 ? std::strong_ordering::greater :
           !exhaust2 ? std::strong_ordering::less :
                       std::strong_ordering::equal;
}
template<typename InputIterator1, typename InputIterator2>
constexpr auto lexicographical_compare_three_way(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
{
    return lexicographical_compare_three_way(first1, last1, first2, last2, std::compare_three_way());
}

// ======================================== permutation algorithms =================================================================================

} // namespace tstd


#endif // TALGORITHM_HPP