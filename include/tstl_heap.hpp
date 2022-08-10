#ifndef TSTL_HEAP_HPP
#define TSTL_HEAP_HPP

// stl heap algorithms

namespace tstd
{

// default case is max heap, use operator < to compare
// using 1-based index, parent i has children 2*i and 2*i+1

// is_heap_until: get the max subrange that is a heap in range [first, last), return it that [first, it) is a heap
template<typename RandomIterator>
constexpr RandomIterator is_heap_until(RandomIterator first, RandomIterator last) // 1
{
    if (last - first <= 1)
    {
        return last;
    }
    auto current = 2;
    auto parent = current / 2;
    while (first + current - 1 < last && !(*(first + parent - 1) < *(first + current - 1)))
    {
        ++current;
        parent = current / 2;
    }
    return first + current - 1;
}
template<typename RandomIterator, typename Compare>
constexpr RandomIterator is_heap_until(RandomIterator first, RandomIterator last, Compare cmp) // 2
{
    if (last - first <= 1)
    {
        return last;
    }
    auto current = 2;
    auto parent = current / 2;
    while (first + current - 1 < last && !cmp(*(first + parent - 1), *(first + current - 1)))
    {
        ++current;
        parent = current / 2;
    }
    return first + current - 1;
}

// is_heap: check that if [first, last) is a heap
template<typename RandomIterator>
constexpr bool is_heap(RandomIterator first, RandomIterator last) // 1
{
    return tstd::is_heap_until(first, last) == last;
}
template<typename RandomIterator, typename Compare>
constexpr bool is_heap(RandomIterator first, RandomIterator last, Compare cmp) // 2
{
    return tstd::is_heap_until(first, last, cmp) == last;
}

// push_heap: insert the element at last-1 to the heap of [first, last-1)
template<typename RandomIterator>
constexpr void push_heap(RandomIterator first, RandomIterator last) // 1
{
    auto value = *(last - 1);
    auto holeIndex = last - 1 - first + 1; // 1-based index of last-1 (index of first is 1)
    auto parent = holeIndex / 2;
    while (parent >= 1 && *(first + parent - 1) < value)
    {
        *(first + holeIndex - 1) = *(first + parent - 1);
        holeIndex = parent;
        parent = holeIndex / 2;
    }
    *(first + holeIndex - 1) = value;
}
template<typename RandomIterator, typename Compare>
constexpr void push_heap(RandomIterator first, RandomIterator last, Compare cmp) // 2
{
    auto value = *(last - 1);
    auto holeIndex = last - 1 - first + 1; // 1-based index of last-1 (index of first is 1)
    auto parent = holeIndex / 2;
    while (parent >= 1 && cmp(*(first + parent - 1), value))
    {
        *(first + holeIndex - 1) = *(first + parent - 1);
        holeIndex = parent;
        parent = holeIndex / 2;
    }
    *(first + holeIndex - 1) = value;
}

// pop_heap: pop first element of heap [first, last), swap first and last-1, make subrange [first, last-1) become the new heap
template<typename RandomIterator>
constexpr void pop_heap(RandomIterator first, RandomIterator last) // 1
{
    if (first >= last - 1)
    {
        return;
    }
    auto value = *(last - 1);
    *(last - 1) = *first;
    auto holeIndex = 1;
    auto left = 2 * holeIndex;
    auto right = left + 1;
    while (right < (last - 1) - first + 1)
    {
        if (*(first + left - 1) < *(first + right - 1)) // left < right
        {
            *(first + holeIndex - 1) = *(first + right - 1);
            holeIndex = right;
        }
        else
        {
            *(first + holeIndex - 1) = *(first + left - 1);
            holeIndex = left;
        }
        left = 2 * holeIndex;
        right = left + 1;
    }
    // if there is only left child
    if (left < (last - 1) - first + 1)
    {
        *(first + holeIndex - 1) = *(first + left - 1);
        holeIndex = left;
    }
    // give the original last-1 value to the hole, the order of the holeIndex may not be satisfied
    *(first + holeIndex - 1) = value;
    tstd::push_heap(first, first + holeIndex);
}
template<typename RandomIterator, typename Compare>
constexpr void pop_heap(RandomIterator first, RandomIterator last, Compare cmp) // 2
{
    if (last - first <= 1)
    {
        return;
    }
    auto value = *(last - 1);
    *(last - 1) = *first;
    auto holeIndex = 1;
    auto left = 2 * holeIndex;
    auto right = left + 1;
    while (right < (last - 1) - first + 1)
    {
        if (cmp(*(first + left - 1), *(first + right - 1)))
        {
            *(first + holeIndex - 1) = *(first + right - 1);
            holeIndex = right;
        }
        else
        {
            *(first + holeIndex - 1) = *(first + left - 1);
            holeIndex = left;
        }
        left = 2 * holeIndex;
        right = left + 1;
    }
    // if there is only left child
    if (left < (last - 1) - first + 1)
    {
        *(first + holeIndex - 1) = *(first + left - 1);
        holeIndex = left;
    }
    // give the original last-1 value to the hole, the order of the holeIndex may not be satisfied
    *(first + holeIndex - 1) = value;
    tstd::push_heap(first, first + holeIndex, cmp);
}

// make_heap: make a heap in range [first, last)
template<typename RandomIterator>
constexpr void make_heap(RandomIterator first, RandomIterator last) // 1
{
    if (last - first <= 1)
    {
        return;
    }
    for (auto iter = first + 2; iter <= last; ++iter)
    {
        tstd::push_heap(first, iter);
    }
}
template<typename RandomIterator, typename Compare>
constexpr void make_heap(RandomIterator first, RandomIterator last, Compare cmp) // 2
{
    if (last - first <= 1)
    {
        return;
    }
    for (auto iter = first + 2; iter <= last; ++iter)
    {
        tstd::push_heap(first, iter, cmp);
    }
}

// sort_heap: make the heap [first, last) to a sorted range, default case uses operator <, become an asending sequence
template<typename RandomIterator>
constexpr void sort_heap(RandomIterator first, RandomIterator last) // 1
{
    while (last - first > 1)
    {
        tstd::pop_heap(first, last--);
    }
}
template<typename RandomIterator, typename Compare>
constexpr void sort_heap(RandomIterator first, RandomIterator last, Compare cmp) // 2
{
    while (last - first > 1)
    {
        tstd::pop_heap(first, last--, cmp);
    }
}

} // namespace tstd


#endif // TSTL_HEAP_HPP