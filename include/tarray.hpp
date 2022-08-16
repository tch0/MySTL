#ifndef TARRAY_HPP
#define TARRAY_HPP

#include <cstddef>
#include <stdexcept>
#include <limits>
#include <tstl_uninitialized.hpp>
#include <type_traits>
#include <initializer_list>

namespace tstd
{

// array is an aggregate class
template<typename T, std::size_t N>
class array
{
public:
    using value_type = T;
    using size_type= std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = tstd::reverse_iterator<iterator>;
    using const_reverse_iterator = tstd::reverse_iterator<const_iterator>;
public:
    // constructors are implicit
    // elements access
    constexpr reference at(size_type pos)
    {
        if (pos >= N)
        {
            throw std::out_of_range("array::at index out of range");
        }
        return elements[pos];
    }
    constexpr const_reference at(size_type pos) const
    {
        if (pos >= N)
        {
            throw std::out_of_range("array::at index out of range");
        }
        return elements[pos];
    }
    constexpr reference operator[](size_type pos)
    {
        return elements[pos];
    }
    constexpr const_reference operator[](size_type pos) const
    {
        return elements[pos];
    }
    constexpr reference front()
    {
        return elements[0];
    }
    constexpr const_reference front() const
    {
        return elements[0];
    }
    constexpr reference back()
    {
        return elements[N-1];
    }
    constexpr const_reference back() const
    {
        return elements[N-1];
    }
    // iterators
    constexpr iterator begin() noexcept
    {
        return elements;
    }
    constexpr const_iterator begin() const noexcept
    {
        return elements;
    }
    constexpr const_iterator cbegin() const noexcept
    {
        return elements;
    }
    constexpr iterator end() noexcept
    {
        return elements + N;
    }
    constexpr const_iterator end() const noexcept
    {
        return elements + N;
    }
    constexpr const_iterator cend() const noexcept
    {
        return elements + N;
    }
    constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(elements + N);
    }
    constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(elements + N);
    }
    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(elements + N);
    }
    constexpr reverse_iterator rend() noexcept
    {
        return reverse_iterator(elements);
    }
    constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(elements);
    }
    constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(elements);
    }
    // size and capacity
    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return N == 0;
    }
    constexpr size_type size() const noexcept
    {
        return N;
    }
    constexpr size_type max_size() const noexcept
    {
        return N;
    }
    // modifiers
    constexpr void fill(const T& value)
    {
        for (size_type i = 0; i < N; ++i)
        {
            elements[i] = value;
        }
    }
    constexpr void swap(array& other) noexcept(std::is_nothrow_swappable_v<T>)
    {
        T tmp;
        for (size_type i = 0; i < N; ++i)
        {
            tmp = elements[i];
            elements[i] = other.elements[i];
            other.elements[i] = tmp;
        }
    }
public:
    T elements[N];
};

// non-member functions
// a non-standard compare function for tstd::array
// equal 0 less -1 greater 1
template<typename T, std::size_t N>
constexpr int _cmp_array(const tstd::array<T, N>& lhs, const tstd::array<T, N>& rhs)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        if (lhs[i] == rhs[i])
        {
            continue;
        }
        return lhs[i] > rhs[i] ? 1 : -1;
    }
    return 0;
}

// comparisons
template<typename T, std::size_t N>
constexpr bool operator==(const tstd::array<T, N>& lhs, const tstd::array<T, N>& rhs)
{
    return _cmp_array(lhs, rhs) == 0;
}
template<typename T, std::size_t N>
constexpr bool operator!=(const tstd::array<T, N>& lhs, const tstd::array<T, N>& rhs)
{
    return _cmp_array(lhs, rhs) != 0;
}
template<typename T, std::size_t N>
constexpr bool operator<(const tstd::array<T, N>& lhs, const tstd::array<T, N>& rhs)
{
    return _cmp_array(lhs, rhs) < 0;
}
template<typename T, std::size_t N>
constexpr bool operator<=(const tstd::array<T, N>& lhs, const tstd::array<T, N>& rhs)
{
    return _cmp_array(lhs, rhs) <= 0;
}
template<typename T, std::size_t N>
constexpr bool operator>(const tstd::array<T, N>& lhs, const tstd::array<T, N>& rhs)
{
    return _cmp_array(lhs, rhs) > 0;
}
template<typename T, std::size_t N>
constexpr bool operator>=(const tstd::array<T, N>& lhs, const tstd::array<T, N>& rhs)
{
    return _cmp_array(lhs, rhs) >= 0;
}

// tstd::get
template<std::size_t I, typename T, std::size_t N>
constexpr T& get(tstd::array<T, N>& a) noexcept
{
    return a[I];
}
template<std::size_t I, typename T, std::size_t N>
constexpr T&& get(tstd::array<T, N>&& a) noexcept
{
    return a[I];
}
template<std::size_t I, typename T, std::size_t N>
constexpr const T& get(const tstd::array<T, N>& a) noexcept
{
    return a[I];
}
template<std::size_t I, typename T, std::size_t N>
constexpr const T&& get(const tstd::array<T, N>&& a) noexcept
{
    return a[I];
}

// tstd::swap
template<class T, std::size_t N,
    typename = std::enable_if<N == 0 || std::is_swappable_v<T>>>
constexpr void swap(tstd::array<T, N>& lhs, tstd::array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

// built-in array to tstd::array
template<typename T, std::size_t N, 
    typename = std::enable_if<std::is_constructible_v<T, T&> && !std::is_array_v<T>>>
constexpr tstd::array<std::remove_cv_t<T>, N> to_array(T (&a)[N])
{
    tstd::array<std::remove_cv_t<T>, N> ret;
    for (std::size_t i = 0; i < N; ++i)
    {
        ret[i] = a[i];
    }
    return ret;
}
template<typename T, std::size_t N, 
    typename std::enable_if<std::is_move_constructible_v<T> && !std::is_array_v<T>>>
constexpr tstd::array<std::remove_cv_t<T>, N> to_array(T (&&a)[N])
{
    tstd::array<std::remove_cv_t<T>, N> ret;
    for (std::size_t i = 0; i < N; ++i)
    {
        ret[i] = std::move(a[i]);
    }
    return ret;
}

// auxiliary classes
// todo: specilization of (t?)std::tuple_size and (t?)std::tuple_element

} // namespace tstd


#endif // TARRAY_HPP