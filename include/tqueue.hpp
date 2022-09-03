#ifndef TQUEUE_HPP
#define TQUEUE_HPP

#include <tdeque.hpp>
#include <initializer_list>
#include <tstl_heap.hpp>
#include <tvector.hpp>
#include <functional> // todo: replace with <tfunctional.hpp>
#include <type_traits>

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
        assert(!c.empty());
        return c.front();
    }
    const_reference front() const
    {
        assert(!c.empty());
        return c.front();
    }
    reference back()
    {
        assert(!c.empty());
        return c.back();
    }
    const_reference back() const
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
// comparisons
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


// priority_queue

template<typename T, typename Container = tstd::vector<T>, typename Compare = std::less<typename Container::value_type>>
class priority_queue
{
public:
    using container_type = Container;
    using value_compare = Compare;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
public:
    
    priority_queue() // 1
        : priority_queue(Compare(), Container())
    {
    }
    explicit priority_queue(const Compare& compare) // 2
        : priority_queue(compare, Container())
    {
    }
    priority_queue(const Compare& compare, const Container& cont) // 3
        : c(cont)
        , comp(compare)
    {
        tstd::make_heap(c.begin(), c.end(), comp);
    }
    priority_queue(const Compare& compare, Container&& cont) // 4
        : c(std::move(cont))
        , comp(compare)
    {
        tstd::make_heap(c.begin(), c.end(), comp);
    }
    priority_queue(const priority_queue& other) // 5
        : c(other.c)
        , comp(other.comp)
    {
    }
    priority_queue(priority_queue&& other) // 6
        : c(std::move(other.c))
        , comp(other.comp)
    {
    }
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    priority_queue(InputIterator first, InputIterator last, const Compare& compare = Compare()) // 7
        : c(first, last)
        , comp(compare)
    {
        tstd::make_heap(c.begin(), c.end(), comp);
    }
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    priority_queue(InputIterator first, InputIterator last, const Compare& compare, const Container& cont) // 8
        : c(cont)
        , comp(compare)
    {
        c.insert(c.end(), first, last);
        tstd::make_heap(c.begin(), c.end(), comp);
    }
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    priority_queue(InputIterator first, InputIterator last, const Compare& compare, Container&& cont) // 9
        : c(std::move(cont))
        , comp(compare)
    {
        c.insert(c.end(), first, last);
        tstd::make_heap(c.begin(), c.end(), comp);
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    explicit priority_queue(const Allocator& alloc) // 10
        : c(alloc)
        , comp()
    {
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    priority_queue(const Compare& compare, const Allocator& alloc) // 11
        : c(alloc)
        , comp(compare)
    {
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    priority_queue(const Compare& compare, const Container& cont, const Allocator& alloc) // 12
        : c(cont, alloc)
        , comp(compare)
    {
        tstd::make_heap(c.begin(), c.end(), comp);
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    priority_queue(const Compare& compare, Container&& cont, const Allocator& alloc) // 13
        : c(std::move(cont), alloc)
        , comp(compare)
    {
        tstd::make_heap(c.begin(), c.end(), comp);
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    priority_queue(const priority_queue& other, const Allocator& alloc) // 14
        : c(other.c, alloc)
        , comp(other.comp)
    {
    }
    template<typename Allocator,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    priority_queue(priority_queue&& other, const Allocator& alloc) // 15
        : c(std::move(other.c), alloc)
        , comp(other.comp)
    {
    }
    template<typename InputIterator, typename Allocator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    priority_queue(InputIterator first, InputIterator last, const Allocator& alloc) // 16
        : c(first, last, alloc)
        , comp()
    {
        tstd::make_heap(c.begin(), c.end(), comp);
    }
    template<typename InputIterator, typename Allocator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    priority_queue(InputIterator first, InputIterator last, const Compare& compare, const Allocator& alloc) // 17
        : c(first, last, alloc)
        , comp(compare)
    {
        tstd::make_heap(c.begin(), c.end(), comp);
    }
    template<typename InputIterator, typename Allocator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    priority_queue(InputIterator first, InputIterator last, const Compare& compare, const Container& cont, const Allocator& alloc) // 18
        : c(cont, alloc)
        , comp(compare)
    {
        c.insert(c.end(), first, last);
        tstd::make_heap(c.begin(), c.end(), comp);
    }
    template<typename InputIterator, typename Allocator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>,
        typename = std::enable_if_t<std::uses_allocator_v<Container, Allocator>>>
    priority_queue(InputIterator first, InputIterator last, const Compare& compare, Container&& cont, const Allocator& alloc) // 18
        : c(std::move(cont))
        , comp(compare)
    {
        c.insert(c.end(), first, last);
        tstd::make_heap(c.begin(), c.end(), comp);
    }
    ~priority_queue()
    {
    }
    // assignment
    priority_queue& operator=(const priority_queue& other)
    {
        c = other.c;
        comp = other.comp;
        return *this;
    }
    priority_queue& operator=(priority_queue&& other)
    {
        c = std::move(other.c);
        comp = std::move(other.comp);
        return *this;
    }
    // element access
    reference top()
    {
        assert(!c.empty());
        return c.front();
    }
    const_reference top() const
    {
        assert(!c.empty());
        return c.front();
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
        tstd::push_heap(c.begin(), c.end(), comp);
    }
    void push(value_type&& value)
    {
        c.push_back(std::move(value));
        tstd::push_heap(c.begin(), c.end(), comp);
    }
    template<typename... Args>
    void emplace(Args&&... args)
    {
        c.emplace_back(std::forward<Args>(args)...);
        tstd::push_heap(c.begin(), c.end(), comp);
    }
    void pop()
    {
        assert(!c.empty());
        tstd::pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }
    void swap(priority_queue& other) noexcept(std::is_nothrow_swappable_v<Container> && std::is_nothrow_swappable_v<Compare>)
    {
        using tstd::swap;
        swap(c, other.c);
        swap(comp, other.comp);
    }
protected:
    Container c;
    Compare comp;
};

// global swap for tstd::priority_queue
template<typename T, typename Container, typename Compare>
constexpr void swap(tstd::priority_queue<T, Container, Compare>& lhs, tstd::priority_queue<T, Container, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace tstd


#endif // TQUEUE_HPP