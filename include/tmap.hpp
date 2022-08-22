#ifndef TMAP_HPP
#define TMAP_HPP

#include <functional>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <limits>
#include <utility>
#include <tuple>
#include <stdexcept>
#include <tstl_allocator.hpp>
#include <titerator.hpp>
#include <tstl_bst.hpp>
#include <tutility.hpp>
#include <tmultimap.hpp> // multimap is in <map>

namespace tstd
{

template<typename Key, typename T, typename Compare = std::less<Key>, typename Allocator = tstd::allocator<std::pair<const Key, T>>,
    typename UnderlyingTree = tstd::impl::bst<const Key, std::pair<const Key, T>, tstd::impl::first_of_pair<const Key, T>, false, Compare, Allocator>>
class map;

template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator==(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator!=(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator<(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator<=(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator>(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator>=(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);

template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
class map
{
    friend bool operator== <Key, T, Compare, Allocator, UnderlyingTree>(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);
    friend bool operator!= <Key, T, Compare, Allocator, UnderlyingTree>(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);
    friend bool operator<  <Key, T, Compare, Allocator, UnderlyingTree>(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);
    friend bool operator<= <Key, T, Compare, Allocator, UnderlyingTree>(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);
    friend bool operator>  <Key, T, Compare, Allocator, UnderlyingTree>(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);
    friend bool operator>= <Key, T, Compare, Allocator, UnderlyingTree>(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs);
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using didfference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = Allocator::pointer;
    using const_pointer = Allocator::const_pointer;
    using iterator = typename UnderlyingTree::iterator;
    using const_iterator = typename UnderlyingTree::const_iterator;
    using reverse_iterator = typename UnderlyingTree::reverse_iterator;
    using const_reverse_iterator = typename UnderlyingTree::const_reverse_iterator;
    class value_compare
    {
        friend class map<Key, T, Compare, Allocator, UnderlyingTree>;
    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) {}
    public:
        bool operator()(const value_type& lhs, const value_type& rhs)
        {
            return comp(lhs.first, rhs.first);
        }
    };
    // node_type and insert_return_type(since C++17) are not supported
private:
    UnderlyingTree tree;
public:
    map() // 1
        : tree()
    {
    }
    explicit map(const Compare& comp, const Allocator& _alloc = Allocator()) // 2
        : tree(comp, _alloc)
    {
    }
    explicit map(const Allocator& _alloc) // 3
        : map(Compare(), _alloc)
    {
    }
    template<typename InputIterator>
    map(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Allocator& _alloc = Allocator()) // 4
        : tree(comp, _alloc)
    {
        for (; first != last; ++first)
        {
            tree.insert(*first);
        }
    }
    template<typename InputIterator>
    map(InputIterator first, InputIterator last, const Allocator _alloc) // 5
        : map(first, last, Compare(), _alloc)
    {
    }
    map(const map& other) // 6
        : tree(other.tree)
    {
    }
    map(const map& other, const Allocator& _alloc) // 7
        : tree(other.tree, _alloc)
    {
    }
    map(map&& other) // 8
        : tree(std::move(other.tree))
    {
    }
    map(map&& other, const Allocator& _alloc) // 9
        : tree(std::move(other.tree), _alloc)
    {
    }
    map(std::initializer_list<value_type> il, const Compare& comp = Compare(), const Allocator& _alloc = Allocator()) // 10
        : map(il.begin(), il.end(), comp, _alloc)
    {
    }
    map(std::initializer_list<value_type> il, const Allocator& _alloc) // 11
        : map(il, Compare(), _alloc)
    {
    }
    ~map()
    {
    }
    // assignment
    map& operator=(const map& other) // 1
    {
        tree = other.tree;
        return *this;
    }
    map& operator=(map&& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_move_assignable<Compare>::value) // 2
    {
        tree = std::move(other.tree);
        return *this;
    }
    map& operator=(std::initializer_list<value_type> il) // 3
    {
        tree.clear();
        for (auto& elem : il)
        {
            tree.insert(elem);
        }
        return *this;
    }
    // allocator
    allocator_type get_allocator() const noexcept
    {
        return tree.get_allocator();
    }
    // element access
    T& at(const Key& key) // 1
    {
        auto iter = find(key);
        if (iter == end())
        {
            throw std::out_of_range("map::at do not have this key");
        }
        return iter->second;
    }
    const T& at(const Key& key) const // 2
    {
        auto iter = find(key);
        if (iter == end())
        {
            throw std::out_of_range("map::at do not have this key");
        }
        return iter->second;
    }
    T& operator[](const Key& key) // 1
    {
        auto iter = find(key);
        if (iter == end())
        {
            auto p = insert(std::make_pair(key, T()));
            return p.first->second;
        }
        return iter->second;
    }
    T& operator[](Key&& key) // 2
    {
        auto iter = find(key);
        if (iter == end())
        {
            auto p = insert(std::make_pair(std::move(key), T()));
            return p.first->second;
        }
        return iter->second;
    }
    // iterators
    iterator begin() noexcept
    {
        return tree.begin();
    }
    const_iterator begin() const noexcept
    {
        return tree.begin();
    }
    const_iterator cbegin() const noexcept
    {
        return tree.cbegin();
    }
    iterator end() noexcept
    {
        return tree.end();
    }
    const_iterator end() const noexcept
    {
        return tree.end();
    }
    const_iterator cend() const noexcept
    {
        return tree.cend();
    }
    reverse_iterator rbegin() noexcept
    {
        return tree.rbegin();
    }
    const_reverse_iterator rbegin() const noexcept
    {
        return tree.rbegin();
    }
    const_reverse_iterator crbegin() const noexcept
    {
        return tree.crbegin();
    }
    reverse_iterator rend() noexcept
    {
        return tree.rend();
    }
    const_reverse_iterator rend() const noexcept
    {
        return tree.rend();
    }
    const_reverse_iterator crend() const noexcept
    {
        return tree.crend();
    }
    // size and capacity
    [[nodiscard]] bool empty() const noexcept
    {
        return tree.empty();
    }
    size_type size() const noexcept
    {
        return tree.size();
    }
    size_type max_size() const noexcept
    {
        return tree.max_size();
    }
    // modifiers
    void clear() noexcept
    {
        tree.clear();
    }
    // insert
    std::pair<iterator, bool> insert(const value_type& value) // 1
    {
        return tree.insert(value);
    }
    template<typename P,
        typename = std::enable_if_t<std::is_constructible_v<value_type, P&&>>>
    std::pair<iterator, bool> insert(P&& value) // 2
    {
        return tree.emplace(std::forward<P>(value));
    }
    std::pair<iterator, bool> insert(value_type&& value) // 3
    {
        return tree.insert(std::move(value));
    }
    iterator insert(const_iterator hint, const value_type& value) // 4
    {
        return tree.insert(value).first;
    }
    template<typename P,
        typename = std::enable_if_t<std::is_constructible_v<value_type, P&&>>>
    iterator insert(const_iterator hint, P&& value) // 5
    {
        return tree.emplace(std::forward<P>(value)).first;
    }
    iterator insert(const_iterator hint, value_type&& value) // 6
    {
        return tree.insert(std::move(value)).first;
    }
    template<typename InputIterator>
    void insert(InputIterator first, InputIterator last) // 7
    {
        for (; first != last; ++first)
        {
            tree.insert(*first);
        }
    }
    void insert(std::initializer_list<value_type> il) // 8
    {
        insert(il.begin(), il.end());
    }
    // insert_return_type insert(node_type&& nh) // 9
    // {
    // }
    // iterator insert(const_iterator hint, node_type&& nh) // 10
    // {
    // }
    
    // insert_or_assign
    template<typename M,
        typename = std::enable_if_t<std::is_assignable_v<mapped_type&, M&&>>>
    std::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj) // 1
    {
        auto iter = find(k);
        // insert
        if (iter == end())
        {
            return insert(value_type(k, std::forward<M>(obj)));
        }
        // assign
        iter->second = std::forward<M>(obj);
        return {iter, false};
    }
    template<typename M,
        typename = std::enable_if_t<std::is_assignable_v<mapped_type&, M&&>>>
    std::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj) // 2
    {
        auto iter = find(k);
        // insert
        if (iter == end())
        {
            return insert(value_type(std::move(k), std::forward<M>(obj)));
        }
        // assign
        iter->second = std::forward<M>(obj);
        return {iter, false};
    }
    template<typename M,
        typename = std::enable_if_t<std::is_assignable_v<mapped_type&, M&&>>>
    iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj) // 3
    {
        return insert_or_assign(k, std::forward<M>(obj)).first;
    }
    template<typename M,
        typename = std::enable_if_t<std::is_assignable_v<mapped_type&, M&&>>>
    iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj) // 4
    {
        return insert_or_assign(std::move(k), std::forward<M>(obj)).first;
    }

    // emplace
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        return tree.emplace(std::forward<Args>(args)...);
    }
    // emplace_hint
    template<typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args)
    {
        return tree.emplace(std::forward<Args>(args)...).first;
    }
    // try_emplace
    // if key exist, do nothing
    // else do insert
    template<typename... Args>
    std::pair<iterator, bool> try_emplace(const key_type& k, Args&&... args) // 1
    {
        auto iter = find(k);
        if (iter == end())
        {
            return tree.emplace(std::piecewise_construct, std::forward_as_tuple(k), std::forward_as_tuple(std::forward<Args>(args)...));
        }
        return {iter, false};
    }
    template<typename... Args>
    std::pair<iterator, bool> try_emplace(key_type&& k, Args&&... args) // 2
    {
        auto iter = find(k);
        if (iter == end())
        {
            // In principle, the second argument should be std::forward_as_tuple(std::move(k)), but can not compile with key_type = int, why?
            return tree.emplace(std::piecewise_construct, std::forward_as_tuple(k), std::forward_as_tuple(std::forward<Args>(args)...));
        }
        return {iter, false};
    }
    template<typename... Args>
    iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args) // 3
    {
        return try_emplace(k, std::forward<Args>(args)...).first;
    }
    template<typename... Args>
    iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args) // 4
    {
        return try_emplace(std::move(k), std::forward<Args>(args)...).first;
    }
    // erase
    iterator erase(const_iterator pos) // 1
    {
        return tree.erase(pos);
    }
    iterator erase(const_iterator first, const_iterator last) // 2
    {
        return tree.erase(first, last);
    }
    size_type erase(const key_type& key) // 3
    {
        return tree.erase(key);
    }
    template<typename K,
        typename = std::enable_if_t<!std::is_convertible_v<K, iterator> && !std::is_convertible_v<K, const_iterator>>>
    size_type erase(K&& k) // 4
    {
        return tree.erase(Key(std::forward<K>(k)));
    }
    // swap
    void swap(map& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable_v<Compare>)
    {
        tree.swap(other.tree);
    }
    // extract and merge: do not support

    // lookup
    // count
    size_type count(const Key& key) const // 1
    {
        return tree.find(key) != tree.end() ? 1 : 0;
    }
    template<typename K>
    size_type count(const K& x) const // 2
    {
        return tree.find(Key(x)) != tree.end() ? 1 : 0;
    }
    // find
    iterator find(const Key& key) // 1
    {
        return tree.find(key);
    }
    const_iterator find(const Key& key) const // 2
    {
        return tree.find(key);
    }
    template<typename K>
    iterator find(const K& x) // 3
    {
        return tree.find(Key(x));
    }
    template<typename K>
    const_iterator find(const K& x) const // 4
    {
        return tree.find(Key(x));
    }
    // contains
    bool contains(const Key& key) const // 1
    {
        return tree.find(key) != tree.end();
    }
    template<typename K>
    bool contains(const K& x) const // 2
    {
        return tree.find(Key(x)) != tree.end();
    }
    // equal_range
    std::pair<iterator, iterator> equal_range(const Key& key) // 1
    {
        return {tree.lower_bound(key), tree.upper_bound(key)};
    }
    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const // 2
    {
        return {tree.lower_bound(key), tree.upper_bound(key)};
    }
    template<typename K>
    std::pair<iterator, iterator> equal_range(const K& x) // 3
    {
        return {tree.lower_bound(Key(x)), tree.upper_bound(Key(x))};
    }
    template<typename K>
    std::pair<const_iterator, const_iterator> equal_range(const K& x) const // 4
    {
        return {tree.lower_bound(Key(x)), tree.upper_bound(Key(x))};
    }
    // lower_bound
    iterator lower_bound(const Key& key) // 1
    {
        return tree.lower_bound(key);
    }
    const_iterator lower_bound(const Key& key) const // 2
    {
        return tree.lower_bound(key);
    }
    template<typename K>
    iterator lower_bound(const K& x) // 3
    {
        return tree.lower_bound(Key(x));
    }
    template<typename K>
    const_iterator lower_bound(const K& x) const // 4
    {
        return tree.lower_bound(Key(x));
    }
    // upper_bound
    iterator upper_bound(const Key& key) // 1
    {
        return tree.upper_bound(key);
    }
    const_iterator upper_bound(const Key& key) const // 2
    {
        return tree.upper_bound(key);
    }
    template<typename K>
    iterator upper_bound(const K& x) // 3
    {
        return tree.upper_bound(Key(x));
    }
    template<typename K>
    const_iterator upper_bound(const K& x) const // 4
    {
        return tree.upper_bound(Key(x));
    }
    // observers
    key_compare key_comp() const
    {
        return tree.key_comp();
    }
    value_compare value_comp() const
    {
        return value_compare(key_comp());
    }
};

// non-member operations
// comparisons
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator==(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree == rhs.tree;
}
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator!=(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree != rhs.tree;
}
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator<(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree < rhs.tree;
}
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator<=(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree <= rhs.tree;
}
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator>(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree > rhs.tree;
}
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator>=(const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, const tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree >= rhs.tree;
}

// global swap for tstd::map
template<typename Key, typename T, typename Compare, typename Allocator, typename UnderlyingTree>
void swap(tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& lhs, tstd::map<Key, T, Compare, Allocator, UnderlyingTree>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace tstd


#endif // TMAP_HPP