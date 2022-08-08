#ifndef TQUEUE_HPP
#define TQUEUE_HPP

#include <tdeque.hpp>
#include <initializer_list>

namespace tstd
{

template<typename T, typename Container = tstd::deque<T>>
class queue;

template<typename T, typename Container>
constexpr bool operator==(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);
template<typename T, typename Container>
constexpr bool operator!=(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);
template<typename T, typename Container>
constexpr bool operator<(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);
template<typename T, typename Container>
constexpr bool operator<=(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);
template<typename T, typename Container>
constexpr bool operator>(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);
template<typename T, typename Container>
constexpr bool operator>=(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);

template<typename T, typename Container>
class queue
{
    friend constexpr bool operator== <T, Container>(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);
    friend constexpr bool operator!= <T, Container>(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);
    friend constexpr bool operator<  <T, Container>(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);
    friend constexpr bool operator<= <T, Container>(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);
    friend constexpr bool operator>  <T, Container>(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);
    friend constexpr bool operator>= <T, Container>(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs);
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
public:
    queue() : queue(Container()) {} // 1
    explicit queue(const Container& cont) // 2
        : c(cont)
    {
    }
    explicit queue(Container&& cont) // 3
        : c(std::move(cont))
    {
    }
    queue(const queue& other) // 4
        : c(other.c)
    {
    }
    queue(queue&& other) // 5
        : c(std::move(other.c))
    {
    }
    // since C++23
    // template<typename InputIterator,
    //     typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    // queue(InputIterator first, InputIterator last) // 6
    //     : c(first, last)
    // {
    // }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    explicit queue(const Allocator& alloc) // 7
        : c(alloc)
    {
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    queue(const Container& cont, const Allocator& alloc) // 8
        : c(cont, alloc)
    {
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    queue(Container&& cont, const Allocator& alloc) // 9
        : c(std::move(cont), alloc)
    {
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    queue(const queue& other, const Allocator& alloc) // 10
        : c(other.c, alloc)
    {
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    queue(queue&& other, const Allocator& alloc) // 11
        : c(std::move(other.c), alloc)
    {
    }
    // since C++23
    // template<typename InputIterator, typename Allocator,
    //     typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>,
    //     typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    // queue(InputIterator first, InputIterator last, const Allocator& alloc) // 12
    //     : c(first, last, alloc)
    // {
    // }
    ~queue()
    {
    }
    // assignment
    queue& operator=(const queue& other)
    {
        c = other.c;
        return *this;
    }
    queue& operator=(queue&& other)
    {
        c = std::move(other.c);
        return *this;
    }
    // element access
    reference front()
    {
        return c.front();
    }
    const_reference front() const
    {
        return c.front();
    }
    reference back()
    {
        return c.back();
    }
    const_reference back() const
    {
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
        c.pop_front();
    }
    void swap(queue& other) noexcept(std::is_nothrow_swappable_v<Container>)
    {
        using tstd::swap;
        swap(c, other.c);
    }
protected:
    Container c;
};

// non-member operations
// comparasions
template<typename T, typename Container>
constexpr bool operator==(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs)
{
    return lhs.c == rhs.c;
}
template<typename T, typename Container>
constexpr bool operator!=(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs)
{
    return lhs.c != rhs.c;
}
template<typename T, typename Container>
constexpr bool operator<(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs)
{
    return lhs.c < rhs.c;
}
template<typename T, typename Container>
constexpr bool operator<=(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs)
{
    return lhs.c <= rhs.c;
}
template<typename T, typename Container>
constexpr bool operator>(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs)
{
    return lhs.c > rhs.c;
}
template<typename T, typename Container>
constexpr bool operator>=(const tstd::queue<T, Container>& lhs, const tstd::queue<T, Container>& rhs)
{
    return lhs.c >= rhs.c;
}

// global swap for tstd::queue
template<typename T, typename Container>
constexpr void swap(tstd::queue<T, Container>& lhs, tstd::queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace tstd


#endif // TQUEUE_HPP