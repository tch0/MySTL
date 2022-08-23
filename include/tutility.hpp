#ifndef TUTILITY_HPP
#define TUTILITY_HPP

#include <type_traits>
#include <cstddef>
#include <utility>

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

namespace impl
{

// types for set/map/multimap/unorder_set/unordered_map/unordered_multiset/unordered_multimap
// for set/multiset/...
template<typename T>
class identity
{
public:
    T& operator()(T& value) const
    {
        return value;
    }
};

// for map/multimap/...
template<typename T1, typename T2>
class first_of_pair
{
public:
    T1& operator()(std::pair<T1, T2>& p) const
    {
        return p.first;
    }
    const T1& operator()(const std::pair<T1, T2>& p) const
    {
        return p.first;
    }
};

} // namespace impl

} // namespace tstd

#endif // TUTILITY_HPP