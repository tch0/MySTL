#ifndef TVECTOR_HPP
#define TVECTOR_HPP

#include <tstl_allocator.hpp>
#include <tstl_uninitialized.hpp>
#include <titerator.hpp>
#include <initializer_list>
#include <cstddef>
#include <type_traits>
#include <numeric>
#include <stdexcept> // for std::out_of_range
#include <iterator>

namespace tstd
{

template<typename T, typename Allocator = tstd::allocator<T>>
class vector
{
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type= std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = Allocator::pointer;
    using const_pointer = Allocator::const_pointer;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = tstd::reverse_iterator<iterator>;
    using const_reverse_iterator = tstd::reverse_iterator<const_iterator>;
public:
    // constructors
    constexpr vector() noexcept(noexcept(Allocator())) // 1
        : alloc()
        , start(nullptr)
        , finish(nullptr)
        , end_of_storage(nullptr)
    {
    }
    constexpr explicit vector(const Allocator& _alloc) noexcept // 2
        : alloc(_alloc)
        , start(nullptr)
        , finish(nullptr)
        , end_of_storage(nullptr)
    {
    }
    constexpr vector(size_type count, const T& value, const Allocator& _alloc = Allocator()) // 3
        : alloc(_alloc)
        , start(nullptr)
        , finish(nullptr)
        , end_of_storage(nullptr)
    {
        start = alloc.allocate(count * sizeof(T));
        finish = end_of_storage = start + count;
        tstd::uninitialized_fill_n(start, count, value);
    }
    constexpr explicit vector(size_type count, const Allocator& _alloc = Allocator()) // 4
        : alloc(_alloc)
        , start(nullptr)
        , finish(nullptr)
        , end_of_storage(nullptr)
    {
        start = alloc.allocate(count * sizeof(T));
        finish = end_of_storage = start + count;
        tstd::uninitialized_fill_n(start, count, T());
    }
    // In order to avoid ambiguity with overload 3, this overload participates in overload resolution only if InputIterator satisfies LegacyInputIterator.
    // shall use SFINAE
    template<typename InputIterator,
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    constexpr vector(InputIterator first, InputIterator last, const Allocator& _alloc = Allocator()) // 5
        : alloc(_alloc)
        , start(nullptr)
        , finish(nullptr)
        , end_of_storage(nullptr)
    {
        for (auto iter = first; iter != last; ++iter)
        {
            push_back(*iter);
        }
    }
    constexpr vector(const vector& other) // 6
        : alloc(other.alloc)
        , start(nullptr)
        , finish(nullptr)
        , end_of_storage(nullptr)
    {
        if (other.size() > 0)
        {
            start = alloc.allocate(other.size() * sizeof(T));
            finish = end_of_storage = start + other.size();
            tstd::uninitialized_copy(other.begin(), other.end(), start);
        }
    }
    constexpr vector(const vector& other, const Allocator& _alloc) // 7
        : alloc(_alloc)
        , start(nullptr)
        , finish(nullptr)
        , end_of_storage(nullptr)
    {
        if (other.size() > 0)
        {
            start = alloc.allocate(other.size() * sizeof(T));
            finish = end_of_storage = start + other.size();
            tstd::uninitialized_copy(other.begin(), other.end(), start);
        }
    }
    constexpr vector(vector&& other) noexcept // 8
        : alloc(std::move(other.alloc))
        , start(nullptr)
        , finish(nullptr)
        , end_of_storage(nullptr)
    {
        start = other.start;
        finish = other.finish;
        end_of_storage = other.end_of_storage;
        other.start = other.finish = other.end_of_storage = nullptr;
    }
    constexpr vector(vector&& other, const Allocator& _alloc) // 9
        : alloc(_alloc)
        , start(nullptr)
        , finish(nullptr)
        , end_of_storage(nullptr)
    {
        if (alloc == other.get_allocator())
        {
            start = other.start;
            finish = other.finish;
            end_of_storage = other.end_of_storage;
            other.start = other.finish = other.end_of_storage = nullptr;
        }
        else
        {
            if (other.size() > 0)
            {
                start = alloc.allocate(other.size() * sizeof(T));
                tstd::uninitialized_move(other.begin(), other.end(), start);
            }
        }
    }
    constexpr vector(std::initializer_list<T> il, const Allocator& _alloc = Allocator()) // 10
        : alloc(_alloc)
        , start(nullptr)
        , finish(nullptr)
        , end_of_storage(nullptr)
    {
        if (il.size() > 0)
        {
            start = alloc.allocate(il.size() * sizeof(T));
            finish = end_of_storage = start + il.size();
            tstd::uninitialized_copy(il.begin(), il.end(), start);
        }
    }
    // destructor
    constexpr ~vector()
    {
        clear();
        free_all_spaces();
    }
    // operator =
    constexpr vector& operator=(const vector& rhs) // 1
    {
        clear();
        if (rhs.size() <= capacity())
        {
            tstd::uninitialized_copy(rhs.begin(), rhs.end(), start);
            finish = start + rhs.size();
        }
        else
        {
            free_all_spaces();
            start = alloc.allocate(rhs.size() * sizeof(T));
            tstd::uninitialized_copy(rhs.begin(), rhs.end(), start);
            finish = end_of_storage = start + rhs.size();
        }
        return *this;
    }
    constexpr vector& operator=(vector&& rhs) noexcept // 2
    {
        clear();
        free_all_spaces();
        start = rhs.start;
        finish = rhs.finish;
        end_of_storage = rhs.end_of_storage;
        rhs.start = rhs.finish = rhs.end_of_storage = nullptr;
        return *this;
    }
    constexpr vector& operator=(std::initializer_list<T> il) // 3
    {
        assign(il);
        return *this;
    }
    // assign
    constexpr void assign(size_type count, const T& value) // 1
    {
        clear();
        if (count <= capacity())
        {
            tstd::uninitialized_fill_n(start, count, value);
            finish = start + count;
        }
        else
        {
            free_all_spaces();
            start = alloc.allocate(count * sizeof(T));
            tstd::uninitialized_fill_n(start, count, value);
            finish = end_of_storage = start + count;
        }
    }
    template<typename InputIterator, 
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    constexpr void assign(InputIterator first, InputIterator last) // 2
    {
        clear();
        free_all_spaces();
        for (; first != last; ++first)
        {
            push_back(*first);
        }
    }
    constexpr void assign(std::initializer_list<T> il)
    {
        clear();
        if (il.size() <= capacity())
        {
            tstd::uninitialized_copy(il.begin(), il.end(), start);
            finish = start + il.size();
        }
        else
        {
            free_all_spaces();
            start = alloc.allocate(il.size() * sizeof(T));
            tstd::uninitialized_copy(il.begin(), il.end(), start);
            finish = end_of_storage = start + il.size();
        }
    }
    // allocator
    constexpr allocator_type get_allocator() const noexcept
    {
        return alloc;
    }

    // element access
    constexpr reference at(size_type pos)
    {
        if (pos > size())
        {
            throw std::out_of_range("vector::at : input index is out of bounds");
        }
        return *(begin() + pos);
    }
    constexpr const_reference at(size_type pos) const
    {
        if (pos > size())
        {
            throw std::out_of_range("vector::at : input index is out of bounds");
        }
        return *(begin() + pos);
    }
    constexpr reference operator[](size_type pos)
    {
        return *(begin() + pos);
    }
    constexpr const_reference operator[](size_type pos) const
    {
        return *(begin() + pos);
    }
    constexpr reference front()
    {
        return *begin();
    }
    constexpr const_reference front() const
    {
        return *begin();
    }
    constexpr reference back()
    {
        return *(end() - 1);
    }
    constexpr const_reference back() const
    {
        return *(end() - 1);
    }
    constexpr T* data() noexcept
    {
        return start;
    }
    constexpr const T* data() const noexcept
    {
        return start;
    }

    // iterators
    constexpr iterator begin() noexcept
    {
        return start;
    }
    constexpr const_iterator begin() const noexcept
    {
        return start;
    }
    constexpr const_iterator cbegin() const noexcept
    {
        return start;
    }
    constexpr iterator end() noexcept
    {
        return finish;
    }
    constexpr const_iterator end() const noexcept
    {
        return finish;
    }
    constexpr const_iterator cend() const noexcept
    {
        return finish;
    }
    constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }
    constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    constexpr reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }
    constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    // size and capacity
    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return begin() == end();
    }
    constexpr size_type size() const noexcept
    {
        return size_type(end() - begin());
    }
    constexpr size_type max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }
    constexpr void reserve(size_type new_cap)
    {
        if (new_cap > capacity())
        {
            if (new_cap < 2 * capacity())
            {
                new_cap = 2 * capacity();
            }
            adjust_capacity(new_cap);
        }
    }
    constexpr size_type capacity() const noexcept
    {
        return end_of_storage - start;
    }
    constexpr void shrink_to_fit()
    {
        // do shrink only when capacity > 2*size(), just a choice of implementation, no sepcial meaning
        if (capacity() > 2 * size())
        {
            adjust_capacity(size());
        }
    }

    // modifiers
    constexpr void clear() noexcept
    {
        erase_range(start, finish);
        finish = start;
    }
    constexpr iterator insert(const_iterator pos, const T& value) // 1
    {
        size_type idx = (size_type)(pos - start);
        move_backward(idx, 1);
        *(start + idx) = value;
        return start + idx;
    }
    constexpr iterator insert(const_iterator pos, T&& value) // 2
    {
        size_type idx = (size_type)(pos - start);
        move_backward(idx, 1);
        *(start + idx) = std::move(value);
        return start + idx;
    }
    constexpr iterator insert(const_iterator pos, size_type count, const T& value) // 3
    {
        size_type idx = (size_type)(pos - start);
        move_backward(idx, count);
        fill_range(start + idx, start + idx + count, value);
        return start + idx;
    }
    template<typename InputIterator, 
        typename = std::enable_if_t<std::is_base_of_v<typename std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
    constexpr iterator insert(const_iterator pos, InputIterator first, InputIterator last) // 4
    {
        size_type idx = (size_type)(pos - start);
        move_backward(idx, last - first);
        copy_range(first, last, start + idx);
        return start + idx;
    }
    constexpr iterator insert(const_iterator pos, std::initializer_list<T> il) // 5
    {
        size_type idx = (size_type)(pos - start);
        if (il.size() > 0)
        {
            move_backward(idx, il.size());
            copy_range(il.begin(), il.end(), start + idx);
        }
        return start + idx;
    }
    template<typename... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args)
    {
        size_type idx = (size_type)(pos - start);
        move_backward(idx, 1);
        alloc.construct(start + idx, std::forward<Args>(args)...);
        return start + idx;
    }
    constexpr iterator erase(const_iterator pos) // 1
    {
        move_forward(pos + 1, 1);
        return (iterator)pos;
    }
    constexpr iterator erase(const_iterator first, const_iterator last) // 2
    {
        move_forward(last, last - first);
        return (iterator)first;
    }
    constexpr void push_back(const T& value) // 1
    {
        if (size() == capacity())
        {
            adjust_capacity(); // default ajust to double of size()
        }
        alloc.construct(finish, value);
        ++finish;
    }
    constexpr void push_back(T&& value) // 2
    {
        if (size() == capacity())
        {
            adjust_capacity();
        }
        alloc.construct(finish, std::forward<T>(value));
        ++finish;
    }
    template<typename... Args>
    constexpr reference emplace_back(Args&&... args)
    {
        if (size() == capacity())
        {
            adjust_capacity();
        }
        alloc.construct(finish, std::forward<Args>(args)...);
        ++finish;
        return *finish;
    }
    constexpr void pop_back()
    {
        alloc.destroy(finish - 1);
        --finish;
    }
    constexpr void resize(size_type count) // 1
    {
        if (count == size())
        {
            return;
        }
        else if (count < size())
        {
            erase_range(start + count, finish);
            finish = start + count;
        }
        else if (count <= capacity()) // count > size()
        {
            tstd::uninitialized_fill(finish, start + count, T());
            finish = start + count;
        }
        else // count > capacity()
        {
            adjust_capacity(count);
            tstd::uninitialized_fill(finish, start + count, T());
            finish = start + count;
        }
    }
    constexpr void resize(size_type count, const value_type& value) // 2
    {
        if (count == size())
        {
            return;
        }
        else if (count < size())
        {
            erase_range(start + count, finish);
            finish = start + count;
        }
        else if (count <= capacity()) // count > size()
        {
            tstd::uninitialized_fill(finish, start + count, value);
            finish = start + count;
        }
        else // count > capacity()
        {
            adjust_capacity(count);
            tstd::uninitialized_fill(finish, start + count, value);
            finish = start + count;
        }
    }
    constexpr void swap(vector& other) noexcept // () // todo: contents in noexcept()
    {
        std::swap(alloc, other.alloc);
        std::swap(start, other.start);
        std::swap(finish, other.finish);
        std::swap(end_of_storage, other.end_of_storage);
    }
private:
    // auxiliary functions
    template<typename InputIterator>
    void copy_range(InputIterator first, InputIterator last, iterator dest)
    {
        for (; first != last; ++first, ++dest)
        {
            *dest = *first;
        }
    }
    // move from front to back
    void move_range(const_iterator first, const_iterator last, iterator dest)
    {
        for (; first != last; ++first, ++dest)
        {
            *dest = std::move(*first);
        }
    }
    // move from back to front
    void move_range_from_back_to_front(const_iterator first, const_iterator last, iterator dest)
    {
        auto count = last - first;
        auto rfirst = make_reverse_iterator(first);
        auto iter = make_reverse_iterator(last);
        for (auto rdest = make_reverse_iterator(dest + count); iter != rfirst; ++iter, ++rdest)
        {
            *rdest = std::move(*iter);
        }
    }
    void fill_range(iterator first, iterator last, const T& value)
    {
        for (auto dest = first; dest != last; ++dest)
        {
            *dest = value;
        }
    }
    // destroy a range of elements in reverse order.
    void erase_range(iterator first, iterator last)
    {
        auto rfirst = make_reverse_iterator(first);
        for (auto iter = make_reverse_iterator(last); iter != rfirst; ++iter)
        {
            alloc.destroy(&*iter);
        }
    }
    // free all spaces for another allocation
    void free_all_spaces()
    {
        if (start)
        {
            alloc.deallocate(start, sizeof(T) * capacity());
        }
        start = finish = end_of_storage = nullptr;
    }
    // adjust capacity: less or more, reserve, shrink_to_fit, etc
    // new_cap must greater than or equal to size(), default adjust to double of current size()
    void adjust_capacity(size_type new_cap = 0)
    {
        if (new_cap <= size())
        {
            new_cap = 2 * size();
            new_cap = new_cap > 0 ? new_cap : 1; // make sure at least for 1 elements
        }
        T* new_start = alloc.allocate(new_cap * sizeof(T));
        T* new_finish = new_start + size();
        T* new_end_of_storage = new_start + new_cap;
        uninitialized_move(start, finish, new_start);
        clear();
        free_all_spaces();
        start = new_start;
        finish = new_finish;
        end_of_storage = new_end_of_storage;
    }
    // move elements after(include) idx backward specific location.
    // do the copy(move if possible), and ajust capacity if necessary
    // for insert
    void move_backward(size_type idx, size_type count)
    {
        while (size() + count > capacity())
        {
            adjust_capacity(std::max(2*size(), size() + count)); // todo: replaced with tstd::max
        }
        if (idx + count >= size())
        {
            uninitialized_copy(start + idx, finish, start + idx + count);
        }
        else // idx + count < size()
        {
            uninitialized_copy(finish - count, finish, finish);
            // move elements from back to front
            move_range_from_back_to_front(start + idx, finish - count, start + idx + count);
        }
        finish += count;
    }
    // move elements forward
    // called after erasing
    void move_forward(const_iterator first, size_type count)
    {
        move_range(first, finish, (iterator)(first - count));
        erase_range(finish - count, finish);
        finish -= count;
    }
private:
    Allocator alloc;
    T* start;
    T* finish; // off the end
    T* end_of_storage; // off the capacity
};

// non-member operations
// a non-standard compare function for tstd::vector
// equal 0 less -1 greater 1
template<typename T, typename Allocator>
constexpr int _cmp_vector(const tstd::vector<T, Allocator>& lhs, const tstd::vector<T, Allocator>& rhs)
{
    if (lhs.size() == 0 || rhs.size() == 0)
    {
        return lhs.size() < rhs.size() ? -1 : (lhs.size() == rhs.size() ? 0 : 1);
    }
    auto it1 = lhs.begin();
    auto it2 = rhs.begin();
    for (; it1 != lhs.end() && it2 != rhs.end(); ++it1, ++it2)
    {
        if (*it1 == *it2)
        {
            continue;
        }
        return *it1 > *it2 ? 1 : -1;
    }
    if (it1 == lhs.end())
    {
        return it2 == rhs.end() ? 0 : -1;
    }
    return 1; // it1 != lhs.end() && it2 == rhs.end()
}

// comparasions
template<typename T, typename Allocator>
constexpr bool operator==(const tstd::vector<T, Allocator>& lhs, const tstd::vector<T, Allocator>& rhs)
{
    return _cmp_vector(lhs, rhs) == 0;
}
template<typename T, typename Allocator>
constexpr bool operator!=(const tstd::vector<T, Allocator>& lhs, const tstd::vector<T, Allocator>& rhs)
{
    return _cmp_vector(lhs, rhs) != 0;
}
template<typename T, typename Allocator>
constexpr bool operator<(const tstd::vector<T, Allocator>& lhs, const tstd::vector<T, Allocator>& rhs)
{
    return _cmp_vector(lhs, rhs) < 0;
}
template<typename T, typename Allocator>
constexpr bool operator<=(const tstd::vector<T, Allocator>& lhs, const tstd::vector<T, Allocator>& rhs)
{
    return _cmp_vector(lhs, rhs) <= 0;
}
template<typename T, typename Allocator>
constexpr bool operator>(const tstd::vector<T, Allocator>& lhs, const tstd::vector<T, Allocator>& rhs)
{
    return _cmp_vector(lhs, rhs) > 0;
}
template<typename T, typename Allocator>
constexpr bool operator>=(const tstd::vector<T, Allocator>& lhs, const tstd::vector<T, Allocator>& rhs)
{
    return _cmp_vector(lhs, rhs) >= 0;
}

// global swap for tstd::vector
template<typename T, typename Allocator>
constexpr void swap(tstd::vector<T, Allocator>& lhs, tstd::vector<T, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace tstd


#endif // TVECTOR_HPP