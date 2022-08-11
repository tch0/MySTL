#ifndef TFORWROD_LIST_HPP
#define TFORWROD_LIST_HPP

#include <tstl_allocator.hpp>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <titerator.hpp>
#include <numeric>
#include <tutility.hpp>
#include <functional>

namespace tstd
{

// implement with a singly linked list
template<typename T, typename Allocator = tstd::allocator<T>>
class forward_list
{
private:
    struct list_node
    {
        list_node* next;
        T data;
    };
    // iterator
    template<typename T2, typename Ref, typename Ptr>
    struct __forward_list_iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T2;
        using pointer = Ptr;
        using reference = Ref;
        using difference_type = std::ptrdiff_t;

        using iterator = __forward_list_iterator<T2, T2&, T2*>;
        using self = __forward_list_iterator<T2, Ref, Ptr>;
        using size_type = std::size_t;
        using link_type = list_node*;
        link_type node; // pointer to node of linked list

        __forward_list_iterator() : node(nullptr) {} // the end() iterator
        __forward_list_iterator(link_type _node) : node(_node) {}
        __forward_list_iterator(const iterator& x) : node(x.node) {}
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
            return node->data;
        }
        pointer operator->() const
        {
            return &(operator*());
        }
        self& operator++()
        {
            node = node->next;
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            ++tmp;
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
    using iterator = __forward_list_iterator<T, T&, T*>; // bidirectional iterator
    using const_iterator = __forward_list_iterator<T, const T&, const T*>;
private:
    using link_type = list_node*;
    using node_allocator_type = typename Allocator::template rebind<list_node>::other;
private:
    link_type node;
    Allocator alloc;
    node_allocator_type node_alloc;
private:
    // auxiliary functions
    // allocate node but do not construct
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
    // initialize forward_list with an empty head node
    void init_empty()
    {
        node = get_node();
        node->next = nullptr;
    }
    // insert src to location after pos, return the inserted element
    link_type insert_after_impl(link_type pos, link_type src)
    {
        if (pos && src)
        {
            link_type tmp = pos->next;
            pos->next = src;
            src->next = tmp;
            return src;
        }
        return pos;
    }
    // insert a link to location after pos, return the last inserted element, if no element inserted, return pos
    link_type insert_after_impl(link_type pos, link_type first, link_type last)
    {
        if (pos && first && last)
        {
            last->next = pos->next;
            pos->next = first;
            return last;
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
                prev_last = last;
            }
        }
    }
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    void construct_a_link(link_type& first, link_type& last, InputIterator s_first, InputIterator s_last)
    {
        if (s_first == s_last)
        {
            first = last = nullptr;
        }
        else
        {
            first = last = construct_node(*s_first);
            ++s_first;
            for (link_type prev_last = last; s_first != s_last; ++s_first)
            {
                last = construct_node(*s_first);
                prev_last->next = last;
                prev_last = last;
            }
        }
    }
    // free elements(include first but not last, last could be nullptr), test empty before call free_element.
    void free_elements(link_type first, link_type last)
    {
        auto cur_node = first;
        while (cur_node != last)
        {
            auto tmp = cur_node->next;
            destroy_node(cur_node);
            cur_node = tmp;
        }
    }
    // free all elements
    void free_all_elements()
    {
        free_elements(node->next, nullptr);
        node->next = nullptr;
    }
    // remove elements (exclude before_first and last, last could be nullptr which represent end of list)
    // remove from the node after befor_first to the node befor last.
    link_type remove_elements(link_type before_first, link_type last)
    {
        if (before_first && before_first->next)
        {
            free_elements(before_first->next, last);
            before_first->next = (last == nullptr ? nullptr : last);
        }
        return last;
    }
    // move from other forward list, ensure current forward lit is empty before move
    void move_from(forward_list&& other)
    {
        node->next = other.node->next;
        other.node->next = nullptr;
    }
    // quick sort implementation (exclude head and tail), it's stable
    template<typename Compare = std::less<value_type>>
    void quick_sort(link_type head, link_type tail, Compare cmp = std::less<value_type>())
    {
        if (head->next == tail || head->next->next == tail) // 0 or 1 element
        {
            return;
        }
        link_type mid = head->next; // first element
        link_type front = head; // first half
        link_type back = mid; // second half
        value_type pivot = mid->data; // first element as partition pivot
        link_type cur = mid->next;
        while (cur != tail)
        {
            if (cmp(cur->data, pivot)) // cur->data < pivot
            {
                front = front->next = cur;
            }
            else
            {
                back = back->next = cur;
            }
            cur = cur->next;
        }
        front->next = mid;
        back->next = tail;
        quick_sort(head, mid, cmp);
        quick_sort(mid, tail);
    }
public:
    forward_list() // 1
        : alloc(Allocator())
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
    }
    explicit forward_list(const Allocator& _alloc) // 2
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
    }
    forward_list(size_type count, const T& value, const Allocator& _alloc = Allocator()) // 3
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, count, value);
        insert_after_impl(node, first, last);
    }
    explicit forward_list(size_type count, const Allocator& _alloc = Allocator()) // 4
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, count);
        insert_after_impl(node, first, last);
    }
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    forward_list(InputIterator _first, InputIterator _last, const Allocator& _alloc = Allocator()) // 5
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, _first, _last);
        insert_after_impl(node, first, last);
    }
    forward_list(const forward_list& other) // 6
        : alloc(other.alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, other.begin(), other.end());
        insert_after_impl(node, first, last);
    }
    forward_list(const forward_list& other, const Allocator& _alloc) // 7
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, other.begin(), other.end());
        insert_after_impl(node, first, last);
    }
    forward_list(forward_list&& other) // 8
        : alloc(std::move(other.alloc))
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        move_from(std::move(other));
    }
    forward_list(forward_list&& other, const Allocator& _alloc) // 9
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        if (alloc == other.get_allocator()) // move the whole forward_list
        {
            move_from(std::move(other));
        }
        else // move every element
        {
            link_type first = nullptr, last = nullptr;
            construct_a_link(first, last, tstd::make_move_iterator(other.begin()), tstd::make_move_iterator(other.end()));
            insert_after_impl(node, first, last);
        }
    }
    forward_list(std::initializer_list<T> il, const Allocator& _alloc = Allocator()) // 10
        : alloc(_alloc)
        , node_alloc(alloc)
        , node(nullptr)
    {
        init_empty();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, il.begin(), il.end());
        insert_after_impl(node, first, last);
    }
    ~forward_list()
    {
        free_all_elements();
        release_node(node);
    }
    // assignment
    forward_list& operator=(const forward_list& other) // 1
    {
        free_all_elements();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, other.begin(), other.end());
        insert_after_impl(node, first, last);
        return *this;
    }
    forward_list& operator=(forward_list&& other) // 2
    {
        free_all_elements();
        move_from(std::move(other));
        return *this;
    }
    forward_list& operator=(std::initializer_list<T> il) // 3
    {
        free_all_elements();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, il.begin(), il.end());
        insert_after_impl(node, first, last);
        return *this;
    }
    void assign(size_type count, const T& value) // 1
    {
        free_all_elements();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, count, value);
        insert_after_impl(node, first, last);
    }
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    void assign(InputIterator _first, InputIterator _last) // 2
    {
        free_all_elements();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, _first, _last);
        insert_after_impl(node, first, last);
    }
    void assign(std::initializer_list<T> il) // 3
    {
        free_all_elements();
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, il.begin(), il.end());
        insert_after_impl(node, first, last);
    }
    // allocator
    allocator_type get_allocator() const noexcept
    {
        return alloc;
    }
    // element access
    reference front()
    {
        return *begin();
    }
    const_reference front() const
    {
        return *begin();
    }
    // iterators
    iterator before_begin() noexcept
    {
        return iterator(node);
    }
    const_iterator before_begin() const noexcept
    {
        return const_iterator(node);
    }
    const_iterator cbefore_begin() const noexcept
    {
        return const_iterator(node);
    }
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
        return iterator();
    }
    const_iterator end() const noexcept
    {
        return const_iterator();
    }
    const_iterator cend() const noexcept
    {
        return const_iterator();
    }
    // size and capacity
    [[nodiscard]] bool empty() const noexcept
    {
        return node->next == nullptr;
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
    iterator insert_after(const_iterator pos, const T& value) // 1
    {
        return insert_after_impl(pos.node, construct_node(value));
    }
    iterator insert_after(const_iterator pos, T&& value) // 2
    {
        return insert_after_impl(pos.node, construct_node(std::move(value)));
    }
    iterator insert_after(const_iterator pos, size_type count, const T& value) // 3
    {
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, count, value);
        return insert_after_impl(pos.node, first, last);
    }
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    iterator insert_after(const_iterator pos, InputIterator _first, InputIterator _last) // 4
    {
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, _first, _last);
        return insert_after_impl(pos.node, first, last);
    }
    iterator insert_after(const_iterator pos, std::initializer_list<T> il) // 5
    {
        link_type first = nullptr, last = nullptr;
        construct_a_link(first, last, il.begin(), il.end());
        return insert_after_impl(pos.node, first, last);
    }
    template<typename... Args>
    iterator emplace_after(const_iterator pos, Args&&... args)
    {
        return insert_after_impl(pos.node, construct_node(std::forward<Args>(args)...));
    }
    iterator erase_after(const_iterator pos) // 1
    {
        if (pos.node && pos.node->next)
        {
            return remove_elements(pos.node, pos.node->next->next);
        }
        return end();
    }
    iterator erase_after(const_iterator first, const_iterator last) // 2
    {
        if (first == last)
        {
            return iterator(last.node);
        }
        return remove_elements(first.node, last.node);
    }
    void push_front(const T& value) // 1
    {
        insert_after_impl(node, construct_node(value));
    }
    void push_front(T&& value) // 2
    {
        insert_after_impl(node, construct_node(std::move(value)));
    }
    template<typename... Args>
    reference emplace_front(Args&&... args)
    {
        link_type p = insert_after_impl(node, construct_node(std::forward<Args>(args)...));
        return p->data;
    }
    void pop_front()
    {
        remove_elements(node, node->next->next);
    }
    void resize(size_type count) // 1
    {
        resize(count, value_type());
    }
    void resize(size_type count, const value_type& value) // 2
    {
        link_type prev = node;
        while (prev->next && count)
        {
            --count;
            prev = prev->next;
        }
        if (count == 0) // size >= count
        {
            if (prev->next) // size > count
            {
                remove_elements(prev, nullptr);
            }
        }
        else // prev->next == nullptr (size < count)
        {
            link_type first = nullptr, last = nullptr;
            construct_a_link(first, last, count, value);
            insert_after_impl(prev, first, last);
        }
    }
    void swap(forward_list& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value)
    {
        tstd::swap(node->next, other.node->next);
    }

    // oeprations/algorithms
    // merge
    // *this and other should be sorted in ascending order, merge other to *this.
    void merge(forward_list& other) // 1
    {
        merge(std::move(other));
    }
    void merge(forward_list&& other) // 2
    {
        link_type this_prev = node;
        while (this_prev->next && other.node->next)
        {
            if (this_prev->next->data < other.node->next->data)
            {
                this_prev = this_prev->next;
            }
            else // this_prev->next->data > other.node->next->data
            {
                link_type tmp = this_prev->next;
                this_prev->next = other.node->next;
                other.node->next = other.node->next->next;
                this_prev = this_prev->next;
                this_prev->next = tmp;
            }
        }
        if (other.node->next) // this_prev->next == nullptr
        {
            this_prev->next = other.node->next;
            other.node->next = nullptr;
        }
    }
    template<typename Compare>
    void merge(forward_list& other, Compare cmp) // 3
    {
        merge(std::move(other), cmp);
    }
    template<typename Compare>
    void merge(forward_list&& other, Compare cmp) // 4
    {
        link_type this_prev = node;
        while (this_prev->next && other.node->next)
        {
            if (cmp(this_prev->next->data, other.node->next->data))
            {
                this_prev = this_prev->next;
            }
            else // this_prev->next->data > other.node->next->data
            {
                link_type tmp = this_prev->next;
                this_prev->next = other.node->next;
                other.node->next = other.node->next->next;
                this_prev = this_prev->next;
                this_prev->next = tmp;
            }
        }
        if (other.node->next) // this_prev->next == nullptr
        {
            this_prev->next = other.node->next;
            other.node->next = nullptr;
        }
    }
    // splice_after
    void splice_after(const_iterator pos, forward_list& other) // 1
    {
        splice_after(pos, std::move(other));
    }
    void splice_after(const_iterator pos, forward_list&& other) // 2
    {
        link_type tmp = pos.node->next;
        pos.node->next = other.node->next;
        other.node->next = nullptr;
        link_type last = pos.node;
        while (last->next != nullptr)
        {
            last = last->next;
        }
        last->next = tmp;
    }
    void splice_after(const_iterator pos, forward_list& other, const_iterator it) // 3
    {
        splice_after(pos, std::move(other), it);
    }
    void splice_after(const_iterator pos, [[maybe_unused]] forward_list&& other, const_iterator it) // 4
    {
        link_type tmp = pos.node->next;
        if (it.node->next)
        {
            link_type tmp2 = it.node->next->next;
            pos.node->next = it.node->next;
            it.node->next->next = tmp;
            it.node->next = tmp2;
        }
    }
    void splice_after(const_iterator pos, forward_list& other, const_iterator _first, const_iterator _last) // 5
    {
        splice_after(pos, std::move(other), _first, _last);
    }
    void splice_after(const_iterator pos, forward_list&& other, const_iterator _first, const_iterator _last) // 6
    {
        link_type tmp = pos.node->next;
        pos.node->next = _first.node->next;
        _first.node->next = _last.node;
        link_type last = pos.node;
        while (last->next != nullptr)
        {
            last = last->next;
        }
        last->next = tmp;
    }
    // remove, remove_if
    size_type remove(const T& value)
    {
        size_type count = 0;
        link_type cur = node;
        while (cur->next)
        {
            if (cur->next->data == value)
            {
                link_type tmp = cur->next;
                cur->next = tmp->next;
                destroy_node(tmp);
                ++count;
            }
            else
            {
                cur = cur->next;
            }
        }
        return count;
    }
    template<typename UnaryPredicate>
    size_type remove_if(UnaryPredicate p)
    {
        size_type count = 0;
        link_type cur = node;
        while (cur->next)
        {
            if (p(cur->next->data))
            {
                link_type tmp = cur->next;
                cur->next = tmp->next;
                destroy_node(tmp);
                ++count;
            }
            else
            {
                cur = cur->next;
            }
        }
        return count;
    }
    // reverse
    void reverse() noexcept
    {
        link_type cur = node->next;
        node->next = nullptr;
        while (cur)
        {
            link_type tmp = cur->next;
            insert_after_impl(node, cur);
            cur = tmp;
        }
    }
    // unique
    size_type unique() // 1
    {
        size_type count = 0;
        link_type cur = node->next;
        while (cur && cur->next)
        {
            if (cur->data == cur->next->data)
            {
                remove_elements(cur, cur->next->next);
                ++count;
            }
            else
            {
                cur = cur->next;
            }
        }
        return count;
    }
    template<typename BinaryPredicate>
    size_type unique(BinaryPredicate p) // 2
    {
        size_type count = 0;
        link_type cur = node->next;
        while (cur && cur->next)
        {
            if (p(cur->data, cur->next->data))
            {
                remove_elements(cur, cur->next->next);
                ++count;
            }
            else
            {
                cur = cur->next;
            }
        }
        return count;
    }
    // sort
    void sort() // 1
    {
        quick_sort(node, nullptr);
    }
    template<typename Compare>
    void sort(Compare cmp) // 2
    {
        quick_sort(node, nullptr, cmp);
    }
};

template<typename T, typename Allocator>
constexpr int _cmp_forward_list(const tstd::forward_list<T, Allocator>& lhs, const tstd::forward_list<T, Allocator>& rhs)
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
constexpr bool operator==(const tstd::forward_list<T, Allocator>& lhs, const tstd::forward_list<T, Allocator>& rhs)
{
    return _cmp_forward_list(lhs, rhs) == 0;
}
template<typename T, typename Allocator>
constexpr bool operator!=(const tstd::forward_list<T, Allocator>& lhs, const tstd::forward_list<T, Allocator>& rhs)
{
    return _cmp_forward_list(lhs, rhs) != 0;
}
template<typename T, typename Allocator>
constexpr bool operator<(const tstd::forward_list<T, Allocator>& lhs, const tstd::forward_list<T, Allocator>& rhs)
{
    return _cmp_forward_list(lhs, rhs) < 0;
}
template<typename T, typename Allocator>
constexpr bool operator<=(const tstd::forward_list<T, Allocator>& lhs, const tstd::forward_list<T, Allocator>& rhs)
{
    return _cmp_forward_list(lhs, rhs) <= 0;
}
template<typename T, typename Allocator>
constexpr bool operator>(const tstd::forward_list<T, Allocator>& lhs, const tstd::forward_list<T, Allocator>& rhs)
{
    return _cmp_forward_list(lhs, rhs) > 0;
}
template<typename T, typename Allocator>
constexpr bool operator>=(const tstd::forward_list<T, Allocator>& lhs, const tstd::forward_list<T, Allocator>& rhs)
{
    return _cmp_forward_list(lhs, rhs) >= 0;
}

// global swap for tstd::forward_list
template<typename T, typename Allocator>
constexpr void swap(tstd::forward_list<T, Allocator>& lhs, tstd::forward_list<T, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace tstd


#endif // TFORWROD_LIST_HPP