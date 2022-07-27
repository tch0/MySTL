#ifndef TSTL_UNINTIALIZED_HPP
#define TSTL_UNINTIALIZED_HPP

#include <tstl_allocator.hpp>

namespace tstd
{

// There is space to optimize for POD types.

template<class InputIterator, class ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest)
{
    for (; first != last; ++dest, ++first)
    {
        _construct(&*dest, *first);
    }
    return dest;
}

template<class InputIterator, class Size, class ForwardIterator>
ForwardIterator uninitialized_copy_n(InputIterator first, Size count, ForwardIterator dest)
{
    for (; count > 0; --count, ++dest, ++first)
    {
        _construct(&*dest, *first);
    }
    return dest;
}

template<class ForwardIterator, class T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value)
{
    for (; first != last; ++first)
    {
        _construct(&*first, value);
    }
}

template<class ForwardIterator, class Size, class T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size count, const T& value)
{
    for (; count > 0; --count, ++first)
    {
        _construct(&*first, value);
    }
    return first;
}

} // namespace tstd


#endif