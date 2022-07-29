#ifndef TSTL_UNINTIALIZED_HPP
#define TSTL_UNINTIALIZED_HPP

#include <tstl_allocator.hpp>
#include <memory>
#include <type_traits>

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

// since C++17
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_move(InputIterator first, InputIterator last, ForwardIterator dest)
{
    for (; first != last; ++dest, ++first)
    {
        _construct(&*dest, std::move(*first));
    }
}

// since C++17
template<typename InputIterator, typename Size, typename ForwardIterator>
ForwardIterator uninitialized_move_n(InputIterator first, Size count, ForwardIterator dest)
{
    for (; count > 0; --count, ++dest, ++first)
    {
        _construct(&*dest, std::move(*first));
    }
}

// since C++20
template<typename T, typename... Args>
constexpr T* construct_at(T* p, Args&&... args)
{
    return new (p) T(std::forward<Args>(args)...);
}

// since C++17
template<typename T>
constexpr void destroy_at(T* p)
{
    if constexpr (std::is_array_v<T>)
    {
        for (auto& elem: *p)
        {
            destroy_at(std::addressof(elem));
        }
    }
    else
    {
        p->~T();
    }
}

// since C++17
template<typename ForwardIterator>
constexpr void destroy(ForwardIterator first, ForwardIterator last)
{
    for (; first != last; ++first)
    {
        destroy_at(std::addressof(*first));
    }
}

// since C++17
template<typename ForwardIterator, typename Size>
constexpr void destroy(ForwardIterator first, Size count)
{
    for (; count > 0; --count, ++first)
    {
        destroy_at(std::addressof(*first));
    }
}

} // namespace tstd


#endif