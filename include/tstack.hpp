#ifndef TSTACK_HPP
#define TSTACK_HPP

#include <tdeque.hpp>
#include <initializer_list>
#include <cassert>

namespace tstd
{

// need forward template declaration of stack for comparision function template declarations.
// default template arguments should only occur at first time (no matter a definition or a declaration).
template<typename T, typename Container = tstd::deque<T>>
class stack;

// need template declaration for friend declaration
template<typename T, typename Container>
constexpr bool operator==(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);
template<typename T, typename Container>
constexpr bool operator!=(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);
template<typename T, typename Container>
constexpr bool operator<(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);
template<typename T, typename Container>
constexpr bool operator<=(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);
template<typename T, typename Container>
constexpr bool operator>(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);
template<typename T, typename Container>
constexpr bool operator>=(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);

template<typename T, typename Container>
class stack
{
    // template arguments are needed in friend declaration, or else these will become non-template friend functions declaration.
    friend constexpr bool operator== <T, Container>(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);
    friend constexpr bool operator!= <T, Container>(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);
    friend constexpr bool operator<  <T, Container>(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);
    friend constexpr bool operator<= <T, Container>(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);
    friend constexpr bool operator>  <T, Container>(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);
    friend constexpr bool operator>= <T, Container>(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs);
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
public:
    stack() : stack(Container()) {} // 1
    explicit stack(const Container& cont) // 2
        : c(cont)
    {
    }
    explicit stack(Container&& cont) // 3
        : c(std::move(cont))
    {
    }
    stack(const stack& other) // 4
        : c(other.c)
    {
    }
    stack(stack&& other) // 5
        : c(std::move(other.c))
    {
    }
    // since C++23
    // template<typename InputIterator,
    //     typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    // stack(InputIterator first, InputIterator last) // 6
    //     : c(first, last)
    // {
    // }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    explicit stack(const Allocator& alloc) // 7
        : c(alloc)
    {
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    stack(const Container& cont, const Allocator& alloc) // 8
        : c(cont, alloc)
    {
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    stack(Container&& cont, const Allocator& alloc) // 9
        : c(std::move(cont), alloc)
    {
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    stack(const stack& other, const Allocator& alloc) // 10
        : c(other.c, alloc)
    {
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    stack(stack&& other, const Allocator& alloc) // 11
        : c(std::move(other.c), alloc)
    {
    }
    // since C++23
    // template<typename InputIterator, typename Allocator,
    //     typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>,
    //     typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    // stack(InputIterator first, InputIterator last, const Allocator& alloc) // 12
    //     : c(first, last, alloc)
    // {
    // }
    ~stack()
    {
    }
    // assignment
    stack& operator=(const stack& other)
    {
        c = other.c;
        return *this;
    }
    stack& operator=(stack&& other)
    {
        c = std::move(other.c);
        return *this;
    }
    // element access
    reference top()
    {
        assert(!c.empty());
        return c.back();
    }
    const_reference top() const
    {
        assert(!c.empty());
        return c.back();
    }
    // size and capacity
    [[nodiscard]] bool empty() const
    {
        return c.empty();
    }
    size_type size() const
    {
        return c.size();
    }
    // modifiers
    void push(const value_type& value)
    {
        c.push_back(value);
    }
    void push(value_type&& value)
    {
        c.push_back(std::move(value));
    }
    template<typename... Args>
    decltype(auto) emplace(Args&&... args)
    {
        return c.emplace_back(std::forward<Args>(args)...);
    }
    void pop()
    {
        assert(!c.empty());
        c.pop_back();
    }
    void swap(stack& other) noexcept(std::is_nothrow_swappable_v<Container>)
    {
        using tstd::swap;
        swap(c, other.c);
    }
protected:
    Container c;
};

// non-member operations
// comparisons
template<typename T, typename Container>
constexpr bool operator==(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs)
{
    return lhs.c == rhs.c;
}
template<typename T, typename Container>
constexpr bool operator!=(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs)
{
    return lhs.c != rhs.c;
}
template<typename T, typename Container>
constexpr bool operator<(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs)
{
    return lhs.c < rhs.c;
}
template<typename T, typename Container>
constexpr bool operator<=(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs)
{
    return lhs.c <= rhs.c;
}
template<typename T, typename Container>
constexpr bool operator>(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs)
{
    return lhs.c > rhs.c;
}
template<typename T, typename Container>
constexpr bool operator>=(const tstd::stack<T, Container>& lhs, const tstd::stack<T, Container>& rhs)
{
    return lhs.c >= rhs.c;
}

// global swap for tstd::stack
template<typename T, typename Container>
constexpr void swap(tstd::stack<T, Container>& lhs, tstd::stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace tstd


#endif // TSTACK_HPP