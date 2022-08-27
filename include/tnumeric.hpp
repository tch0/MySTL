#ifndef TNUMERIC_HPP
#define TNUMERIC_HPP

#include <utility>
#include <type_traits>
#include <tstl_heap.hpp>
#include <tutility.hpp>

namespace tstd
{

// numeric algorithms
// iota: fill the range [first, last) with sequentially increasing values, starting with value and repetitively evaluating ++value
// complexity: last-first applications of operator++ and assignment
template<typename ForwardIterator, typename T>
constexpr void iota(ForwardIterator first, ForwardIterator last, T value)
{
    while (first != last)
    {
        *first++ = value;
        ++value;
    }
}

// accumulate: computes the sum of given initial value and the elements in range [first, last)
// complexity: last-first applications of operator+/binary operation and assignment of T
template<typename InputIterator, typename T>
constexpr T accumulate(InputIterator first, InputIterator last, T init) // 1
{
    for (; first != last; ++first)
    {
        init = std::move(init) + *first;
    }
    return init;
}
template<typename InputIterator, typename T, typename BinaryOperation>
constexpr T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation op) // 2
{
    for (; first != last; ++first)
    {
        init = op(std::move(init), *first);
    }
    return init;
}

// inner_product: compute the inner product of range [first1, last1) and the range begin at first2
// complexity: last1-first1 appliation of multiply and addition / op1 and op2 and assgnment
template<typename InputIterator1, typename InputIterator2, typename T>
constexpr T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) // 1
{
    for (; first1 != last1; ++first1, ++first2)
    {
        init = std::move(init) + *first1 * *first2;
    }
    return init;
}
template<typename InputIterator1, typename InputIterator2, typename T, typename BinaryOperation1, typename BinaryOperation2>
constexpr T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation1 op1, BinaryOperation2 op2) // 2
{
    for (; first1 != last1; ++first1, ++first2)
    {
        init = op1(std::move(init), op2(*first1, *first2));
    }
    return init;
}

// ajacent_difference: compute the differences between the second and the first of each adjacent pair of elements of the range [first, last)
//                     and write them to the range begining at d_first+1, and unmodified copy of *first1 is written to *d_firsts
// complexity: exactly last-first-1 applications of binary function
template<typename InputIterator, typename OutputIterator>
constexpr OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator d_first) // 1
{
    if (first == last)
        return d_first;
    using value_t = typename std::iterator_traits<InputIterator>::value_type;
    value_t acc = *first;
    *d_first = acc;
    while (++first != last)
    {
        value_t val = *first;
        *++d_first = val - std::move(acc);
        acc = std::move(val);
    }
    return ++d_first;
}
template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
constexpr OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator d_first, BinaryOperation op) // 2
{
    if (first == last)
        return d_first;
    using value_t = typename std::iterator_traits<InputIterator>::value_type;
    value_t acc = *first;
    *d_first = acc;
    while (++first != last)
    {
        value_t val = *first;
        *++d_first = op(val, std::move(acc));
        acc = std::move(val);
    }
    return ++d_first;
}

// partial_sum: computes the partial sums of the elements in the subranges of the range [first, last) and write them to the range begin at d_first
// complexity: exactly last-first-1 applications of binary function
template<typename InputIterator, typename OutputIterator>
constexpr OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator d_first) // 1
{
    if (first == last)
        return d_first;
    typename std::iterator_traits<InputIterator>::value_type sum = *first;
    *d_first = sum;
    while (++first != last)
    {
        sum = std::move(sum) + *first;
        *++d_first = sum;
    }
    return ++d_first;
}
template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
constexpr OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator d_first, BinaryOperation op) // 2
{
    if (first == last)
        return d_first;
    typename std::iterator_traits<InputIterator>::value_type sum = *first;
    *d_first = sum;
    while (++first != last)
    {
        sum = op(std::move(sum), *first);
        *++d_first = sum;
    }
    return ++d_first;
}

// reduce: generalized sum of init and *first, ..., *(last-1) over binary operation
// complexity: O(last-first) applications of binary operition
// just like accumulate but support parallel execution (but parallel execution is not supported here, so technically no difference here)
template<typename InputIterator, typename T, typename BinaryOperation>
constexpr typename std::iterator_traits<InputIterator>::value_type reduce(InputIterator first, InputIterator last, T init, BinaryOperation op);
template<typename InputIterator>
constexpr typename std::iterator_traits<InputIterator>::value_type reduce(InputIterator first, InputIterator last) // 1
{
    using value_t = typename std::iterator_traits<InputIterator>::value_type;
    return tstd::reduce(first, last, value_t{}, std::plus<value_t>());
}
template<typename InputIterator, typename T>
constexpr typename std::iterator_traits<InputIterator>::value_type reduce(InputIterator first, InputIterator last, T init) // 2
{
    return tstd::reduce(first, last, init, std::plus<T>());
}
template<typename InputIterator, typename T, typename BinaryOperation>
constexpr typename std::iterator_traits<InputIterator>::value_type reduce(InputIterator first, InputIterator last, T init, BinaryOperation op) // 3
{
    for (; first != last; ++first)
    {
        init = op(init, *first);
    }
    return init;
}

// exclusive_scan: just like partial_sum, but i-th sum exlude i-th element, and has parellel version
// complexity: O(last-first) applications of binary operition
template<typename InputIterator, typename OutputIterator, typename T, typename BinaryOperation>
constexpr OutputIterator exclusive_scan(InputIterator first, InputIterator last, OutputIterator d_first, T init, BinaryOperation op);
template<typename InputIterator, typename OutputIterator, typename T>
constexpr OutputIterator exclusive_scan(InputIterator first, InputIterator last, OutputIterator d_first, T init) // 1
{
    return tstd::exclusive_scan(first, last, d_first, init, std::plus<T>());
}
template<typename InputIterator, typename OutputIterator, typename T, typename BinaryOperation>
constexpr OutputIterator exclusive_scan(InputIterator first, InputIterator last, OutputIterator d_first, T init, BinaryOperation op) // 2
{
    if (first == last)
        return d_first;
    for (; first != last; ++first, ++d_first)
    {
        *d_first = init;
        init = op(init, *first);
    }
    return d_first;
}

// inclusive_scan: just like partial_sum, i-th sum include i-th element, and has parellel version
// complexity: O(last-first) applications of binary operition
template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
constexpr OutputIterator inclusive_scan(InputIterator first, InputIterator last, OutputIterator d_first, BinaryOperation op);
template<typename InputIterator, typename OutputIterator>
constexpr OutputIterator inclusive_scan(InputIterator first, InputIterator last, OutputIterator d_first) // 1
{
    using value_t = typename std::iterator_traits<InputIterator>::value_type;
    return tstd::inclusive_scan(first, last, d_first, std::plus<value_t>());
}
// version 2: *d_first = *first (not op(value_t{}, *first))
template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
constexpr OutputIterator inclusive_scan(InputIterator first, InputIterator last, OutputIterator d_first, BinaryOperation op) // 2
{
    if (first == last)
        return d_first;
    typename std::iterator_traits<InputIterator>::value_type sum = *first;
    *d_first = sum;
    while (++first != last)
    {
        sum = op(sum, *first);
        *++d_first = sum;
    }
    return ++d_first;
}
// *d_first = op(init, *first)
template<typename InputIterator, typename OutputIterator, typename BinaryOperation, typename T>
constexpr OutputIterator inclusive_scan(InputIterator first, InputIterator last, OutputIterator d_first, BinaryOperation op, T init) // 3
{
    if (first == last)
        return d_first;
    for (; first != last; ++first, ++d_first)
    {
        init = op(init, *first);
        *d_first = init;
    }
    return d_first;
}

// transform_reduce: transform and reduce
// complexity: O(last-first)
// first version: as inner_product
template<typename InputIterator1, typename InputIterator2, typename T, typename BinaryOperation1, typename BinaryOperation2>
constexpr T transform_reduce(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation1 op1, BinaryOperation2 op2);
template<typename InputIterator1, typename InputIterator2, typename T>
constexpr T transform_reduce(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) // 1
{
    return tstd::transform_reduce(first1, last1, first2, init, std::plus<T>(), std::multiplies<T>()); // call version 2
}
template<typename InputIterator1, typename InputIterator2, typename T, typename BinaryOperation1, typename BinaryOperation2>
constexpr T transform_reduce(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation1 op1, BinaryOperation2 op2) // 2
{
    for (; first1 != last1; ++first1, ++first2)
    {
        init = op1(init, op2(*first1, *first2));
    }
    return init;
}
template<typename InputIterator1, typename T, typename BinaryOperation, typename UnaryOperation>
constexpr T transform_reduce(InputIterator1 first, InputIterator1 last, T init, BinaryOperation bop, UnaryOperation uop) // 3
{
    for (; first != last; ++first)
    {
        init = bop(init, uop(*first));
    }
    return init;
}

// transform_exclusive_scan: transform then compute inclusive_scan
// complexity: O(last - first) applications of bop and uop
template<typename InputIterator, typename OutputIterator, typename T, typename BinaryOperation, typename UnaryOperation>
constexpr OutputIterator transform_exclusive_scan(InputIterator first, InputIterator last, OutputIterator d_first, T init, BinaryOperation bop, UnaryOperation uop)
{
    if (first == last)
        return d_first;
    for (; first != last; ++first, ++d_first)
    {
        *d_first = init;
        init = bop(init, uop(*first));
    }
    return d_first;
}

// transform_inclusive_scan: transform then compute inclusive_scans
// complexity: O(last - first) applications of bop and uop
template<typename InputIterator, typename OutputIterator, typename BinaryOperation, typename UnaryOperation, typename T>
constexpr OutputIterator transform_inclusive_scan(InputIterator first, InputIterator last, OutputIterator d_first, BinaryOperation bop, UnaryOperation uop, T init);
template<typename InputIterator, typename OutputIterator, typename BinaryOperation, typename UnaryOperation>
constexpr OutputIterator transform_inclusive_scan(InputIterator first, InputIterator last, OutputIterator d_first, BinaryOperation bop, UnaryOperation uop) // 1
{
    if (first == last)
        return d_first;
    typename std::iterator_traits<InputIterator>::value_type sum = uop(*first);
    *d_first = sum;
    while (++first != last)
    {
        sum = bop(sum, uop(*first));
        *++d_first = sum;
    }
    return ++d_first;
}
template<typename InputIterator, typename OutputIterator, typename BinaryOperation, typename UnaryOperation, typename T>
constexpr OutputIterator transform_inclusive_scan(InputIterator first, InputIterator last, OutputIterator d_first, BinaryOperation bop, UnaryOperation uop, T init) // 2
{
    if (first == last)
        return d_first;
    for (; first != last; ++first, ++d_first)
    {
        init = bop(init, uop(*first));
        *d_first = init;
    }
    return d_first;
}

} // namespace tstd


#endif // TNUMERIC_HPP