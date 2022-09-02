#ifndef TALGORITHM_HPP
#define TALGORITHM_HPP

#include <iterator>
#include <type_traits>
#include <utility>
#include <functional>
#include <cstdlib>
#include <random>
#include <initializer_list>
#include <cassert>
#include <compare>
#include <tutility.hpp>
#include <titerator.hpp>
#include <tstl_heap.hpp> // heap algorithms
#include <tstl_uninitialized.hpp>

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

namespace impl
{
// temporary buffer for those algorithms that need extra memory
template<typename ForwardIterator, typename T, typename Allocator = tstd::allocator<T>>
class TemporaryBuffer
{
public:
    using iterator = T*;
    using reverse_iterator = tstd::reverse_iterator<iterator>;
    TemporaryBuffer(ForwardIterator seed, std::size_t n) 
        : elem_count(n == 0 ? 1 : n)
        , data(nullptr)
        , alloc()
    {
        data = static_cast<T*>(alloc.allocate(elem_count));
        for (int i = 0; i < elem_count; ++i)
        {
            alloc.construct(data+i, std::move(*seed));
        }
        *seed = std::move(*data);
    }
    ~TemporaryBuffer()
    {
        for (int i = 0; i < elem_count; ++i)
        {
            alloc.destroy(data+i);
        }
        alloc.deallocate(data, elem_count);
    }
    T* begin()
    {
        return data;
    }
    T* end()
    {
        return data + elem_count;
    }
    reverse_iterator rbegin()
    {
        return reverse_iterator(data + elem_count);
    }
    reverse_iterator rend()
    {
        return reverse_iterator(data);
    }
private:
    T* data;
    std::size_t elem_count;
    tstd::allocator<T> alloc;
};
}

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
    return first;
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
            if (!(*it == *s_it))
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
            if (!(*first == value))
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
            if (!p(*first, value))
            {
                break; // too few in a row
            }
        }
    }
    return last;
}

// ======================================== modifying sequence algorithms ==========================================================================
// copy
// complexity: exactly last-first assignments
template<typename InputIterator, typename OutputIterator>
constexpr OutputIterator copy(InputIterator first, InputIterator last, OutputIterator d_first)
{
    for (; first != last; ++first)
    {
        *d_first++ = *first;
    }
    return d_first;
}
// copy_if
// complexity: exactly last-first applications of predicate
template<typename InputIterator, typename OutputIterator, typename UnaryPredicate>
constexpr OutputIterator copy_if(InputIterator first, InputIterator last, OutputIterator d_first, UnaryPredicate p)
{
    for (; first != last; ++first)
    {
        if (p(*first))
        {
            *d_first++ = *first;
        }
    }
    return d_first;
}

// copy_n
// complexity: if count > 0, exactly count assignments
template<typename InputIterator, typename Size, typename OutputIterator>
constexpr OutputIterator copy_n(InputIterator first, Size count, OutputIterator result)
{
    for (; count > 0; --count, ++first, ++result)
    {
        *result = *first;
    }
    return result;
}

// copy_backward
// complexity: exactly last-first asignments
template<typename BidirectionalIterator1, typename BidirectionalIterator2>
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 d_last)
{
    while (first != last)
    {
        *(--d_last) = *(--last);
    }
    return d_last;
}

// move: move elements in range to another range
// complexity: exactly last-first move assignments
template<typename InputIterator, typename OutputIterator>
constexpr OutputIterator move(InputIterator first, InputIterator last, OutputIterator d_first)
{
    for (; first != last; ++first, ++d_first)
    {
        *d_first = std::move(*first);
    }
    return d_first;
}

// move_backward
// complexity: exactly last-first move assignments
template<typename BidirectionalIterator1, typename BidirectionalIterator2>
BidirectionalIterator2 move_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 d_last)
{
    while (first != last)
    {
        *(--d_last) = std::move(*(--last));
    }
    return d_last;
}

// fill
// complexity: exactly last-first assignments
template<typename ForwardIterator, typename T>
constexpr void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
    for (; first != last; ++first)
    {
        *first = value;
    }
}

// fill_n
// complexity: exactly count assignments
template<typename OutputIterator, typename Size, typename T>
constexpr OutputIterator fill_n(OutputIterator first, Size count, const T& value)
{
    for (; count > 0; --count, ++first)
    {
        *first = value;
    }
    return first;
}

// transform
// complexity: exactly last1-first1 applications of op
template<typename InputIterator, typename OutputIterator, typename UnaryOperation>
constexpr OutputIterator transform(InputIterator first1, InputIterator last1, OutputIterator d_first, UnaryOperation op) // 1
{
    for (; first1 != last1; ++first1, ++d_first)
    {
        *d_first = op(*first1);
    }
    return d_first;
}
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryOperation>
constexpr OutputIterator transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator d_first, BinaryOperation op) // 2
{
    for (; first1 != last1; ++first1, ++first2, ++d_first)
    {
        *d_first = op(*first1, *first2);
    }
    return d_first;
}

// generate
// complexity: exactly last-first calls of g() and assignments
template<typename ForwardIterator, typename Generator>
constexpr void generate(ForwardIterator first, ForwardIterator last, Generator g)
{
    for (; first != last; ++first)
    {
        *first = g();
    }
}

// generate_n
// complexity: exactly count calls of g() and assignments if count > 0
template<typename OutputIterator, typename Size, typename Generator>
constexpr OutputIterator generate_n(OutputIterator first, Size count, Generator g)
{
    for (; count > 0; --count, ++first)
    {
        *first = g();
    }
    return first;
}

// remove: keep the relative order of rest elements, do not change the size of container
// complexity: exactly last-first applications of operator==
template<typename ForwardIterator, typename T>
constexpr ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value)
{
    first = tstd::find(first, last, value);
    if (first != last)
    {
        for (ForwardIterator it = first; ++it != last;)
        {
            if (!(*it == value))
            {
                *first++ = std::move(*it);
            }
        }
    }
    return first;
}
// remove_if
// complexity: exactly last-first applications of predicate
template<typename ForwardIterator, typename UnaryPreciate>
constexpr ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, UnaryPreciate p)
{
    first = tstd::find_if(first, last, p);
    if (first != last)
    {
        for (ForwardIterator it = first; ++it != last;)
        {
            if (!p(*it))
            {
                *first++ = std::move(*it);
            }
        }
    }
    return first;
}

// remove_copy
// complexity: exactly last-first applications of operator==
template<typename InputIterator, typename OutputIterator, typename T>
constexpr OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator d_first, const T& value)
{
    for (; first != last; ++first)
    {
        if (!(*first == value))
        {
            *d_first++ = *first;
        }
    }
    return d_first;
}
// remove_copy_if
// complexity: exactly last-first applications of predicate
template<typename InputIterator, typename OutputIterator, typename UnaryPredicate>
constexpr OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator d_first, UnaryPredicate p)
{
    for (; first != last; ++first)
    {
        if (!p(*first))
        {
            *d_first++ = *first;
        }
    }
    return d_first;
}

// replace
// complexity: exactly last-first applications of operator==
template<typename ForwardIterator, typename T>
constexpr void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value)
{
    for (; first != last; ++first)
    {
        if (*first == old_value)
        {
            *first = new_value;
        }
    }
}
// replace_if
// complexity: exactly last-first applications of predicate
template<typename ForwardIterator, typename UnaryPreciate, typename T>
constexpr void replace_if(ForwardIterator first, ForwardIterator last, UnaryPreciate p, const T& new_value)
{
    for (; first != last; ++first)
    {
        if (p(*first))
        {
            *first = new_value;
        }
    }
}

// replace_copy
// complexity: exactly last-first applications of operator==
template<typename InputIterator, typename OutputIterator, typename T>
constexpr OutputIterator replace_copy(InputIterator first, InputIterator last, OutputIterator d_first, const T& old_value, const T& new_value)
{
    for (; first != last; ++first, ++d_first)
    {
        *d_first = (*first == old_value) ? new_value : *first;
    }
    return d_first;
}
// replace_copy_if
// complexity: exactly last-first applications of predicate
template<typename InputIterator, typename OutputIterator, typename UnaryPredicate, typename T>
constexpr OutputIterator replace_copy_if(InputIterator first, InputIterator last, OutputIterator d_first, UnaryPredicate p, const T& new_value)
{
    for (; first != last; ++first, ++d_first)
    {
        *d_first = p(*first) ? new_value : *first;
    }
    return d_first;
}

// swap
// defined in <tutility.hpp>

// swap_ranges
// complexity: O(last1-first1)
template<typename ForwardIterator1, typename ForwardIterator2>
constexpr void iter_swap(ForwardIterator1 a, ForwardIterator2 b);
template<typename ForwardIterator1, typename ForwardIterator2>
constexpr ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
{
    for (; first1 != last1; ++first1, ++first2)
    {
        tstd::iter_swap(first1, first2);
    }
    return first2;
}

// iter_swap
// complexity: O(1)
template<typename ForwardIterator1, typename ForwardIterator2>
constexpr void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{
    using tstd::swap;
    swap(*a, *b);
}

// reverse
// compelxity: exactly swap (last-first)/2 times
template<typename BidirectionalIterator>
constexpr void reverse(BidirectionalIterator first, BidirectionalIterator last)
{
    if constexpr (is_random_access_iterator_v<BidirectionalIterator>)
    {
        if (first == last)
            return;
        for (--last; first < last; ++first, --last)
        {
            tstd::iter_swap(first, last);
        }
    }
    else // bidirectional iterator
    {
        while ((first != last) && (first != --last))
        {
            tstd::iter_swap(first++, last);
        }
    }
}

// reverse_copy
// complexity: exactly last-first assignments
template<typename BidirectionalIterator, typename OutputIterator>
constexpr OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator d_first)
{
    while (first != last)
    {
        *d_first++ = *--last;
    }
    return d_first;
}

// rotate: rotate range, let n_first be the new first element, n_first-1 be new last element. return the iterator that old first element locate(first + (last - n_first))
// complexity: O(last-first)
template<typename ForwardIterator>
constexpr ForwardIterator rotate(ForwardIterator first, ForwardIterator n_first, ForwardIterator last)
{
    if (first == n_first)
        return last;
    if (n_first == last)
        return first;
    auto read = n_first;
    auto write = first;
    auto next_read = first;
    while (read != last)
    {
        if (write == next_read)
        {
            next_read = read;
        }
        tstd::iter_swap(write++, read++);
    }
    tstd::rotate(write, next_read, last);
    return write;
}

// rotate_copy: rotate and copy to another list
// complexity: O(last-first)
template<typename ForwardIterator, typename OutputIterator>
constexpr OutputIterator rotate_copy(ForwardIterator first, ForwardIterator n_first, ForwardIterator last, OutputIterator d_first)
{
    d_first = tstd::copy(n_first, last, d_first);
    return tstd::copy(first, n_first, d_first);
}

// shift_left
// complexity: at most (last-first)-n assginments
template<typename ForwardIterator>
constexpr ForwardIterator shift_left(ForwardIterator first, ForwardIterator last, typename std::iterator_traits<ForwardIterator>::difference_type n)
{
    if (n <= 0 || n > tstd::distance(first, last))
    {
        return first;
    }
    return tstd::move(tstd::next(first, n), last, first);
}
// shift_right
// complexity: at most (last-first)-n assignments or swaps
template<typename ForwardIterator>
constexpr ForwardIterator shift_right(ForwardIterator first, ForwardIterator last, typename std::iterator_traits<ForwardIterator>::difference_type n)
{
    if (n == 0)
        return first;
    if constexpr (is_bidirectional_iterator_v<ForwardIterator>)
    {
        return tstd::move_backward(first, tstd::prev(last, n), last);
    }
    else // forward iterator
    {
        auto result = tstd::next(first, n);
        if (result == last)
            return last;
        auto dest_head = first;
        auto dest_tail = result;
        while (dest_head != result)
        {
            if (dest_tail == last)
            {
                std::move(first, dest_head, result);
                return result;
            }
            ++dest_head;
            ++dest_tail;
        }
        for (;;)
        {
            auto cursor = first;
            while (cursor != result)
            {
                if (dest_tail == last)
                {
                    dest_head = std::move(cursor, result, dest_head);
                    std::move(first, cursor, dest_head);
                    return result;
                }
                tstd::iter_swap(cursor, dest_head);
                ++dest_head;
                ++dest_tail;
                ++cursor;
            }
        }
        return last;
    }
}

// random_shuffle
// deprecated in C++ 14, removed in C++17
// complexity: O(last-first)
template<typename RandomIterator>
void random_shuffle(RandomIterator first, RandomIterator last)
{
    typename std::iterator_traits<RandomIterator>::difference_type i, n;
    n = last - first;
    for (i = n-1; i > 0; --i)
    {
        using tstd::swap;
        swap(first[i], first[std::rand() % (i+1)]);
    }
}
// r should be a function obejct that receive a nubmer N and generate numbers from 0 to N-1
template<typename RandomIterator, typename RandomFunc>
void random_shuffle(RandomIterator first, RandomIterator last, RandomFunc&& r)
{
    typename std::iterator_traits<RandomIterator>::difference_type i, n;
    n = last - first;
    for (i = n-1; i > 0; --i)
    {
        using tstd::swap;
        swap(first[i], first[r(i+1)]);
    }
}

// shuffle
// complexity: O(last-first)
// g should be a random engine
template<typename RandomIterator, typename URBG>
void shuffle(RandomIterator first, RandomIterator last, URBG&& g)
{
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    using distr_t = std::uniform_int_distribution<diff_t>;
    using param_t = typename distr_t::param_type;
    
    distr_t D;
    diff_t n = last - first;
    for (diff_t i = n-1; i > 0; --i)
    {
        using tstd::swap;
        swap(first[i], first[D(g, param_t(0, i))]);
    }
}

// sample: sample n elements from sequence [first, last) randomly, write to output iterator, using g generate random number
// complexity: O(last-first)
// copy from libstdc++, doesn't understand yet!
template<typename PopulationIterator, typename SampleIterator, typename Distance, typename URBG>
SampleIterator sample(PopulationIterator first, PopulationIterator last, SampleIterator out, Distance n, URBG&& g)
{
    if (is_forward_iterator_v<PopulationIterator>)
    {
        using distrib_type = std::uniform_int_distribution<Distance>;
        using param_type = typename distrib_type::param_type;
        using USize = std::make_unsigned_t<Distance>;
        using Gen = std::remove_reference_t<URBG>;
        using UC_type = std::common_type_t<typename Gen::result_type, USize>;

        if (first == last)
        {
            return out;
        }
        distrib_type d;
        Distance unsampled_size = tstd::distance(first, last);
        n = std::min(n, unsampled_size); // todo: replace with tstd::min
        const UC_type range = g.max() - g.min();
        if (range / UC_type(unsampled_size) >= unsampled_size)
        {
            while (n != 0 && unsampled_size >= 2)
            {
                Distance n1 = d(g, param_type(0, unsampled_size-1));
                --unsampled_size;
                if (n1 < n)
                {
                    *out++ = *first;
                    --n;
                }
                ++first;
            }
        }
        for (; n != 0; ++first)
        {
            if (d(g, param_type(0, --unsampled_size)) < n)
            {
                *out++ = *first;
                --n;
            }
        }
        return out;
    }
    else // input iterator
    {
        static_assert(is_random_access_iterator_v<SampleIterator>, "if population is input iterator, then output should be random access iterator");
        using distrib_type = std::uniform_int_distribution<Distance>;
        using param_type = typename distrib_type::param_type;
        distrib_type d;
        Distance sample_size = 0;
        while (first != last && sample_size != n)
        {
            out[sample_size++] = *first;
            ++first;
        }
        for (auto pop_size = sample_size; first != last; ++first, ++pop_size)
        {
            const auto k = d(g, param_type(0, pop_size)); // random number between 0 to pop_size
            if (k < n)
            {
                out[k] = *first;
            }
        }
        return out + sample_size;
    }
}

// unique
// complexity: exactly last-first applications of predicate
template<typename ForwardIterator>
constexpr ForwardIterator unique(ForwardIterator first, ForwardIterator last)
{
    if (first == last)
        return last;
    ForwardIterator result = first;
    while (++first != last)
    {
        if (!(*result == *first) && ++result != first)
        {
            *result = std::move(*first);
        }
    }
    return ++result;
}
template<typename ForwardIterator, typename BinaryPredicate>
constexpr ForwardIterator unique(ForwardIterator first, ForwardIterator last, BinaryPredicate p)
{
    if (first == last)
        return last;
    ForwardIterator result = first;
    while (++first != last)
    {
        if (!p(*result, *first) && ++result != first)
        {
            *result = std::move(*first);
        }
    }
    return ++result;
}

// unique_copy
// complexity: exactly last-first applications of predicate
template<typename InputIterator, typename OutputIterator>
constexpr OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator d_first)
{
    if (first == last)
        return d_first;
    if constexpr (is_forward_iterator_v<InputIterator>)
    {
        InputIterator next = first;
        *d_first = *first;
        while (++next != last)
        {
            if (!(*next == *first))
            {
                first = next;
                *++d_first = *first;
            }
        }
        return ++d_first;
    }
    else // input iterator
    {
        typename std::iterator_traits<InputIterator>::value_type value = *first;
        *d_first = value;
        while (!(++first == last))
        {
            if (!(*first == value))
            {
                value = *first;
                *++d_first = value;
            }
        }
        return ++d_first;
    }
}
template<typename InputIterator, typename OutputIterator, typename BinaryPredicate>
constexpr OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator d_first, BinaryPredicate p)
{
    if (first == last)
        return d_first;
    if constexpr (is_forward_iterator_v<InputIterator>)
    {
        InputIterator next = first;
        *d_first = *first;
        while (++next != last)
        {
            if (!p(*next, *first))
            {
                first = next;
                *++d_first = *first;
            }
        }
        return ++d_first;
    }
    else // input iterator
    {
        typename std::iterator_traits<InputIterator>::value_type value = *first;
        *d_first = value;
        while (!(++first == last))
        {
            if (!p(*first, value))
            {
                value = *first;
                *++d_first = value;
            }
        }
        return ++d_first;
    }
}

// ======================================== partitioning algorithms ================================================================================
// is_partitioned
// compelxity: at most last-first applications of predicate
template<typename InputIterator, typename UnaryPredicate>
constexpr bool is_partitioned(InputIterator first, InputIterator last, UnaryPredicate p)
{
    for (; first != last; ++first)
    {
        if (!p(*first))
        {
            break;
        }
    }
    for (; first != last; ++first)
    {
        if (p(*first))
        {
            return false;
        }
    }
    return true;
}

// partition: reorder element in range [first, last), elements that p returns true precede those p returns false, return iterator of first element that p returns false
// compelxity: last-first applications of predicate, at most last-first swaps
template<typename ForwardIterator, typename UnaryPredicate>
constexpr ForwardIterator partition(ForwardIterator first, ForwardIterator last, UnaryPredicate p)
{
    first = tstd::find_if_not(first, last, p);
    if (first == last)
    {
        return first;
    }
    for (ForwardIterator it = tstd::next(first); it != last; ++it)
    {
        if (p(*it))
        {
            tstd::iter_swap(first, it);
            ++first;
        }
    }
    return first;
}

// partition_copy: partition and copy two parts to two destination ranges
// complexity: exactly last-first applications of predicate
template<typename InputIterator, typename OutputIterator1, typename OutputIterator2, typename UnaryPredicate>
constexpr std::pair<OutputIterator1, OutputIterator2> partition_copy(InputIterator first, InputIterator last, OutputIterator1 d_first_true, OutputIterator2 d_first_false, UnaryPredicate p)
{
    for (; first != last; ++first)
    {
        if (p(*first))
        {
            *d_first_true++ = *first;
        }
        else
        {
            *d_first_false = *first;
        }
    }
    return {d_first_true, d_first_false};
}

// stable_partition: partition but stable
// complexity: exactly N applications of predicate and swaps, if memory is insufficient at most NlogN swaps,
//             at here we don't check memory, always N swaps, and it will try to allocate a temporary buffer.
template<typename BidirectionalIterator, typename UnaryPredicate>
BidirectionalIterator stable_partition(BidirectionalIterator first, BidirectionalIterator last, UnaryPredicate p)
{
    if (first == last)
    {
        return first;
    }
    using value_t = typename std::iterator_traits<BidirectionalIterator>::value_type;
    using diff_t = typename std::iterator_traits<BidirectionalIterator>::difference_type;
    diff_t N = tstd::distance(first, last);
    // with the help of a temporary buffer
    tstd::impl::TemporaryBuffer<BidirectionalIterator, value_t> buffer(first, N);
    auto buffer_iter = buffer.begin();
    auto rbuffer_iter = buffer.rbegin();
    for (auto iter = first; iter != last; ++iter)
    {
        if (p(*iter))
        {
            *buffer_iter++ = std::move(*iter);
        }
        else
        {
            *rbuffer_iter++ = std::move(*iter);
        }
    }
    // move to the source range
    BidirectionalIterator mid = tstd::move(buffer.begin(), buffer_iter, first);
    tstd::move(buffer.rbegin(), rbuffer_iter, mid);
    return mid;
}

// partition_point: get the iterator of first element that p returns false [that means the input range should be partitioned by p already.]
//                  it's a more general form of lower_bound, pass [&](auto const& e) { return e < value; } as predicate to represent lower_bound.
// complexity: N = std::distance(first, last), O(logN) applications of predicate, N applications of operator++ of iterator for non-random-access iterator.
template<typename ForwardIterator, typename UnaryPredicate>
constexpr ForwardIterator partition_point(ForwardIterator first, ForwardIterator last, UnaryPredicate p)
{
    ForwardIterator it;
    typename std::iterator_traits<ForwardIterator>::difference_type count, step;
    count = tstd::distance(first, last);
    while (count > 0)
    {
        it = first;
        step = count / 2;
        tstd::advance(it, step);
        if (p(*it)) // on the right of it
        {
            first = ++it;
            count -= step + 1;
        }
        else // on the left of it
        {
            count = step;
        }
    }
    return first;
}

// ======================================== sorting algorithms =====================================================================================
// is_sorted
// complexity: O(last-first)
template<typename ForwardIterator>
constexpr ForwardIterator is_sorted_until(ForwardIterator first, ForwardIterator last);
template<typename ForwardIterator, typename Compare>
constexpr ForwardIterator is_sorted_until(ForwardIterator first, ForwardIterator last, Compare comp);
template<typename ForwardIterator>
constexpr bool is_sorted(ForwardIterator first, ForwardIterator last) // 1
{
    return tstd::is_sorted_until(first, last) == last;
}
template<typename ForwardIterator, typename Compare>
constexpr bool is_sorted(ForwardIterator first, ForwardIterator last, Compare comp) // 2
{
    return tstd::is_sorted_until(first, last, comp) == last;
}

// is_sorted_until
// compelxity: O(last-first)
template<typename ForwardIterator>
constexpr ForwardIterator is_sorted_until(ForwardIterator first, ForwardIterator last) // 1
{
    return tstd::is_sorted_until(first, last, std::less<>());
}
template<typename ForwardIterator, typename Compare>
constexpr ForwardIterator is_sorted_until(ForwardIterator first, ForwardIterator last, Compare comp) // 2
{
    if (first != last)
    {
        ForwardIterator next = first;
        while (++next != last)
        {
            if (comp(*next, *first))
            {
                return next;
            }
            first = next;
        }
    }
    return last;
}

// sort: quick sort, not stable
// complexity: average O(NlogN)
template<typename RandomIterator>
constexpr void sort(RandomIterator first, RandomIterator last) // 1
{
    if (first == last)
    {
        return;
    }
    auto pivot = *tstd::next(first, tstd::distance(first, last) / 2);
    auto middle1 = tstd::partition(first, last, [pivot](const auto& elem) { return elem < pivot; });
    auto middle2 = tstd::partition(middle1, last, [pivot](const auto& elem) { return !(pivot < elem); });
    tstd::sort(first, middle1);
    tstd::sort(middle2, last);
}
template<typename RandomIterator, typename Compare>
constexpr void sort(RandomIterator first, RandomIterator last, Compare comp) // 2
{
    if (first == last)
    {
        return;
    }
    auto pivot = *tstd::next(first, tstd::distance(first, last) / 2);
    auto middle1 = tstd::partition(first, last, [pivot, &comp](const auto& elem) { return comp(elem, pivot); });
    auto middle2 = tstd::partition(middle1, last, [pivot, &comp](const auto& elem) { return !comp(pivot, elem); });
    tstd::sort(first, middle1, comp);
    tstd::sort(middle2, last, comp);
}

// partial_sort: sort [first, middle) subrange in range [first, last), not stable
// complexity: O((last-first)log(middle-first))
template<typename RandomIterator>
constexpr void partial_sort(RandomIterator first, RandomIterator middle, RandomIterator last) // 1
{
    if (first == last)
    {
        return;
    }
    auto N = tstd::distance(first, middle);
    auto pivot = *tstd::next(first, tstd::distance(first, last) / 2);
    auto middle1 = tstd::partition(first, last, [pivot](const auto& elem) { return elem < pivot; });
    auto middle2 = tstd::partition(middle1, last, [pivot](const auto& elem) { return !(pivot < elem); });
    if (tstd::distance(first, middle2) < N)
    {
        tstd::sort(first, middle1);
        tstd::partial_sort(middle2, tstd::next(first, N), last);
    }
    else if (tstd::distance(first, middle1) <= N)
    {
        tstd::sort(first, middle1);
    }
    else // middle1-frist > N
    {
        tstd::partial_sort(first, tstd::next(first, N), middle1);
    }
}
template<typename RandomIterator, typename Compare>
constexpr void partial_sort(RandomIterator first, RandomIterator middle, RandomIterator last, Compare comp) // 2
{
    if (first == last)
    {
        return;
    }
    auto N = tstd::distance(first, middle);
    auto pivot = *tstd::next(first, tstd::distance(first, last) / 2);
    auto middle1 = tstd::partition(first, last, [pivot, &comp](const auto& elem) { return comp(elem, pivot); });
    auto middle2 = tstd::partition(middle1, last, [pivot, &comp](const auto& elem) { return !comp(pivot, elem); });
    if (tstd::distance(first, middle2) < N)
    {
        tstd::sort(first, middle1, comp);
        tstd::partial_sort(middle2, tstd::next(first, N), last, comp);
    }
    else if (tstd::distance(first, middle1) <= N)
    {
        tstd::sort(first, middle1, comp);
    }
    else // middle1-frist > N
    {
        tstd::partial_sort(first, tstd::next(first, N), middle1, comp);
    }
}

// partial_sort_copy: partial_sort but copy to another range, use heap sort
// complexity: O((last-first)*log(min(last-first, d_last-d_first)))
template<typename InputIterator, typename RandomIterator>
constexpr RandomIterator partial_sort_copy(InputIterator first, InputIterator last, RandomIterator d_first, RandomIterator d_last) // 1
{
    if (first == last || d_first == d_last)
        return d_first;
    auto N1 = tstd::distance(first, last);
    auto N2 = tstd::distance(d_first, d_last);
    if (N1 <= N2)
    {
        tstd::copy(first, last, d_first);
        tstd::make_heap(d_first, d_first + N1);
        tstd::sort_heap(d_first, d_first + N1);
        return d_first + N1;
    }
    else // N1 > N2
    {
        // copy first N2 elements
        tstd::copy(first, first + N2, d_first);
        tstd::make_heap(d_first, d_last);
        // process rest elements
        for (auto iter = first + N2; iter != last; ++iter)
        {
            if (*iter < *d_first) // replace *d_first with *iter
            {
                tstd::pop_heap(d_first, d_last);
                *(d_last-1) = *iter;
                tstd::push_heap(d_first, d_last);
            }
            // else do nothing
        }
        tstd::sort_heap(d_first, d_last);
        return d_last;
    }
}
template<typename InputIterator, typename RandomIterator, typename Compare>
constexpr RandomIterator partial_sort_copy(InputIterator first, InputIterator last, RandomIterator d_first, RandomIterator d_last, Compare comp) // 2
{
    if (first == last || d_first == d_last)
        return d_first;
    auto N1 = tstd::distance(first, last);
    auto N2 = tstd::distance(d_first, d_last);
    if (N1 <= N2)
    {
        tstd::copy(first, last, d_first);
        tstd::make_heap(d_first, d_first + N1, comp);
        tstd::sort_heap(d_first, d_first + N1, comp);
        return d_first + N1;
    }
    else // N1 > N2
    {
        // copy first N2 elements
        tstd::copy(first, first + N2, d_first);
        tstd::make_heap(d_first, d_last, comp);
        // process rest elements
        for (auto iter = first + N2; iter != last; ++iter)
        {
            if (comp(*iter, *d_first)) // replace *d_first with *iter, rebuild the heap
            {
                tstd::pop_heap(d_first, d_last, comp);
                *(d_last-1) = *iter;
                tstd::push_heap(d_first, d_last, comp);
            }
            // else do nothing
        }
        tstd::sort_heap(d_first, d_last, comp);
        return d_last;
    }
}

// stable_sort: use merge sort
// complexity: O(N*log(N)^2) applications of comp, if additional memory is available then will be O(NlogN), always O(NlogN) in my implementation.
namespace impl
{
// stable_sort helpers
template<typename RandomIterator1, typename RandomIterator2, typename Compare>
void merge_sort(RandomIterator1 first, RandomIterator1 last, RandomIterator2 buf, Compare comp);
template<typename RandomIterator1, typename RandomIterator2>
void merge_sort(RandomIterator1 first, RandomIterator1 last, RandomIterator2 buf) // 1
{
    tstd::impl::merge_sort(first, last, buf, std::less<>());
}
template<typename RandomIterator1, typename RandomIterator2, typename Compare>
void merge_sort(RandomIterator1 first, RandomIterator1 last, RandomIterator2 buf, Compare comp) // 2
{
    if (last - first <= 1)
    {
        return;
    }
    auto mid = first + (last - first) / 2;
    // [first, mid) and [mid, last)
    merge_sort(first, mid, buf, comp);
    merge_sort(mid, last, buf, comp);
    auto iter1 = first;
    auto iter2 = mid;
    auto buf_first = buf;
    while (iter1 != mid && iter2 != last)
    {
        if (comp(*iter2, *iter1))
        {
            *buf_first++ = std::move(*iter2++);
        }
        else
        {
            *buf_first++ = std::move(*iter1++);
        }
    }
    if (iter1 != mid)
    {
        tstd::move(iter1, mid, buf_first);
    }
    if (iter2 != last)
    {
        tstd::move(iter2, last, buf_first);
    }
    tstd::move(buf, buf + (last-first), first);
}
}
template<typename RandomIterator>
void stable_sort(RandomIterator first, RandomIterator last) // 1
{
    if (first == last)
    {
        return;
    }
    using value_t = typename std::iterator_traits<RandomIterator>::value_type;
    auto N = last - first;
    // with the helper of a temporary buffer
    tstd::impl::TemporaryBuffer<RandomIterator, value_t> buffer(first, N);
    tstd::impl::merge_sort(first, last, buffer.begin());
}
template<typename RandomIterator, typename Compare>
void stable_sort(RandomIterator first, RandomIterator last, Compare comp) // 2
{
    if (first == last)
    {
        return;
    }
    using value_t = typename std::iterator_traits<RandomIterator>::value_type;
    auto N = last - first;
   // with the helper of a temporary buffer
    tstd::impl::TemporaryBuffer<RandomIterator, value_t> buffer(first, N);
    tstd::impl::merge_sort(first, last, buffer.begin(), comp);
}

// nth_element
// complexity: O(N) in average
template<typename RandomIterator>
constexpr void nth_element(RandomIterator first, RandomIterator nth, RandomIterator last) // 1
{
    if (first == last)
    {
        return;
    }
    auto N = nth - first;
    auto pivot = *tstd::next(first, tstd::distance(first, last) / 2);
    auto middle1 = tstd::partition(first, last, [pivot](const auto& elem) { return elem < pivot; });
    auto middle2 = tstd::partition(middle1, last, [pivot](const auto& elem) { return !(pivot < elem); });
    if (tstd::distance(first, middle2) < N)
    {
        tstd::nth_element(middle2, nth, last);
    }
    else if (tstd::distance(first, middle1) <= N)
    {
        return;
    }
    else // middle1-frist > N
    {
        tstd::nth_element(first, nth, middle1);
    }
}
template<typename RandomIterator, typename Compare>
constexpr void nth_element(RandomIterator first, RandomIterator nth, RandomIterator last, Compare comp) // 2
{
    if (first == last)
    {
        return;
    }
    auto N = nth - first;
    auto pivot = *tstd::next(first, tstd::distance(first, last) / 2);
    auto middle1 = tstd::partition(first, last, [pivot, &comp](const auto& elem) { return comp(elem, pivot); });
    auto middle2 = tstd::partition(middle1, last, [pivot, &comp](const auto& elem) { return !comp(pivot, elem); });
    if (tstd::distance(first, middle2) < N)
    {
        tstd::nth_element(middle2, nth, last, comp);
    }
    else if (tstd::distance(first, middle1) <= N)
    {
        return;
    }
    else // middle1-frist > N
    {
        tstd::nth_element(first, nth, middle1, comp);
    }
}

// ======================================== binary search algorithms (on sorted ranges)=============================================================
// lower_bound: find first element that >= value
// complexity: O(log(last-first))
template<typename ForwardIterator, typename T>
constexpr ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value) // 1
{
    ForwardIterator it;
    typename std::iterator_traits<ForwardIterator>::difference_type count, step;
    count = tstd::distance(first, last);
    while (count > 0)
    {
        it = first;
        step = count / 2;
        tstd::advance(it, step);
        if (*it < value)
        {
            first = ++it;
            count -= step + 1;
        }
        else
        {
            count = step;
        }
    }
    return first;
}
template<typename ForwardIterator, typename T, typename Compare>
constexpr ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) // 2
{
    ForwardIterator it;
    typename std::iterator_traits<ForwardIterator>::difference_type count, step;
    count = tstd::distance(first, last);
    while (count > 0)
    {
        it = first;
        step = count / 2;
        tstd::advance(it, step);
        if (comp(*it, value))
        {
            first = ++it;
            count -= step + 1;
        }
        else
        {
            count = step;
        }
    }
    return first;
}

// upper_bound: find first element that > value
// compelxity: O(log(last-first))
template<typename ForwardIterator, typename T>
constexpr ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value) // 1
{
    ForwardIterator it;
    typename std::iterator_traits<ForwardIterator>::difference_type count, step;
    count = tstd::distance(first, last);
    while (count > 0)
    {
        it = first;
        step = count / 2;
        tstd::advance(it, step);
        if (value < *it)
        {
            count = step;
        }
        else
        {
            first = ++it;
            count -= step + 1;
        }
    }
    return first;
}
template<typename ForwardIterator, typename T, typename Compare>
constexpr ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) // 2
{
    ForwardIterator it;
    typename std::iterator_traits<ForwardIterator>::difference_type count, step;
    count = tstd::distance(first, last);
    while (count > 0)
    {
        it = first;
        step = count / 2;
        tstd::advance(it, step);
        if (comp(value, *it))
        {
            count = step;
        }
        else
        {
            first = ++it;
            count -= step + 1;
        }
    }
    return first;
}

// binary_search: find if the element is in range [first, last)
// complexity: O(log(last-first))
template<typename ForwardIterator, typename T>
constexpr bool binary_search(ForwardIterator first, ForwardIterator last, const T& value) // 1
{
    first = tstd::lower_bound(first, last, value);
    return (first != last) && !(value < *first);
}
template<typename ForwardIterator, typename T, typename Compare>
constexpr bool binary_search(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) // 2
{
    first = tstd::lower_bound(first, last, value, comp);
    return (first != last) && !comp(value, *first);
}

// equal_range
// complexity: O(log(last-first))
template<typename ForwardIterator, typename T>
std::pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value) // 1
{
    return {tstd::lower_bound(first, last, value), tstd::upper_bound(first, last, value)};
}
template<typename ForwardIterator, typename T, typename Compare>
std::pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) // 2
{
    return {tstd::lower_bound(first, last, value, comp), tstd::upper_bound(first, last, value, comp)};
}

// ======================================== algorithms on sorted ranges ============================================================================
// merge: merge two sorted ranges to destination
// complexity: at most last1-first1 + last2-first2 - 1 comparisons
// it's stable, for equal elements, element from first range precede element from the second.
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
constexpr OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator d_first) // 1
{
    for (; first1 != last1; ++d_first)
    {
        if (first2 == last2)
        {
            return tstd::copy(first1, last1, d_first);
        }
        if (*first2 < *first1)
        {
            *d_first = *first2++;
        }
        else
        {
            *d_first = *first1++;
        }
    }
    return tstd::copy(first2, last2, d_first);
}
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
constexpr OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator d_first, Compare comp) // 2
{
    for (; first1 != last1; ++d_first)
    {
        if (first2 == last2)
        {
            return tstd::copy(first1, last1, d_first);
        }
        if (comp(*first2, *first1))
        {
            *d_first = *first2++;
        }
        else
        {
            *d_first = *first1++;
        }
    }
    return tstd::copy(first2, last2, d_first);
}

// inplace_merge: merge two consecutive sorted ranges [first, middle) and [middle, last) to one sorted range [first, last)
//               it's also stable, for equal elements, element from first range precede element from the second.
// complexity: if extra memory is available, will be exactly last-first-1 comparisons, or else O(NlogN)
template<typename BidirectionalIterator>
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last) // 1
{
    using value_t = typename std::iterator_traits<BidirectionalIterator>::value_type;
    auto N = last - first;
    auto iter1 = first;
    auto iter2 = middle;
    tstd::impl::TemporaryBuffer<BidirectionalIterator, value_t> buffer(first, N);
    auto d_iter = buffer.begin();
    for (; iter1 != middle && iter2 != last; ++d_iter)
    {
        if (*iter2 < *iter1)
        {
            *d_iter = std::move(*iter2++);
        }
        else
        {
            *d_iter = std::move(*iter1++);
        }
    }
    if (iter1 != middle)
    {
        tstd::move(iter1, middle, d_iter);
    }
    if (iter2 != last)
    {
        tstd::move(iter2, last, d_iter);
    }
    tstd::move(buffer.begin(), buffer.end(), first);
}
template<typename BidirectionalIterator, typename Compare>
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Compare comp) // 2
{
    using value_t = typename std::iterator_traits<BidirectionalIterator>::value_type;
    auto N = last - first;
    auto iter1 = first;
    auto iter2 = middle;
    tstd::impl::TemporaryBuffer<BidirectionalIterator, value_t> buffer(first, N);
    auto d_iter = buffer.begin();
    for (; iter1 != middle && iter2 != last; ++d_iter)
    {
        if (comp(*iter2, *iter1))
        {
            *d_iter = std::move(*iter2++);
        }
        else
        {
            *d_iter = std::move(*iter1++);
        }
    }
    if (iter1 != middle)
    {
        tstd::move(iter1, middle, d_iter);
    }
    if (iter2 != last)
    {
        tstd::move(iter2, last, d_iter);
    }
    tstd::move(buffer.begin(), buffer.end(), first);
}

// ======================================== set algorithms (on sorted ranges) ======================================================================
// set algorithms: use sorted range represent set

// includes: if range [first1, last1) includes range [first2, last2)
// complexity: at most 2*(N1+N2-1) comparisons where N1 = last1-first1, N2 = last2-first2
template<typename InputIterator1, typename InputIterator2>
constexpr bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) // 1
{
    while (first1 != last1 && first2 != last2)
    {
        if (*first1 < *first2)
        {
            first1++;
        }
        else if (*first2 < *first1)
        {
            return false;
        }
        else // *first1 == *first2
        {
            first1++;
            first2++;
        }
    }
    return first2 == last2;
}
template<typename InputIterator1, typename InputIterator2, typename Compare>
constexpr bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) // 2
{
    while (first1 != last1 && first2 != last2)
    {
        if (comp(*first1, *first2))
        {
            first1++;
        }
        else if (comp(*first2, *first1))
        {
            return false;
        }
        else // *first1 == *first2
        {
            first1++;
            first2++;
        }
    }
    return first2 == last2;
}

// set_difference: the difference of range1 and range2, aka [first1, last1) - [first2, last2)
// at most 2*(N1+N2)-1 comparisons where N1 = last1-first1, N2 = last2-first2
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
constexpr OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator d_first) // 1
{
    while (first1 != last1 && first2 != last2)
    {
        if (*first1 < *first2)
        {
            *d_first++ = *first1++;
        }
        else if (*first2 < *first1)
        {
            first2++;
        }
        else // *first1 == *first2
        {
            first1++;
            first2++;
        }
    }
    if (first1 != last1)
    {
        d_first = std::copy(first1, last1, d_first);
    }
    return d_first;
}
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
constexpr OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator d_first, Compare comp) // 2
{
    while (first1 != last1 && first2 != last2)
    {
        if (comp(*first1, *first2))
        {
            *d_first++ = *first1++;
        }
        else if (comp(*first2, *first1))
        {
            first2++;
        }
        else // *first1 == *first2
        {
            first1++;
            first2++;
        }
    }
    if (first1 != last1)
    {
        d_first = std::copy(first1, last1, d_first);
    }
    return d_first;
}

// set_intersection: intersection of two set2 [first1, last1) and [first2, last2)
// complexity: at most 2*(N1+N2)-1 comparisons where N1 = last1-first1, N2 = last2-first2
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
constexpr OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator d_first) // 1
{
    while (first1 != last1 && first2 != last2)
    {
        if (*first1 < *first2)
        {
            first1++;
        }
        else if (*first2 < *first1)
        {
            first2++;
        }
        else
        {
            *d_first++ = *first1;
            first1++;
            first2++;
        }
    }
    return d_first;
}
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
constexpr OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator d_first, Compare comp) // 2
{
    while (first1 != last1 && first2 != last2)
    {
        if (comp(*first1, *first2))
        {
            first1++;
        }
        else if (comp(*first2, *first1))
        {
            first2++;
        }
        else
        {
            *d_first++ = *first1;
            first1++;
            first2++;
        }
    }
    return d_first;
}

// set_symmetric_difference: symmetric difference of range1 and range2
// complexity: at most 2*(N1+N2)-1 comparisons where N1 = last1-first1, N2 = last2-first2
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
constexpr OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator d_first) // 1
{
    while (first1 != last1 && first2 != last2)
    {
        if (*first1 < *first2)
        {
            *d_first++ = *first1++;
        }
        else if (*first2 < *first1)
        {
            *d_first++ = *first2++;
        }
        else // *first1 == *first2
        {
            first1++;
            first2++;
        }
    }
    if (first1 != last1)
    {
        d_first = tstd::copy(first1, last1, d_first);
    }
    else if (first2 != last2)
    {
        d_first = tstd::copy(first2, last2, d_first);
    }
    return d_first;
}
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
constexpr OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator d_first, Compare comp) // 2
{
    while (first1 != last1 && first2 != last2)
    {
        if (comp(*first1, *first2))
        {
            *d_first++ = *first1++;
        }
        else if (comp(*first2, *first1))
        {
            *d_first++ = *first2++;
        }
        else // *first1 == *first2
        {
            first1++;
            first2++;
        }
    }
    if (first1 != last1)
    {
        d_first = tstd::copy(first1, last1, d_first);
    }
    else if (first2 != last2)
    {
        d_first = tstd::copy(first2, last2, d_first);
    }
    return d_first;
}

// set_union: union of two sets
// complexity: at most 2*(N1+N2)-1 comparisons where N1 = last1-first1, N2 = last2-first2
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
constexpr OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator d_first) // 1
{
    while (first1 != last1 && first2 != last2)
    {
        if (*first1 < *first2)
        {
            *d_first++ = *first1++;
        }
        else if (*first2 < *first1)
        {
            *d_first++ = *first2++;
        }
        else // *first == *first2
        {
            *d_first++ = *first1;
            first1++;
            first2++;
        }
    }
    if (first1 != last1)
    {
        d_first = tstd::copy(first1, last1, d_first);
    }
    else if (first2 != last2)
    {
        d_first = tstd::copy(first2, last2, d_first);
    }
    return d_first;
}
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
constexpr OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator d_first, Compare comp) // 2
{
    while (first1 != last1 && first2 != last2)
    {
        if (comp(*first1, *first2))
        {
            *d_first++ = *first1++;
        }
        else if (comp(*first2, *first1))
        {
            *d_first++ = *first2++;
        }
        else // *first == *first2
        {
            *d_first++ = *first1;
            first1++;
            first2++;
        }
    }
    if (first1 != last1)
    {
        d_first = tstd::copy(first1, last1, d_first);
    }
    else if (first2 != last2)
    {
        d_first = tstd::copy(first2, last2, d_first);
    }
    return d_first;
}

// ======================================== heap algorithms (in <tstl_heap.hpp>) ===================================================================

// ======================================== minimum/maximum algorithms =============================================================================
// max: return greater one of given values
// complexity: exactly one comparison
template<typename T>
constexpr const T& max(const T& a, const T& b) // 1
{
    return a < b ? b : a;
}
template<typename T, typename Compare>
constexpr const T& max(const T& a, const T& b, Compare comp) // 2
{
    return comp(a, b) ? b : a;
}
// complexity: exactly il.size()-1 comparisons
template<typename ForwardIterator>
constexpr ForwardIterator max_element(ForwardIterator first, ForwardIterator last);
template<typename ForwardIterator, typename Compare>
constexpr ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp);
template<typename T>
constexpr T max(std::initializer_list<T> il) // 3
{
    return *tstd::max_element(il.begin(), il.end());
}
template<typename T, typename Compare>
constexpr T max(std::initializer_list<T> il, Compare comp) // 4
{
    return *tstd::max_element(il.begin(), il.end(), comp);
}

// max_element: find max element in range [first, last)
// complexity: exactly max(last-first, 0) comparisons
template<typename ForwardIterator>
constexpr ForwardIterator max_element(ForwardIterator first, ForwardIterator last) // 1
{
    if (first == last)
        return last;
    ForwardIterator largest = first;
    ++first;
    for (; first != last; ++first)
    {
        if (*largest < *first)
        {
            largest = first;
        }
    }
    return largest;
}
template<typename ForwardIterator, typename Compare>
constexpr ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) // 2
{
    if (first == last)
        return last;
    ForwardIterator largest = first;
    ++first;
    for (; first != last; ++first)
    {
        if (comp(*largest, *first))
        {
            largest = first;
        }
    }
    return largest;
}

// max: return lesser one of given values
// complexity: exactly one comparison
template<typename T>
constexpr const T& min(const T& a, const T& b) // 1
{
    return b < a ? b : a;
}
template<typename T, typename Compare>
constexpr const T& min(const T& a, const T& b, Compare comp) // 2
{
    return comp(b, a) ? b : a;
}
// complexity: exactly il.size()-1 comparisons
template<typename ForwardIterator, typename Compare>
constexpr ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp);
template<typename ForwardIterator, typename Compare>
constexpr ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp);
template<typename T>
constexpr T min(std::initializer_list<T> il) // 3
{
    return *tstd::min_element(il.begin(), il.end());
}
template<typename T, typename Compare>
constexpr T min(std::initializer_list<T> il, Compare comp) // 4
{
    return *tstd::min_element(il.begin(), il.end(), comp);
}

// min_element: find max element in range [first, last)
// complexity: exactly max(last-first, 0) comparisons
template<typename ForwardIterator>
constexpr ForwardIterator min_element(ForwardIterator first, ForwardIterator last) // 1
{
    if (first == last)
        return last;
    ForwardIterator smallest = first;
    ++first;
    for (; first != last; ++first)
    {
        if (*first < *smallest)
        {
            smallest = first;
        }
    }
    return smallest;
}
template<typename ForwardIterator, typename Compare>
constexpr ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp) // 2
{
    if (first == last)
        return last;
    ForwardIterator smallest = first;
    ++first;
    for (; first != last; ++first)
    {
        if (comp(*first, *smallest))
        {
            smallest = first;
        }
    }
    return smallest;
}

// minmax: return the (min, max) pair of given values
// complexity: exactly one comparison
template<typename T>
constexpr std::pair<const T&, const T&> minmax(const T& a, const T& b) // 1
{
    return b < a ? std::pair<const T&, const T&>{b, a} : std::pair<const T&, const T&>{a, b};
}
template<typename T, typename Compare>
constexpr std::pair<const T&, const T&> minmax(const T& a, const T& b, Compare comp) // 2
{
    return comp(b, a) ? std::pair<const T&, const T&>{b, a} : std::pair<const T&, const T&>{a, b};
}
// complexity: at most il.size()*2 comparisons
template<typename ForwardIterator>
constexpr std::pair<ForwardIterator, ForwardIterator> minmax_element(ForwardIterator first, ForwardIterator last);
template<typename ForwardIterator, typename Compare>
constexpr std::pair<ForwardIterator, ForwardIterator> minmax_element(ForwardIterator first, ForwardIterator last, Compare comp);
template<typename T>
constexpr std::pair<T, T> minmax(std::initializer_list<T> il) // 3
{
    auto p = tstd::minmax_element(il.begin(), il.end());
    return {*p.first, *p.second};
}
template<typename T, typename Compare>
constexpr std::pair<T, T> minmax(std::initializer_list<T> il, Compare comp) // 4
{
    auto p = tstd::minmax_element(il.begin(), il.end(), comp);
    return {*p.first, *p.second};
}

// minmax_element: return minimum and maximum element of a sequence
// complexity: O(2N) applications of predicate
template<typename ForwardIterator>
constexpr std::pair<ForwardIterator, ForwardIterator> minmax_element(ForwardIterator first, ForwardIterator last) // 1
{
    return std::minmax_element(first, last, std::less<>());
}
template<typename ForwardIterator, typename Compare>
constexpr std::pair<ForwardIterator, ForwardIterator> minmax_element(ForwardIterator first, ForwardIterator last, Compare comp) // 2
{
    auto min = first, max = first;
    if (first == last)
    {
        return {min, max};
    }
    while (++first != last)
    {
        if (comp(*first, *min))
        {
            min = first;
        }
        if (comp(*max, *first))
        {
            max = first;
        }
    }
    return {min, max};
}

// clamp
// complexity: at most two comparisons
template<typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) // 1
{
    assert(!(hi < lo));
    return v < lo ? lo : hi < v ? hi : v;
}
template<typename T, typename Compare>
constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp) // 2
{
    assert(!comp(hi, lo));
    return comp(v, lo) ? lo : comp(hi, v) ? hi : v; 
}

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
    return tstd::lexicographical_compare_three_way(first1, last1, first2, last2, std::compare_three_way());
}

// ======================================== permutation algorithms =================================================================================
// is_permutation
// complexity: at most O(N^2) applications of predicate
// predicate is equivalence test
template<typename ForwardIterator1, typename ForwardIterator2>
constexpr bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
{
    // skip common prefix
    std::tie(first1, first2) = tstd::mismatch(first1, last1, first2);
    // iterate over rest elements, count how many times each element appears in [first2, last2)
    if (first1 != last1)
    {
        ForwardIterator2 last2 = first2;
        tstd::advance(last2, tstd::distance(first1, last1));
        for (ForwardIterator1 iter = first1; iter != last1; ++iter)
        {
            // *iter has been checked
            if (iter != tstd::find(first1, iter, *iter))
                continue;
            auto ct = tstd::count(first2, last2, *iter);
            if (ct == 0 || tstd::count(iter, last1, *iter) != ct)
            {
                return false;
            }
        }
    }
    return true;
}
template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
constexpr bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, BinaryPredicate p)
{
    // skip common prefix
    std::tie(first1, first2) = tstd::mismatch(first1, last1, first2);
    // iterate over rest elements, count how many times each element appears in [first2, last2)
    if (first1 != last1)
    {
        ForwardIterator2 last2 = first2;
        tstd::advance(last2, tstd::distance(first1, last1));
        for (ForwardIterator1 iter = first1; iter != last1; ++iter)
        {
            auto pred = [iter, &p](auto& val) { return p(val, *iter); };
            // *iter has been checked
            if (iter != tstd::find_if(first1, iter, pred))
                continue;
            auto ct = tstd::count(first2, last2, *iter);
            if (ct == 0 || tstd::count_if(iter, last1, pred) != ct)
            {
                return false;
            }
        }
    }
    return true;
}
template<typename ForwardIterator1, typename ForwardIterator2>
constexpr bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
{
    if (std::distance(first1, last1) != std::distance(first2, last2))
    {
        return false;
    }
    // skip common prefix
    std::tie(first1, first2) = tstd::mismatch(first1, last1, first2);
    // iterate over rest elements, count how many times each element appears in [first2, last2)
    if (first1 != last1)
    {
        for (ForwardIterator1 iter = first1; iter != last1; ++iter)
        {
            // *iter has been checked
            if (iter != tstd::find(first1, iter, *iter))
                continue;
            auto ct = tstd::count(first2, last2, *iter);
            if (ct == 0 || tstd::count(iter, last1, *iter) != ct)
            {
                return false;
            }
        }
    }
    return true;
}
template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
constexpr bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate p)
{
    if (std::distance(first1, last1) != std::distance(first2, last2))
    {
        return false;
    }
    // skip common prefix
    std::tie(first1, first2) = tstd::mismatch(first1, last1, first2);
    // iterate over rest elements, count how many times each element appears in [first2, last2)
    if (first1 != last1)
    {
        for (ForwardIterator1 iter = first1; iter != last1; ++iter)
        {
            auto pred = [iter, &p](auto& val) { return p(val, *iter); };
            // *iter has been checked
            if (iter != tstd::find_if(first1, iter, pred))
                continue;
            auto ct = tstd::count(first2, last2, *iter);
            if (ct == 0 || tstd::count_if(iter, last1, pred) != ct)
            {
                return false;
            }
        }
    }
    return true;
}

// next_permutation: get next permutation in lexicographical order, return true if exist, or else return false(in this case, will get the first permutation)
// complexity: at most N/2 swaps
template<typename BidirectionalIterator>
constexpr bool next_permutation(BidirectionalIterator first, BidirectionalIterator last)
{
    // no element
    if (first == last)
        return false;
    // only one element
    BidirectionalIterator iter = last;
    if (first == --iter)
        return false;
    while (true)
    {
        BidirectionalIterator iter1, iter2;
        iter1 = iter;
        if (*--iter < *iter1)
        {
            iter2 = last;
            while (!(*iter < *--iter2)) {}
            tstd::iter_swap(iter, iter2);
            tstd::reverse(iter1, last);
            return true;
        }
        if (iter == first)
        {
            tstd::reverse(first, last);
            return false;
        }
    }
}
template<typename BidirectionalIterator, typename Compare>
constexpr bool next_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp)
{
    if (first == last)
        return false;
    BidirectionalIterator iter = last;
    if (first == --iter)
        return false;
    while (true)
    {
        BidirectionalIterator iter1, iter2;
        iter1 = iter;
        if (comp(*--iter, *iter1))
        {
            iter2 = last;
            while (!comp(*iter, *--iter2)) {}
            tstd::iter_swap(iter, iter2);
            tstd::reverse(iter1, last);
            return true;
        }
        if (iter == first)
        {
            tstd::reverse(first, last);
            return false;
        }
    }
}

// perv_permutation: get pervious permutation in lexicographical order, return true if exist, or else return false(in this case, will get the last permutation)
// complexity: at most N/2 swaps
template<typename BidirectionalIterator>
constexpr bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last)
{
    if (first == last)
        return false;
    BidirectionalIterator iter = last;
    if (first == --iter)
        return false;
    while (true)
    {
        BidirectionalIterator iter1, iter2;
        iter1 = iter;
        if (*iter1 < *--iter)
        {
            iter2 = last;
            while (!(*--iter2 < *iter)) {}
            tstd::iter_swap(iter, iter2);
            tstd::reverse(iter1, last);
            return true;
        }
        if (iter == first)
        {
            tstd::reverse(first, last);
            return false;
        }
    }
}
template<typename BidirectionalIterator, typename Compare>
constexpr bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp)
{
    if (first == last)
        return false;
    BidirectionalIterator iter = last;
    if (first == --iter)
        return false;
    while (true)
    {
        BidirectionalIterator iter1, iter2;
        iter1 = iter;
        if (comp(*iter1, *--iter))
        {
            iter2 = last;
            while (!comp(*--iter2, *iter)) {}
            tstd::iter_swap(iter, iter2);
            tstd::reverse(iter1, last);
            return true;
        }
        if (iter == first)
        {
            tstd::reverse(first, last);
            return false;
        }
    }
}

} // namespace tstd

#endif // TALGORITHM_HPP