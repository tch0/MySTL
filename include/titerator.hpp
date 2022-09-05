#ifndef IITERATOR_HPP
#define IITERATOR_HPP

#include <cstddef>
#include <iterator>

namespace tstd
{

// deprecated in C++17 : this makes things complicated, should use typename xxx:: represent those types in derived classes
// usually there is no need to use tstd::iterator
template<
    typename Category,
    typename T,
    typename Distance = std::ptrdiff_t,
    typename Pointer = T*,
    typename Reference = T&>
class iterator
{
public:
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};

// iterator adaptors

// 1. reverse iterators
template<typename Iter>
class reverse_iterator
{
public:
    using iterator_type         = Iter;
    using iterator_category     = std::iterator_traits<Iter>::iterator_category;
    using value_type            = std::iterator_traits<Iter>::value_type;
    using difference_type       = std::iterator_traits<Iter>::difference_type;
    using pointer               = std::iterator_traits<Iter>::pointer;
    using reference             = std::iterator_traits<Iter>::reference;

    // constructors
    constexpr reverse_iterator() // 1
        : current() {}
    constexpr explicit reverse_iterator(iterator_type it) // 2
        : current(it) {}
    template<typename U>
    constexpr reverse_iterator(const reverse_iterator<U>& other) // 3
        : current(other.current) {}
    // operator=
    template<typename U>
    constexpr reverse_iterator& operator=(const reverse_iterator<U>& other)
    {
        current = other.current;
        return *this;
    }

    constexpr iterator_type base() const
    {
        return current;
    }
    constexpr reference operator*() const
    {
        iterator_type tmp = current;
        return *--tmp;
    }
    constexpr pointer operator->() const
    {
        iterator_type tmp = current;
        return &(*--tmp);
    }
    constexpr reference operator[](difference_type n) const
    {
        return base()[-n-1];
    }

    // operators of iterator
    constexpr reverse_iterator& operator++() // pre++
    {
        --current;
        return *this;
    }
    constexpr reverse_iterator& operator--() // pre--
    {
        ++current;
        return *this;
    }
    constexpr reverse_iterator operator++(int) // post++
    {
        return reverse_iterator(current--);
    }
    constexpr reverse_iterator operator--(int) // post--
    {
        return reverse_iterator(current++);
    }
    constexpr reverse_iterator operator+(difference_type n) const
    {
        return reverse_iterator(current - n);
    }
    constexpr reverse_iterator operator-(difference_type n) const
    {
        return reverse_iterator(current + n);
    }
    constexpr reverse_iterator& operator+=(difference_type n)
    {
        current -= n;
        return *this;
    }
    constexpr reverse_iterator& operator-=(difference_type n)
    {
        current += n;
        return *this;
    }
protected:
    Iter current;
};

// non-member operations of reverse_iterator
template<typename Iter>
constexpr tstd::reverse_iterator<Iter> make_reverse_iterator(Iter it)
{
    return tstd::reverse_iterator<Iter>(it);
}
// comparisons of reverse_iterator
template<typename Iterator1, typename Iterator2>
bool operator==(const tstd::reverse_iterator<Iterator1>& lhs, const tstd::reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() == rhs.base();
}
template<typename Iterator1, typename Iterator2>
bool operator!=(const tstd::reverse_iterator<Iterator1>& lhs, const tstd::reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() != rhs.base();
}
template<typename Iterator1, typename Iterator2>
bool operator<(const tstd::reverse_iterator<Iterator1>& lhs, const tstd::reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() > rhs.base();
}
template<typename Iterator1, typename Iterator2>
bool operator<=(const tstd::reverse_iterator<Iterator1>& lhs, const tstd::reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() >= rhs.base();
}
template<typename Iterator1, typename Iterator2>
bool operator>(const tstd::reverse_iterator<Iterator1>& lhs, const tstd::reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() < rhs.base();
}
template<typename Iterator1, typename Iterator2>
bool operator>=(const tstd::reverse_iterator<Iterator1>& lhs, const tstd::reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() <= rhs.base();
}

// non-member arithemetic operations
template<typename Iter>
constexpr reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& it)
{
    return reverse_iterator<Iter>(it.base() - n);
}
template<class Iterator1, class Iterator2>
constexpr auto operator-( const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) -> decltype(rhs.base() - lhs.base())
{
    return rhs.base() - lhs.base();
}

// 2. move iterator
template<typename Iter>
class move_iterator
{
public:
    using iterator_type         = Iter;
    using iterator_category     = std::iterator_traits<Iter>::iterator_category;
    using value_type            = std::iterator_traits<Iter>::value_type;
    using difference_type       = std::iterator_traits<Iter>::difference_type;
    using pointer               = std::iterator_traits<Iter>::pointer;
    using reference             = std::iter_rvalue_reference_t<Iter>; // will be a rvalue reference (if std::iterator_traits<Iter>::reference is a reference instead of value)

    // constructors
    constexpr move_iterator() // 1
        : current() {}
    constexpr explicit move_iterator(iterator_type it) // 2
        : current(it) {}
    template<typename U>
    constexpr move_iterator(const move_iterator<U>& other) // 3
        : current(other.current) {}
    
    template<typename U>
    constexpr move_iterator& operator=(const move_iterator<U>& other)
    {
        current = other.current;
    }

    constexpr iterator_type base() const
    {
        return current;
    }
    constexpr reference operator*() const
    {
        return static_cast<reference>(*current);
    }
    constexpr pointer operator->() const
    {
        return &*current;
    }
    constexpr reference operator[](difference_type n) const
    {
        return std::move(current[n]);
    }

    // operators
    constexpr move_iterator& operator++() // pre++
    {
        ++current;
        return *this;
    }
    constexpr move_iterator& operator--() // pre--
    {
        --current;
        return *this;
    }
    constexpr move_iterator operator++(int) // post++
    {
        return move_iterator(current++);
    }
    constexpr move_iterator operator--(int) // post--
    {
        return move_iterator(current--);
    }
    constexpr move_iterator operator+(difference_type n) const
    {
        return move_iterator(current + n);
    }
    constexpr move_iterator operator-(difference_type n) const
    {
        return move_iterator(current - n);
    }
    constexpr move_iterator& operator+=(difference_type n)
    {
        current += n;
        return *this;
    }
    constexpr move_iterator& operator-=(difference_type n)
    {
        current -= n;
        return *this;
    }

private:
    Iter current;
};

// non-member operations of move_iterator
template<typename Iter>
constexpr tstd::move_iterator<Iter> make_move_iterator(Iter it)
{
    return tstd::move_iterator<Iter>(it);
}
// comparisons of move_iterator
template<typename Iterator1, typename Iterator2>
bool operator==(const tstd::move_iterator<Iterator1>& lhs, const tstd::move_iterator<Iterator2>& rhs)
{
    return lhs.base() == rhs.base();
}
template<typename Iterator1, typename Iterator2>
bool operator!=(const tstd::move_iterator<Iterator1>& lhs, const tstd::move_iterator<Iterator2>& rhs)
{
    return lhs.base() != rhs.base();
}
template<typename Iterator1, typename Iterator2>
bool operator<(const tstd::move_iterator<Iterator1>& lhs, const tstd::move_iterator<Iterator2>& rhs)
{
    return lhs.base() < rhs.base();
}
template<typename Iterator1, typename Iterator2>
bool operator<=(const tstd::move_iterator<Iterator1>& lhs, const tstd::move_iterator<Iterator2>& rhs)
{
    return lhs.base() <= rhs.base();
}
template<typename Iterator1, typename Iterator2>
bool operator>(const tstd::move_iterator<Iterator1>& lhs, const tstd::move_iterator<Iterator2>& rhs)
{
    return lhs.base() > rhs.base();
}
template<typename Iterator1, typename Iterator2>
bool operator>=(const tstd::move_iterator<Iterator1>& lhs, const tstd::move_iterator<Iterator2>& rhs)
{
    return lhs.base() >= rhs.base();
}

// non-member arithemetic operations
template<typename Iter>
constexpr move_iterator<Iter> operator+(typename move_iterator<Iter>::difference_type n, const move_iterator<Iter>& it)
{
    return move_iterator<Iter>(it.base() + n);
}
template<class Iterator1, class Iterator2>
constexpr auto operator-( const move_iterator<Iterator1>& lhs, const move_iterator<Iterator2>& rhs) -> decltype(lhs.base() - rhs.base())
{
    return lhs.base() - rhs.base();
}

// front insert iterator
template<typename Container>
class front_insert_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_tyep = std::ptrdiff_t;
    using pointer = void;
    using reference = void;
    using container_type = Container;
private:
    Container* container;
public:
    explicit constexpr front_insert_iterator(Container& c)
        : container(&c)
    {
    }
    constexpr front_insert_iterator& operator=(typename Container::value_type& value)
    {
        container->push_front(value);
        return *this;
    }
    constexpr front_insert_iterator& operator=(typename Container::value_type&& value)
    {
        container->push_front(std::move(value));
        return *this;
    }
    constexpr front_insert_iterator& operator*() { return *this; }
    constexpr front_insert_iterator& operator++() { return *this; }
    constexpr front_insert_iterator& operator++(int) { return *this; }
};

template<typename Container>
constexpr tstd::front_insert_iterator<Container> front_inserter(Container& c)
{
    return tstd::front_insert_iterator<Container>(c);
}

// back insert iterator
template<typename Container>
class back_insert_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_tyep = std::ptrdiff_t;
    using pointer = void;
    using reference = void;
    using container_type = Container;
private:
    Container* container;
public:
    explicit constexpr back_insert_iterator(Container& c)
        : container(&c)
    {
    }
    constexpr back_insert_iterator& operator=(typename Container::value_type& value)
    {
        container->push_back(value);
        return *this;
    }
    constexpr back_insert_iterator& operator=(typename Container::value_type&& value)
    {
        container->push_back(std::move(value));
        return *this;
    }
    constexpr back_insert_iterator& operator*() { return *this; }
    constexpr back_insert_iterator& operator++() { return *this; }
    constexpr back_insert_iterator& operator++(int) { return *this; }
};

template<typename Container>
constexpr tstd::back_insert_iterator<Container> back_inserter(Container& c)
{
    return tstd::back_insert_iterator<Container>(c);
}

// insert iterator
template<typename Container>
class insert_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_tyep = std::ptrdiff_t;
    using pointer = void;
    using reference = void;
    using container_type = Container;
private:
    Container* container;
    typename Container::iterator iter;
public:
    constexpr insert_iterator(Container& c, typename Container::iterator i)
        : container(&c)
        , iter(i)
    {
    }
    constexpr insert_iterator& operator=(typename Container::value_type& value)
    {
        iter = container->insert(iter, value);
        ++iter;
        return *this;
    }
    constexpr insert_iterator& operator=(typename Container::value_type&& value)
    {
        iter = container->insert(iter, std::move(value));
        ++iter;
        return *this;
    }
    constexpr insert_iterator& operator*() { return *this; }
    constexpr insert_iterator& operator++() { return *this; }
    constexpr insert_iterator& operator++(int) { return *this; }
};

template<class Container>
constexpr tstd::insert_iterator<Container> inserter(Container& c, typename Container::iterator i)
{
    return tstd::insert_iterator<Container>(c, i);
}

template<typename InputIterator, typename Distance>
constexpr void advance(InputIterator& it, Distance n)
{
    using category = typename std::iterator_traits<InputIterator>::iterator_category;
    static_assert(std::is_base_of_v<std::input_iterator_tag, category>);

    auto dist = typename std::iterator_traits<InputIterator>::difference_type(n);
    if constexpr (std::is_base_of_v<std::random_access_iterator_tag, category>)
    {
        it += n;
    }
    else
    {
        while (dist > 0)
        {
            --dist;
            ++it;
        }
        if constexpr (std::is_base_of_v<std::bidirectional_iterator_tag, category>)
        {
            while (dist < 0)
            {
                ++dist;
                --it;
            }
        }
    }
}

template<typename InputIterator>
typename std::iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
{
    using category = typename std::iterator_traits<InputIterator>::iterator_category;
    static_assert(std::is_base_of_v<std::input_iterator_tag, category>);

    if constexpr (std::is_base_of_v<std::random_access_iterator_tag, category>)
    {
        return last - first;
    }
    else
    {
        typename std::iterator_traits<InputIterator>::difference_type result = 0;
        while (first != last)
        {
            ++first;
            ++result;
        }
        return result;
    }
}

template<typename InputIterator>
constexpr InputIterator next(InputIterator it, typename std::iterator_traits<InputIterator>::difference_type n = 1)
{
    tstd::advance(it, n);
    return it;
}

template<typename BidirectionalIterator>
constexpr BidirectionalIterator prev(BidirectionalIterator it, typename std::iterator_traits<BidirectionalIterator>::difference_type n = 1)
{
    static_assert(std::is_base_of_v<std::bidirectional_iterator_tag, typename std::iterator_traits<BidirectionalIterator>::iterator_category>);
    tstd::advance(it, -n);
    return it;
}

// range access
// begin, cbegin
template<typename C>
constexpr auto begin(C& c) -> decltype(c.begin())
{
    return c.begin();
}
template<typename C>
constexpr auto begin(const C& c) -> decltype(c.begin())
{
    return c.begin();
}
template<typename T, std::size_t N> // specialization for array
constexpr T* begin(T (&arr)[N]) noexcept
{
    return arr;
}
template<typename C>
constexpr auto cbegin(const C& c) noexcept(noexcept(tstd::begin(c))) -> decltype(tstd::begin(c))
{
    return tstd::begin(c);
}
// end, cend
template<typename C>
constexpr auto end(C& c) -> decltype(c.end())
{
    return c.end();
}
template<typename C>
constexpr auto end(const C& c) -> decltype(c.end())
{
    return c.end();
}
template<typename T, std::size_t N> // specialization for array
constexpr T* end(T (&arr)[N]) noexcept
{
    return arr + N;
}
template<typename C>
constexpr auto cend(const C& c) noexcept(noexcept(tstd::end(c))) -> decltype(tstd::end(c))
{
    return tstd::end(c);
}
// rbegin, crbegin
template<typename C>
constexpr auto rbegin(C& c) -> decltype(c.rbegin())
{
    return c.rbegin();
}
template<typename C>
constexpr auto rbegin(const C& c) -> decltype(c.rbegin())
{
    return c.rbegin();
}
template<typename T, std::size_t N> // specialization for array
constexpr tstd::reverse_iterator<T*> rbegin(T (&arr)[N]) noexcept
{
    return tstd::reverse_iterator<T*>(arr + N);
}
template<typename T>
constexpr tstd::reverse_iterator<const T*> rbegin(std::initializer_list<T> il)
{
    return tstd::reverse_iterator<const T*>(il.end());
}
template<typename C>
constexpr auto crbegin(const C& c) noexcept(noexcept(tstd::rbegin(c))) -> decltype(tstd::rbegin(c))
{
    return tstd::rbegin(c);
}
// rend, crend
template<typename C>
constexpr auto rend(C& c) -> decltype(c.rend())
{
    return c.rend();
}
template<typename C>
constexpr auto rend(const C& c) -> decltype(c.rend())
{
    return c.rend();
}
template<typename T, std::size_t N> // specialization for array
constexpr tstd::reverse_iterator<T*> rend(T (&arr)[N]) noexcept
{
    return tstd::reverse_iterator<T*>(arr);
}
template<typename T>
constexpr tstd::reverse_iterator<const T*> rend(std::initializer_list<T> il)
{
    return tstd::reverse_iterator<const T*>(il.begin());
}
template<typename C>
constexpr auto crend(const C& c) noexcept(noexcept(tstd::rend(c))) -> decltype(tstd::rend(c))
{
    return tstd::rend(c);
}
// size
template<typename C>
constexpr auto size(const C& c) -> decltype(c.size())
{
    return c.size();
}
template<typename T, std::size_t N>
constexpr std::size_t size(const T (&arr)[N]) noexcept
{
    return N;
}
// empty
template<typename C>
[[nodiscard]] constexpr auto empty(const C& c) -> decltype(c.empty())
{
    return c.empty();
}
template<typename T, std::size_t N>
[[nodiscard]] constexpr bool empty(const T (&arr)[N]) noexcept
{
    return false;
}
template<typename E>
[[nodiscard]] constexpr bool empty(std::initializer_list<E> il) noexcept
{
    return il.size() == 0;
}
// data
template<typename C>
constexpr auto data(C& c) -> decltype(c.data())
{
    return c.data();
}
template<typename C>
constexpr auto data(const C& c) -> decltype(c.data())
{
    return c.data();
}
template<typename T, std::size_t N>
constexpr T* data(T (&arr)[N]) noexcept
{
    return arr;
}
template<typename E>
constexpr const E* data(std::initializer_list<E> il) noexcept
{
    return il.begin();
}

} // namespace tstd


#endif // IITERATOR_HPP