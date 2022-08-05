#ifndef TLIST_HPP
#define TLIST_HPP

#include <tstl_allocator.hpp>
#include <tstl_uninitialized.hpp>
#include <titerator.hpp>
#include <tutility.hpp>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <memory>
#include <cassert>

namespace tstd
{
// implement with a cyclic doubly linked list
template<typename T, typename Allocator = tstd::allocator<T>>
class list
{
private:
    // nested classes
    // list node
    struct list_node
    {
        list_node* next;
        list_node* prev;
        T data;
    };
    // iterator
    template<typename T2, typename Ref, typename Ptr>
    struct __list_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T2;
        using pointer = Ptr;
        using reference = Ref;
        using difference_type = std::ptrdiff_t;
        
        using iterator = __list_iterator<T2, T2&, T2*>;
        using self = __list_iterator<T2, Ref, Ptr>;
        using size_type = std::size_t;
        using link_type = list_node*;
        link_type node; // pointer to node of linked list

        __list_iterator(link_type _node) : node(_node) {}
        __list_iterator() : node(nullptr) {}
        __list_iterator(const iterator& x) : node(x.node) {}
        bool operator==(const self& x) const
        {
            return node == x.node;
        }
        bool operator!=(const self& x) const
        {
            return node != x.node;
        }
        reference operator*() const
        {
            return (*node).data;
        }
        pointer operator->() const
        {
            return &(operator*());
        }
        self& operator++() // pre++
        {
            node = node->next;
            return *this;
        }
        self operator++(int) // post++
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--()
        {
            node = node->prev;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            --*this;
            return tmp;
        }
    };
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type= std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = Allocator::pointer;
    using const_pointer = Allocator::const_pointer;
    using iterator = __list_iterator<T, T&, T*>; // bidirectional iterator
    using const_iterator = __list_iterator<T, const T&, const T*>;
    using reverse_iterator = tstd::reverse_iterator<iterator>;
    using const_reverse_iterator = tstd::reverse_iterator<const_iterator>;
private:
    using link_type = list_node*;
    using node_allocator_type = typename Allocator::template rebind<list_node>::other;
    // cyclic doubly linked list, next point to first element, prev point to last element(and node represent end()), only one pointer could represent the whole list
    link_type node;
    Allocator alloc;
    node_allocator_type node_alloc; // must be defined after alloc, space to optimize
private:
    // auxiliary functions
    // allocate but do not construct
    [[nodiscard]] link_type get_node()
    {
        return node_alloc.allocate(1);
    }
    // deallocate a node
    void release_node(link_type p)
    {
        if (p)
        {
            node_alloc.deallocate(p, 1);
        }
    }
    // allocate and construct
    template<typename... Args>
    [[nodiscard]] link_type construct_node(Args&&... args)
    {
        link_type p = get_node();
        node_alloc.construct(&p->data, std::forward<Args>(args)...);
        return p;
    }
    // destroy a node
    void destroy_node(link_type p)
    {
        if (p)
        {
            node_alloc.destroy(&p->data);
            release_node(p);
        }
    }
    // initialize list with an empty node
    void init_empty()
    {
        node = get_node();
        node->next = node;
        node->prev = node;
    }
    // insert src to location before pos, return the inserted element
    link_type insert_before(link_type pos, link_type src)
    {
        if (pos && src)
        {
            src->next = pos;
            src->prev = pos->prev;
            pos->prev->next = src;
            pos->prev = src;
            return src;
        }
        return pos;
    }
    // insert a link to location before pos, return the first inserted element, if no element inserted, return pos
    link_type insert_before(link_type pos, link_type first, link_type last)
    {
        if (pos && first && last)
        {
            last->next = pos;
            first->prev = pos->prev;
            pos->prev->next = first;
            pos->prev = last;
            return first;
        }
        return pos;
    }
    // construct a link
    void construct_a_link(link_type& first, link_type& last, size_type count, const T& value = T())
    {
        if (count == 0)
        {
            first = last = nullptr;
        }
        else
        {
            first = last = construct_node(value);
            --count;
            link_type prev_last = last;
            while (count--)
            {
                last = construct_node(value);
                prev_last->next = last;
                last->prev = prev_last;
                prev_last = last;
            }
        }
    }
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    void construct_a_link(link_type& first, link_type& last, InputIterator d_first, InputIterator d_last)
    {
        if (d_first == d_last)
        {
            first = last = nullptr;
        }
        else
        {
            first = last = construct_node(*d_first);
            ++d_first;
            for (link_type prev_last = last; d_first != d_last; ++d_first)
            {
                last = construct_node(*d_first);
                prev_last->next = last;
                last->prev = prev_last;
                prev_last = last;
            }
        }
    }
    // free elements (include first and last), test empty before call free_elements
    void free_elements(link_type first, link_type last)
    {
        auto cur_node = first;
        while (cur_node != last)
        {
            auto tmp = cur_node->next; // must record next before destroy
            destroy_node(cur_node);
            cur_node = tmp;
        }
        destroy_node(last);
    }
    // free all elements
    void free_all_elements()
    {
        if (!empty())
        {
            free_elements(node->next, node->prev);
        }
        node->next = node->prev = node;
    }
    // remove elements (include first and last), test empty before call remove_elements if necessary
    link_type remove_elements(link_type first, link_type last)
    {
        first->prev->next = last->next;
        last->next->prev = first->prev;
        link_type ret = last->next;
        free_elements(first, last);
        return ret;
    }
    // move from other list, ensure this list is empty before move
    void move_from(list&& other)
    {
        if (!other.empty())
        {
            insert_before(node, other.node->next, other.node->prev);
            other.node->next = other.node->prev = other.node;
        }
    }
    // transfer nodes from one list to location before pos (could be in one list)
    void transfer(const_iterator pos, const_iterator first, const_iterator last)
    {
        if (first == last)
        {
            return;
        }
        link_type first_node = first.node;
        link_type last_node = last.node->prev;
        link_type pos_node = pos.node;
        link_type first_prev = first.node->prev;
        first_node->prev = pos_node->prev;
        pos_node->prev->next = first_node;
        last_node->next = pos_node;
        pos_node->prev = last_node;

        first_prev->next = last.node;
        last.node->prev = first_prev;
    }
    // auxiliary function for debug
    void print_all_nodes() const
    {
        std::cout << std::hex;
        if (node)
        {
            std::cout << "head node: " << node << ", next: " << node->next << ", prev: " << node->prev << std::endl;
        }
        link_type tmp = node->next;
        while (tmp != node)
        {
            std::cout << "value node: " << tmp << ", next: " << tmp->next << ", prev: " << tmp->prev << ", value: " << std::dec << tmp->data << std::endl;
            tmp = tmp->next;
        }
    }
public:
    list() // 1
        : alloc(Allocator())
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
    }
    explicit list(const Allocator& _alloc) // 2
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
    }
    list(size_type count, const T& value, const Allocator& _alloc = Allocator()) // 3
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, count, value);
        insert_before(node, first, last);
    }
    explicit list(size_type count, const Allocator& _alloc = Allocator()) // 4
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, count);
        insert_before(node, first, last);
    }
    template<typename InputIterator, 
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    list(InputIterator _first, InputIterator _last, const Allocator& _alloc = Allocator()) // 5
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, _first, _last);
        insert_before(node, first, last);
    }
    list(const list& other) // 6
        : alloc(other.alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, other.begin(), other.end());
        insert_before(node, first, last);
    }
    list(const list& other, const Allocator& _alloc) // 7s
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, other.begin(), other.end());
        insert_before(node, first, last);
    }
    list(list&& other) // 8
        : alloc(other.alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        move_from(std::move(other));
    }
    list(list&& other, const Allocator& _alloc) // 9
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        if (alloc == Allocator()) // move the whole list
        {
            move_from(std::move(other));
        }
        else // move every element
        {
            link_type first = nullptr, last = nullptr;
            construct_a_link(first, last, tstd::make_move_iterator(other.begin()), tstd::make_move_iterator(other.end()));
            insert_before(node, first, last);
        }
    }
    list(std::initializer_list<T> il, const Allocator& _alloc = Allocator()) // 10
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, il.begin(), il.end());
        insert_before(node, first, last);
    }
    ~list()
    {
        // print_all_nodes();
        free_all_elements();
        release_node(node);
    }
    // assignment
    list& operator=(const list& other)
    {
        free_all_elements();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, other.begin(), other.end());
        insert_before(node, first, last);
        return *this;
    }
    list& operator=(list&& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value)
    {
        free_all_elements();
        move_from(std::move(other));
        return *this;
    }
    list& operator=(std::initializer_list<T> il)
    {
        free_all_elements();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, il.begin(), il.end());
        insert_before(node, first, last);
        return *this;
    }
    void assign(size_type count, const T& value)
    {
        free_all_elements();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, count, value);
        insert_before(node, first, last);
    }
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    void assign(InputIterator _first, InputIterator _last)
    {
        free_all_elements();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, _first, _last);
        insert_before(node, first, last);
    }
    void assign(std::initializer_list<T> il)
    {
        free_all_elements();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, il.begin(), il.end());
        insert_before(node, first, last);
    }
    allocator_type get_allocator() const noexcept
    {
        return alloc;
    }
    // element access
    reference front()
    {
        return node->next->data;
    }
    const_reference front() const
    {
        return node->next->data;
    }
    reference back()
    {
        return node->prev->data;
    }
    const_reference back() const
    {
        return node->prev->data;
    }
    // iterators
    iterator begin() noexcept
    {
        return iterator(node->next);
    }
    const_iterator begin() const noexcept
    {
        return const_iterator(node->next);
    }
    const_iterator cbegin() const noexcept
    {
        return const_iterator(node->next);
    }
    iterator end() noexcept
    {
        return iterator(node);
    }
    const_iterator end() const noexcept
    {
        return const_iterator(node);
    }
    const_iterator cend() const noexcept
    {
        return const_iterator(node);
    }
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    // size and capacity
    [[nodiscard]] bool empty() const noexcept
    {
        return node->next == node;
    }
    // should be O(1) since C++11, but still O(N) here
    size_type size() const noexcept
    {
        return tstd::distance(begin(), end());
    }
    size_type max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }
    // modifiers
    void clear() noexcept
    {
        free_all_elements();
    }
    iterator insert(const_iterator pos, const T& value) // 1
    {
        return insert_before(pos.node, construct_node(value));
    }
    iterator insert(const_iterator pos, T&& value) // 2
    {
        return insert_before(pos.node, construct_node(std::move(value)));
    }
    iterator insert(const_iterator pos, size_type count, const T& value) //3
    {
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, count, value);
        return insert_before(pos.node, first, last);
    }
    template<typename InputIterator, 
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    iterator insert(const_iterator pos, InputIterator _first, InputIterator _last) // 4
    {
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, _first, _last);
        return insert_before(pos.node, first, last);
    }
    iterator insert(const_iterator pos, std::initializer_list<T> il) // 5
    {
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, il.begin(), il.end());
        return insert_before(pos.node, first, last);
    }
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args)
    {
        return insert_before(pos.node, construct_node(std::forward<Args>(args)...));
    }
    iterator erase(const_iterator pos) // 1
    {
        return remove_elements(pos.node, pos.node);
    }
    iterator erase(const_iterator _first, const_iterator _last) // 2
    {
        if (_first == _last)
        {
            return iterator(_last.node);
        }
        return remove_elements(_first.node, _last.node->prev);
    }
    void push_back(const T& value) // 1
    {
        insert_before(node, construct_node(value));
    }
    void push_back(T&& value) // 2
    {
        insert_before(node, construct_node(std::move(value)));
    }
    template<typename... Args>
    reference emplace_back(Args&&... args)
    {
        link_type p = insert_before(node, construct_node(std::forward<Args>(args)...));
        return p->data;
    }
    void pop_back()
    {
        remove_elements(node->prev, node->prev);
    }
    void push_front(const T& value) // 1
    {
        insert_before(node->next, construct_node(value));
    }
    void push_front(T&& value) // 2
    {
        insert_before(node->next, construct_node(std::move(value)));
    }
    template<typename... Args>
    reference emplace_front(Args&&... args)
    {
        link_type p = insert_before(node->next, construct_node(std::forward<Args>(args)...));
        return p->data;
    }
    void pop_front()
    {
        remove_elements(node->next, node->next);
    }
    void resize(size_type count) // 1
    {
        resize(count, value_type());
    }
    void resize(size_type count, const value_type& value) // 2
    {
        size_type sz = size();
        if (sz > count)
        {
            erase(tstd::prev(end(), sz - count), end());
        }
        else if (sz < count)
        {
            link_type first = nullptr, last = nullptr;
            construct_a_link(first, last, count - sz, value);
            insert_before(node, first, last);
        }
    }
    void swap(list& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value)
    {
        if (!empty())
        {
            if (other.empty()) // other is empty
            {
                other.node->next = node->next;
                other.node->prev = node->prev;
                other.node->next->prev = other.node->prev->next = other.node;
                node->next = node->prev = node;
            }
            else // both are not empty
            {
                tstd::swap(node->next, other.node->next);
                tstd::swap(node->prev, other.node->prev);
                node->next->prev = node->prev->next= node;
                other.node->next->prev = other.node->prev->next= other.node;
            }
        }
        else if (!other.empty()) // this is empty
        {
            node->next = other.node->next;
            node->prev = other.node->prev;
            node->next->prev = node->prev->next = node;
            other.node->next = other.node->prev = other.node;
        }
    }
    // operations/algorithms
    // merge, this and other should be sorted in ascending order
    void merge(list& other) // 1
    {
        iterator first1 = begin();
        iterator first2 = other.begin();
        while (first1 != end() && first2 != other.end())
        {
            if (*first2 < *first1)
            {
                iterator tmp = tstd::next(first2);
                transfer(first1, first2, tmp);
                first2 = tmp;
            }
            else
            {
                ++first1;
            }
        }
        if (first2 != other.end())
        {
            transfer(end(), first2, other.end());
        }
    }
    void merge(list&& other) // 2
    {
        merge(other); // call merge(list&)
    }
    template<typename Compare>
    void merge(list& other, Compare cmp) // 3
    {
        iterator first1 = begin();
        iterator first2 = other.begin();
        while (first1 != end() && first2 != other.end())
        {
            if (cmp(*first2, *first1))
            {
                iterator tmp = tstd::next(first2);
                transfer(first1, first2, tmp);
                first2 = tmp;
            }
            else
            {
                ++first1;
            }
        }
        if (first2 != other.end())
        {
            transfer(end(), first2, other.end());
        }
    }
    template<typename Compare>
    void mrege(list&& other, Compare cmp) // 4
    {
        merge(other, cmp);
    }
    // splice
    void splice(const_iterator pos, list& other)
    {
        assert(alloc == other.alloc);
        transfer(pos, other.begin(), other.end());
    }
    void splice(const_iterator pos, list&& other)
    {
        assert(alloc == other.alloc);
        transfer(pos, other.begin(), other.end());
    }
    void splice(const_iterator pos, [[maybe_unused]] list& other, const_iterator it)
    {
        assert(alloc == other.alloc);
        transfer(pos, it, tstd::next(it));
    }
    void splice(const_iterator pos, [[maybe_unused]] list&& other, const_iterator it)
    {
        assert(alloc == other.alloc);
        transfer(pos, it, tstd::next(it));
    }
    void splice(const_iterator pos, [[maybe_unused]] list& other, const_iterator _first, const_iterator _last)
    {
        assert(alloc == other.alloc);
        transfer(pos, _first, _last);
    }
    void splice(const_iterator pos, [[maybe_unused]] list&& other, const_iterator _first, const_iterator _last)
    {
        assert(alloc == other.alloc);
        transfer(pos, _first, _last);
    }
    // remove, remove_if
    size_type remove(const T& value)
    {
        size_type count = 0;
        for (iterator iter = begin(); iter != end(); ++iter)
        {
            if (*iter == value)
            {
                iter = erase(iter);
                ++count;
            }
        }
        return count;
    }
    template<typename UnaryPredicate>
    size_type remove_if(UnaryPredicate p)
    {
        size_type count = 0;
        for (iterator iter = begin(); iter != end();)
        {
            if (p(*iter))
            {
                iter = erase(iter);
                ++count;
            }
            else
            {
                ++iter;
            }
        }
        return count;
    }
    // reverse
    void reverse() noexcept
    {
        // empty or only one element
        if (node->next == node || node->next->next == node)
        {
            return;
        }
        iterator first = begin();
        ++first;
        while (first != end())
        {
            iterator old = first;
            ++first;
            transfer(begin(), old, first);
        }
    }
    // unique
    size_type unique()
    {
        if (empty())
        {
            return 0;
        }
        size_type count = 0;
        for (iterator iter = begin(), next = tstd::next(iter); next != end(); next = tstd::next(iter))
        {
            if (*iter == *next)
            {
                erase(next);
                ++count;
            }
            else
            {
                ++iter;
            }
        }
        return count;
    }
    template<typename BinaryPredicate>
    size_type unique(BinaryPredicate p)
    {
        if (empty())
        {
            return 0;
        }
        size_type count = 0;
        for (iterator iter = begin(), next = tstd::next(iter); next != end(); next = tstd::next(iter))
        {
            if (p(*iter, *next))
            {
                erase(next);
                ++count;
            }
            else
            {
                ++iter;
            }
        }
        return count;
    }
    // sort : quick sort
    void sort()
    {
        // empty or only one element
        if (node->next == node || node->next->next == node)
        {
            return;
        }
        list carry;
        list counter[64];
        int fill = 0;
        while (!empty())
        {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty())
            {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill)
            {
                ++fill;
            }
        }
        for (int i = 1; i < fill; ++i)
        {
            counter[i].merge(counter[i-1]);
        }
        swap(counter[fill-1]);
    }
    template<typename Compare>
    void sort(Compare cmp)
    {
         // empty or only one element
        if (node->next == node || node->next->next == node)
        {
            return;
        }
        list carry;
        list counter[64];
        int fill = 0;
        while (!empty())
        {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty())
            {
                counter[i].merge(carry, cmp);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill)
            {
                ++fill;
            }
        }
        for (int i = 1; i < fill; ++i)
        {
            counter[i].merge(counter[i-1], cmp);
        }
        swap(counter[fill-1]);
    }
};

// non-member operations
// a non-standard compare function for tstd::list
// equal 0 less -1 greater 1
template<typename T, typename Allocator>
constexpr int _cmp_list(const tstd::list<T, Allocator>& lhs, const tstd::list<T, Allocator>& rhs)
{
    auto iter1 = lhs.begin();
    auto iter2 = rhs.begin();
    for (; iter1 != lhs.end() && iter2 != rhs.end(); ++iter1, ++iter2)
    {
        if (*iter1 == *iter2)
        {
            continue;
        }
        return *iter1 > *iter2 ? 1 : -1;
    }
    if (iter1 != lhs.end())
    {
        return 1;
    }
    if (iter2 != rhs.end())
    {
        return -1;
    }
    return 0;
}

// comparasions
template<typename T, typename Allocator>
constexpr bool operator==(const tstd::list<T, Allocator>& lhs, const tstd::list<T, Allocator>& rhs)
{
    return _cmp_list(lhs, rhs) == 0;
}
template<typename T, typename Allocator>
constexpr bool operator!=(const tstd::list<T, Allocator>& lhs, const tstd::list<T, Allocator>& rhs)
{
    return _cmp_list(lhs, rhs) != 0;
}
template<typename T, typename Allocator>
constexpr bool operator<(const tstd::list<T, Allocator>& lhs, const tstd::list<T, Allocator>& rhs)
{
    return _cmp_list(lhs, rhs) < 0;
}
template<typename T, typename Allocator>
constexpr bool operator<=(const tstd::list<T, Allocator>& lhs, const tstd::list<T, Allocator>& rhs)
{
    return _cmp_list(lhs, rhs) <= 0;
}
template<typename T, typename Allocator>
constexpr bool operator>(const tstd::list<T, Allocator>& lhs, const tstd::list<T, Allocator>& rhs)
{
    return _cmp_list(lhs, rhs) > 0;
}
template<typename T, typename Allocator>
constexpr bool operator>=(const tstd::list<T, Allocator>& lhs, const tstd::list<T, Allocator>& rhs)
{
    return _cmp_list(lhs, rhs) >= 0;
}

// global swap for tstd::list
template<typename T, typename Allocator>
constexpr void swap(tstd::list<T, Allocator>& lhs, tstd::list<T, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace tstd

#endif // TLIST_HPP