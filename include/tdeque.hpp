#ifndef TDEQUE_HPP
#define TDEQUE_HPP

#include <tstl_allocator.hpp>
#include <tstl_uninitialized.hpp>
#include <titerator.hpp>
#include <tutility.hpp>
#include <initializer_list>
#include <type_traits>
#include <algorithm> // todo: replace with <talgorithm.hpp>, replace all algorithms with tstd::xxx

namespace tstd
{

template<typename T, typename Allocator = tstd::allocator<T>>
class deque
{
private:
    // inline static varaible does not need definition outside class, constexpr static variable is implicitly inline
    inline constexpr static std::size_t ElemSize = 64; // elements in one buffer
    inline constexpr static std::size_t initial_map_size = 8; // minimum map size
    // iterator
    template<typename T2, typename Ref, typename Ptr, std::size_t _ElemSize>
    struct __deque_iterator
    {
        using iterator_category = std::random_access_iterator_tag;
        using value_type  = T2;
        using pointer = Ptr;
        using reference = Ref;
        using difference_type = std::ptrdiff_t;

        using size_type = std::size_t;
        using map_pointer = T2**;
        using self = __deque_iterator;
        using iterator = __deque_iterator<T2, T2&, T2*, _ElemSize>;
        T* cur;     // current element
        T* first;   // first element of current buffer
        T* last;    // off-the-end element pointer of current buffer
        map_pointer node; // buffer pointer

        __deque_iterator()
            : cur(nullptr)
            , first(nullptr)
            , last(nullptr)
            , node(nullptr)
        {
        }
        __deque_iterator(const iterator& other)
            : cur(other.cur)
            , first(other.first)
            , last(other.last)
            , node(other.node)
        {
        }
        void set_node(map_pointer new_node)
        {
            node = new_node;
            first = *new_node;
            last = first + _ElemSize;
        }
        reference operator*() const
        {
            return *cur;
        }
        pointer operator->() const
        {
            return &(operator*());
        }
        difference_type operator-(const self& x) const
        {
            return difference_type(_ElemSize) * (node - x.node - 1) + 
                (cur - first) + (x.last - x.cur);
        }
        self& operator++() // pre++
        {
            ++cur;
            if (cur == last)
            {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }
        self operator++(int) // post++
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--() // pre--
        {
            if (cur == first)
            {
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }
        self operator--(int) // post--
        {
            self tmp = *this;
            --*this;
            return tmp;
        }
        self& operator+=(difference_type n)
        {
            difference_type offset = n + (cur - first); // offset relative to first of current buffer
            // in same buffer
            if (offset >= 0 && offset < difference_type(_ElemSize))
            {
                cur += n;
            }
            // not in same buffer
            else
            {
                difference_type node_offset = offset > 0 ? offset / difference_type(_ElemSize)
                    : - difference_type((-offset - 1) / _ElemSize) - 1; // -ElemSize -> -1, -ElemSize-1 -> -2
                // switch to correct buffer
                set_node(node + node_offset);
                // switch to correct element
                cur = first + (offset - node_offset * difference_type(_ElemSize));
            }
            return *this;
        }
        self operator+(difference_type n) const
        {
            self tmp = *this;
            return tmp += n;
        }
        self& operator-=(difference_type n)
        {
            return *this += -n;
        }
        self operator-(difference_type n) const
        {
            self tmp = *this;
            return tmp -= n;
        }
        reference operator[](difference_type n) const
        {
            return *(*this + n);
        }
        bool operator==(const self& x) const
        {
            return cur == x.cur;
        }
        bool operator!=(const self& x) const
        {
            return !(*this == x);
        }
        bool operator<(const self& x) const
        {
            return (node == x.node) ? (cur < x.cur) : node < x.node;
        }
        bool operator<=(const self& x) const
        {
            return !(x < *this);
        }
        bool operator>(const self& x) const
        {
            return x < *this;
        }
        bool operator>=(const self& x) const
        {
            return !(*this < x);
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
    using iterator = __deque_iterator<T, T&, T*, deque::ElemSize>; // random access iterator
    using const_iterator = __deque_iterator<T, const T&, const T*, deque::ElemSize>;
    using reverse_iterator = tstd::reverse_iterator<iterator>;
    using const_reverse_iterator = tstd::reverse_iterator<const_iterator>;
private:
    using map_pointer = pointer*; // element of map
    using map_allocator = typename Allocator::template rebind<pointer>::other;
private:
    map_pointer map;            // start of map
    size_type map_size;         // size of map
    iterator start, finish;     // begin() and end() iterator
    allocator_type alloc;       // allocator of elements
    map_allocator map_alloc;    // allocator of map
private:
    // auxiliary functions
    // allocate one buffer
    pointer allocate_node()
    {
        return alloc.allocate(ElemSize);
    }
    void deallocate_node(pointer p)
    {
        alloc.deallocate(p, ElemSize);
    }
    // get ready for all memory
    void create_map_and_nodes(size_type num_elements)
    {
        size_type num_nodes = num_elements / ElemSize + 1;
        map_size = std::max(initial_map_size, num_nodes + 2);
        map = map_alloc.allocate(map_size);
        for (int i = 0; i < map_size; ++i)
        {
            map[i] = nullptr;
        }
        // let node_start, node_finish point to the middle of all nodes
        map_pointer node_start = map + (map_size - num_nodes) / 2;
        map_pointer node_finish = node_start + num_nodes - 1;
        for (map_pointer cur = node_start; cur <= node_finish; ++cur)
        {
            *cur = allocate_node();
        }
        start.set_node(node_start);
        finish.set_node(node_finish);
        start.cur = start.first;
        finish.cur = finish.first + num_elements % ElemSize;
    }
    // get ready for memory and fill all values
    void fill_initialize(size_type n, const value_type& value)
    {
        create_map_and_nodes(n);
        map_pointer cur;
        for (cur = start.node; cur < finish.node; ++cur)
        {
            tstd::uninitialized_fill(*cur, *cur + ElemSize, value);
        }
        tstd::uninitialized_fill(finish.first, finish.cur, value);
    }
    // get ready for memory and copy initilize all elements
    template<typename InputIterator>
    void copy_initialize(size_type n, InputIterator first, InputIterator last)
    {
        create_map_and_nodes(n);
        tstd::uninitialized_copy(first, last, start);
    }
    // get ready for memory and move initilize all elements
    template<typename InputIterator>
    void move_initialize(size_type n, InputIterator first, InputIterator last)
    {
        create_map_and_nodes(n);
        tstd::uninitialized_move(first, last, start);
    }
    // move from another deque
    void move_from(deque&& other)
    {
        map = other.map;
        map_size = other.map_size;
        start = other.start;
        finish = other.finish;
        other.map = nullptr;
        other.map_size = 0;
        other.create_map_and_nodes(0); // keep other in a valid state
    }
    template<typename InputIterator>
    void fill_range(InputIterator first, InputIterator last, const T& value)
    {
        for (auto iter = first; iter != last; ++iter)
        {
            *iter = value;
        }
    }
    void reserve_map_at_back(size_type nodes_to_add = 1)
    {
        if (nodes_to_add + 1 > map_size - (finish.node - map))
        {
            reallocate_map(nodes_to_add, false);
        }
    }
    void reserve_map_at_front(size_type nodes_to_add = 1)
    {
        if (nodes_to_add > start.node - map)
        {
            reallocate_map(nodes_to_add, true);
        }
    }
    // reallocate map: allocate new map, copy and release old map
    // every time new map size should be at least double of current number of nodes
    void reallocate_map(size_type nodes_to_add, bool add_at_front)
    {
        size_type old_num_nodes = finish.node - start.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;
        map_pointer new_start;
        if (map_size > 2 * new_num_nodes) // use old map
        {
            new_start = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
            if (new_start < start.node)
            {
                std::copy(start.node, finish.node + 1, new_start);
            }
            else
            {
                std::copy_backward(start.node, finish.node + 1, new_start + old_num_nodes);
            }
        }
        else // allocate a new map
        {
            size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
            map_pointer new_map = map_alloc.allocate(new_map_size);
            new_start = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
            std::copy(start.node, finish.node + 1, new_start);
            map_alloc.deallocate(map, map_size);
            map = new_map;
            map_size = new_map_size;
        }
        start.set_node(new_start);
        finish.set_node(new_start + old_num_nodes - 1);
    }
    // free map and free all buffers
    void release_map()
    {
        if (map)
        {
            for (auto p = start.node; p <= finish.node; ++p)
            {
                deallocate_node(*p);
            }
            map_alloc.deallocate(map, map_size);
        }
        map = nullptr;
        map_size = 0;
    }
    // destruct all elements and free extra buffer (all buffers except first)
    void clear_elements()
    {
        for (iterator iter = finish; iter != start;)
        {
            alloc.destroy((--iter).cur);
        }
        for (auto node = start.node + 1; node <= finish.node; ++node)
        {
            deallocate_node(*node);
        }
        finish = start;
    }
    // get ready for inserting, default construct extra values at back or front, move elements appropriately, return first inserted position
    // the easiest way to implement insert, still great room for optimization.
    [[nodiscard]] iterator get_ready_for_insert(size_type elems_num, const_iterator pos)
    {
        difference_type index = pos - start;
        iterator insert_pos = const_iterator_to_iterator(pos);
        if (index < size() / 2) // insert to front
        {
            size_type nodes_to_add = (elems_num - (start.cur - start.first) - 1 + ElemSize) / ElemSize;
            reserve_map_at_front(nodes_to_add);
            for (map_pointer node = start.node - nodes_to_add; node < start.node; ++node)
            {
                *node = allocate_node();
            }
            start -= elems_num;
            tstd::uninitialized_fill_n(start, elems_num, T());
            insert_pos = start + index;
            std::move(start + elems_num, insert_pos + elems_num, start);
        }
        else // insert to back
        {
            size_type nodes_to_add = (elems_num - (finish.last - finish.cur) + ElemSize) / ElemSize;
            reserve_map_at_back(nodes_to_add);
            for (map_pointer node = finish.node + nodes_to_add; node > finish.node; --node)
            {
                *node = allocate_node();
            }
            tstd::uninitialized_fill_n(finish, elems_num, T());
            finish += elems_num;
            insert_pos = start + index;
            std::move_backward(insert_pos, finish - elems_num, finish);
        }
        return insert_pos;
    }
    // convert const_iterator to iterator
    iterator const_iterator_to_iterator(const_iterator iter)
    {
        iterator tmp;
        tmp.cur = iter.cur;
        tmp.first = iter.first;
        tmp.last = iter.last;
        tmp.node = iter.node;
        return tmp;
    }
public:
    deque() // 1
        : alloc()
        , map_alloc(alloc)
        , map(nullptr)
        , map_size(0)
    {
        fill_initialize(0, T());
    }
    explicit deque(const Allocator& _alloc) // 2
        : alloc(_alloc)
        , map_alloc(alloc)
        , map(nullptr)
        , map_size(0)
    {
        fill_initialize(0, T());
    }
    deque(size_type count, const T& value, const Allocator& _alloc = Allocator()) // 3
        : alloc(_alloc)
        , map_alloc(alloc)
        , map(nullptr)
        , map_size(0)
    {
        fill_initialize(count, value);
    }
    explicit deque(size_type count, const Allocator& _alloc = Allocator()) // 4
        : alloc(_alloc)
        , map_alloc(alloc)
        , map(nullptr)
        , map_size(0)
    {
        fill_initialize(count, T());
    }
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    deque(InputIterator first, InputIterator last, const Allocator& _alloc = Allocator()) // 5
        : alloc(_alloc)
        , map_alloc(alloc)
        , map(nullptr)
        , map_size(0)
    {
        copy_initialize(last - first, first, last);
    }
    deque(const deque& other) // 6
        : alloc(other.alloc)
        , map_alloc(alloc)
        , map(nullptr)
        , map_size(0)
    {
        copy_initialize(other.size(), other.begin(), other.end());
    }
    deque(const deque& other, const Allocator& _alloc) // 7
        : alloc(_alloc)
        , map_alloc(alloc)
        , map(nullptr)
        , map_size(0)
    {
        copy_initialize(other.size(), other.begin(), other.end());
    }
    deque(deque&& other) // 8
        : alloc(other.alloc)
        , map_alloc(alloc)
        , map(nullptr)
        , map_size(0)
    {
        move_from(std::move(other));
    }
    deque(deque&& other, const Allocator& _alloc) // 9
        : alloc(_alloc)
        , map_alloc(alloc)
        , map(nullptr)
        , map_size(0)
    {
        if (_alloc == other.get_allocator())
        {
            move_from(std::move(other));
        }
        else
        {
            move_initialize(other.size(), other.begin(), other.end());
        }
    }
    deque(std::initializer_list<T> il, const Allocator& _alloc = Allocator()) // 10
        : alloc(_alloc)
        , map_alloc(alloc)
        , map(nullptr)
        , map_size(0)
    {
        copy_initialize(il.size(), il.begin(), il.end());
    }
    ~deque()
    {
        clear_elements();
        release_map();
    }
    // assignment
    deque& operator=(const deque& other) // 1
    {
        clear_elements();
        release_map();
        copy_initialize(other.size(), other.begin(), other.end());
        return *this;
    }
    deque& operator=(deque&& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value) // 2
    {
        clear_elements();
        release_map();
        move_from(std::move(other));
        return *this;
    }
    deque& operator=(std::initializer_list<T> il) // 3
    {
        clear_elements();
        release_map();
        copy_initialize(il.size(), il.begin(), il.end());
        return *this;
    }
    void assign(size_type count, const T& value) // 1
    {
        clear_elements();
        release_map();
        fill_initialize(count, value);
    }
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    void assign(InputIterator first, InputIterator last) // 2
    {
        clear_elements();
        release_map();
        copy_initialize(last - first, first, last);
    }
    void assign(std::initializer_list<T> il) // 3
    {
        clear_elements();
        release_map();
        copy_initialize(il.size(), il.begin(), il.end());
    }
    // allocator
    allocator_type get_allocator() const noexcept
    {
        return alloc;
    }
    // element access
    reference at(size_type pos)
    {
        if (pos > size())
        {
            throw std::out_of_range("deque::at : input index is out of bounds");
        }
        return *(start + pos);
    }
    const_reference at(size_type pos) const
    {
        if (pos > size)
        {
            throw std::out_of_range("deque::at : input index is out of bounds");
        }
        return *(start + pos);
    }
    reference operator[](size_type pos)
    {
        return *(start + pos);
    }
    const_reference operator[](size_type pos) const
    {
        return *(start + pos);
    }
    reference front()
    {
        return *start;
    }
    const_reference front() const
    {
        return *start;
    }
    reference back()
    {
        return *(finish - 1);
    }
    const_reference back() const
    {
        return *(finish - 1);
    }
    // iterators
    iterator begin() noexcept
    {
        return start;
    }
    const_iterator begin() const noexcept
    {
        return start;
    }
    const_iterator cbegin() const noexcept
    {
        return start;
    }
    iterator end() noexcept
    {
        return finish;
    }
    const_iterator end() const noexcept
    {
        return finish;
    }
    const_iterator cend() const noexcept
    {
        return finish;
    }
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(finish);
    }
    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(finish);
    }
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(finish);
    }
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(start);
    }
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(start);
    }
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(start);
    }
    // size and capacity
    [[nodiscard]] bool empty() const noexcept
    {
        return start == finish;
    }
    size_type size() const noexcept
    {
        return finish - start;
    }
    size_type max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }
    void shrink_to_fit()
    {
        // buffers are freed dynamically, so do nothing.
    }
    // modifiers
    void clear() noexcept
    {
        clear_elements();
    }
    iterator insert(const_iterator pos, const T& value) // 1
    {
        iterator insert_pos = get_ready_for_insert(1, pos);
        *insert_pos = value;
        return insert_pos;
    }
    iterator insert(const_iterator pos, T&& value) // 2
    {
        iterator insert_pos = get_ready_for_insert(1, pos);
        *insert_pos = std::move(value);
        return insert_pos;
    }
    iterator insert(const_iterator pos, size_type count, const T& value) // 3
    {
        iterator insert_pos = get_ready_for_insert(count, pos);
        std::fill_n(insert_pos, count, value);
        return insert_pos;
    }
    template<typename InputIterator, 
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    iterator insert(const_iterator pos, InputIterator first, InputIterator last) // 4
    {
        difference_type count = tstd::distance(first, last);
        iterator insert_pos = get_ready_for_insert(count, pos);
        std::copy_n(first, count, insert_pos);
        return insert_pos;
    }
    iterator insert(const_iterator pos, std::initializer_list<T> il) // 5
    {
        iterator insert_pos = get_ready_for_insert(il.size(), pos);
        std::copy_n(il.begin(), il.size(), insert_pos);
        return insert_pos;
    }
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args)
    {
        iterator insert_pos = get_ready_for_insert(1, pos);
        *insert_pos = T(std::forward<Args>(args)...);
        return insert_pos;
    }
    iterator erase(const_iterator pos) // 1
    {
        iterator _pos = const_iterator_to_iterator(pos);
        iterator next = tstd::next(_pos);
        difference_type index = _pos - start;
        if (index < size() / 2) // move front elements
        {
            std::move_backward(start, _pos, next);
            pop_front();
        }
        else // move back elements
        {
            std::move(next, finish, _pos);
            pop_back();
        }
        return start + index;
    }
    iterator erase(const_iterator first, const_iterator last) // 2
    {
        iterator _first = const_iterator_to_iterator(first);
        iterator _last = const_iterator_to_iterator(last);
        if (_first == start && _last == finish) // erase all elements
        {
            clear();
            return finish;
        }
        else
        {
            difference_type n = _last - _first; // numbers of elements to erase
            difference_type elems_before = _first - start;
            if (elems_before < (size() - n) / 2) // more elements in back of range, move front elements
            {
                std::move_backward(start, _first, _last);
                iterator new_start = start + n;
                for (auto iter = start; iter < new_start; ++iter)
                {
                    alloc.destroy(iter.cur);
                }
                for (map_pointer cur = start.node; cur < new_start.node; ++cur)
                {
                    deallocate_node(*cur);
                }
                start = new_start;
            }
            else // more elements in front of range, move back elements
            {
                std::move(_last, finish, _first);
                iterator new_finish = finish - n;
                for (auto iter = finish - 1; iter >= new_finish; --iter)
                {
                    alloc.destroy(iter.cur);
                }
                for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
                {
                    deallocate_node(*cur);
                }
                finish = new_finish;
            }
            return start + elems_before;
        }
    }
    void push_back(const T& value) // 1
    {
        if (finish.cur != finish.last - 1) // two of more elements free spaces in last buffer
        {
            alloc.construct(finish.cur, value);
            ++finish.cur;
        }
        else // only one element space left in last buffer
        {
            reserve_map_at_back();
            *(finish.node + 1) = allocate_node();
            alloc.construct(finish.cur, value);
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }
    }
    void push_back(T&& value) // 2
    {
        if (finish.cur != finish.last - 1) // two of more elements free spaces in last buffer
        {
            alloc.construct(finish.cur, std::move(value));
            ++finish.cur;
        }
        else // only one element space left in last buffer
        {
            reserve_map_at_back();
            *(finish.node + 1) = allocate_node();
            alloc.construct(finish.cur, std::move(value));
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }
    }
    template<typename... Args>
    reference emplace_back(Args&&... args)
    {
        if (finish.cur < finish.last - 1) // two of more elements free spaces in last buffer
        {
            alloc.construct(finish.cur, std::forward<Args>(args)...);
            ++finish.cur;
        }
        else // only one element space left in last buffer
        {
            reserve_map_at_back();
            *(finish.node + 1) = allocate_node();
            alloc.construct(finish.cur, std::forward<Args>(args)...);
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }
        return back();
    }
    void pop_back()
    {
        if (finish.cur > finish.first) // two or more elements in last buffer
        {
            --finish.cur;
            alloc.destroy(finish.cur);
        }
        else // only one element in last buffer
        {
            deallocate_node(finish.first);
            finish.set_node(finish.node - 1);
            finish.cur = finish.last - 1;
            alloc.destroy(finish.cur);
        }
    }
    void push_front(const T& value) // 1
    {
        if (start.cur > start.first) // first buffer is not full
        {
            alloc.construct(start.cur - 1, value);
            --start.cur;
        }
        else // first buffer is full
        {
            reserve_map_at_front();
            *(start.node - 1) = allocate_node();
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            alloc.construct(start.cur, value);
        }
    }
    void push_front(T&& value) // 2
    {
        if (start.cur > start.first) // first buffer is not full
        {
            alloc.construct(start.cur - 1, std::move(value));
            --start.cur;
        }
        else // first buffer is full
        {
            reserve_map_at_front();
            *(start.node - 1) = allocate_node();
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            alloc.construct(start.cur, std::move(value));
        }
    }
    template<typename... Args>
    reference emplace_front(Args&&... args)
    {
        if (start.cur > start.first) // first buffer is not full
        {
            alloc.construct(start.cur - 1, std::forward<Args>(args)...);
            --start.cur;
        }
        else // first buffer is full
        {
            reserve_map_at_front();
            *(start.node - 1) = allocate_node();
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            alloc.construct(start.cur, std::forward<Args>(args)...);
        }
        return front();
    }
    void pop_front()
    {
        if (start.cur < start.last - 1) // two or more elements in first buffer
        {
            alloc.destroy(start.cur);
            ++start.cur;
        }
        else // only one element in first buffer
        {
            alloc.destroy(start.cur);
            deallocate_node(start.first);
            start.set_node(start.node + 1);
            start.cur = start.first;
        }
    }
    void resize(size_type count) // 1
    {
        if (count > size())
        {
            size_type elems_to_insert = count - size();
            iterator insert_pos = get_ready_for_insert(elems_to_insert, finish);
            std::fill_n(insert_pos, elems_to_insert, T());
        }
        else if (count < size())
        {
            erase(start + count, finish);
        }
    }
    void resize(size_type count, const value_type& value) // 2
    {
        if (count > size())
        {
            size_type elems_to_insert = count - size();
            iterator insert_pos = get_ready_for_insert(elems_to_insert, finish);
            std::fill_n(insert_pos, elems_to_insert, value);
        }
        else if (count < size())
        {
            erase(start + count, finish);
        }
    }
    void swap(deque& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value)
    {
        tstd::swap(alloc, other.alloc);
        tstd::swap(map_alloc, other.map_alloc);
        tstd::swap(map, other.map);
        tstd::swap(map_size, other.map_size);
        tstd::swap(start, other.start);
        tstd::swap(finish, other.finish);
    }
};

// non-member operations
// a non-standard compare function for tstd::deque
// equal 0 less -1 greater 1
template<typename T, typename Allocator>
constexpr int _cmp_deque(const tstd::deque<T, Allocator>& lhs, const tstd::deque<T, Allocator>& rhs)
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
constexpr bool operator==(const tstd::deque<T, Allocator>& lhs, const tstd::deque<T, Allocator>& rhs)
{
    return _cmp_deque(lhs, rhs) == 0;
}
template<typename T, typename Allocator>
constexpr bool operator!=(const tstd::deque<T, Allocator>& lhs, const tstd::deque<T, Allocator>& rhs)
{
    return _cmp_deque(lhs, rhs) != 0;
}
template<typename T, typename Allocator>
constexpr bool operator<(const tstd::deque<T, Allocator>& lhs, const tstd::deque<T, Allocator>& rhs)
{
    return _cmp_deque(lhs, rhs) < 0;
}
template<typename T, typename Allocator>
constexpr bool operator<=(const tstd::deque<T, Allocator>& lhs, const tstd::deque<T, Allocator>& rhs)
{
    return _cmp_deque(lhs, rhs) <= 0;
}
template<typename T, typename Allocator>
constexpr bool operator>(const tstd::deque<T, Allocator>& lhs, const tstd::deque<T, Allocator>& rhs)
{
    return _cmp_deque(lhs, rhs) > 0;
}
template<typename T, typename Allocator>
constexpr bool operator>=(const tstd::deque<T, Allocator>& lhs, const tstd::deque<T, Allocator>& rhs)
{
    return _cmp_deque(lhs, rhs) >= 0;
}

// global swap for tstd::deque
template<typename T, typename Allocator>
constexpr void swap(tstd::deque<T, Allocator>& lhs, tstd::deque<T, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace tstd


#endif // TDEQUE_HPP