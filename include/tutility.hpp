#ifndef TUTILITY_HPP
#define TUTILITY_HPP

#include <type_traits>
#include <cstddef>

namespace tstd
{

// swap
template<typename T>
constexpr void swap(T& a, T& b)
    noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>)
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

template<typename T, std::size_t N>
constexpr void swap(T (&a)[N], T (&b)[N]) noexcept(std::is_nothrow_swappable_v<T>)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        swap(a[i], b[i]);
    }
}

} // namespace tstd


#endif