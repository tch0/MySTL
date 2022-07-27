#ifndef TSTL_ALLOCATOR_HPP
#define TSTL_ALLOCATOR_HPP

#include <new>     // for placement new
#include <cstddef> // for ptrdiff_t size_t
#include <limits>  // for numeric_limits

namespace tstd
{

// auxiliary global functions for allocator
template<typename T>
inline T* _allocate(ptrdiff_t size, [[maybe_unused]] T*)
{
    std::set_new_handler(nullptr);
    T* tmp = static_cast<T*>(::operator new(static_cast<size_t>(size * sizeof(T))));
    if (tmp == 0)
    {
        throw std::bad_alloc();
    }
    return tmp;
}

template<typename T>
inline void _deallocate(T* buffer)
{
    ::operator delete(buffer);
}

template<typename T1, typename... Args>
inline void _construct(T1* p, Args&&... args)
{
    new (p) T1(std::forward<Args>(args)...); // invoke constructor of T1
}

template<typename T>
inline void _destroy(T* ptr)
{
    ptr->~T();
}

// allocator
template<typename T>
class allocator
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    // deprecated in C++17, removed in C++20
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    template< class U > struct rebind {
        using other = allocator<U>;
    };
    // deprecated in C++17, removed in C++23
    using is_always_equal = std::true_type;

    // default constructor
    constexpr allocator() noexcept = default;

    // copy constructor
    constexpr allocator(const allocator& other) noexcept = default;
    template<typename U>
    constexpr allocator(const allocator<U>& other) noexcept {}

    // deprecated in C++17, removed in C++20
    T* allocate(size_type n, const void* hint)
    {
        _allocate(static_cast<difference_type>(n), hint);
    }

    [[nodiscard]] constexpr T* allocate(size_type n)
    {
        return _allocate(static_cast<difference_type>(n), (T*)nullptr);
    }

    constexpr void deallocate(T* p, [[maybe_unused]] size_type n)
    {
        _deallocate(p);
    }

    // deprecated in C++17, removed in C++20
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args)
    {
        _construct(p, std::forward<Args>(args)...);
    }

    // deprecated in C++17, removed in C++20
    size_type max_size() const noexcept
    {
        return static_cast<size_type>(std::numeric_limits<size_t>::max() / sizeof(T));
    }

    // deprecated in C++17, removed in C++20
    template<typename U>
    void destroy(U* p)
    {
        _destroy(p);
    }

    // deprecated in C++17, removed in C++20
    pointer address(reference x) const noexcept
    {
        return static_cast<pointer>(&x);
    }
    const_pointer address(const_reference x) const noexcept
    {
        return static_cast<const_pointer>(&x);
    }
};

// comparsion of allocators
template<typename T1, typename T2>
constexpr bool operator==( const allocator<T1>& lhs, const allocator<T2>& rhs ) noexcept
{
    return true;
}
template<typename T1, typename T2>
bool operator!=( const allocator<T1>& lhs, const allocator<T2>& rhs ) noexcept
{
    return false;
}

} // namespace tstd

#endif