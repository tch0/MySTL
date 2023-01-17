#ifndef TSET_HPP
#define TSET_HPP

#include <functional>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <limits>
#include <utility>
#include <tstl_allocator.hpp>
#include <titerator.hpp>
#include <tstl_bst.hpp>
#include <tstl_rbtree.hpp>
#include <tutility.hpp>
#include <tmultiset.hpp> // multiset is in <set>

namespace tstd
{

template<typename Key, typename Compare = std::less<Key>, typename Allocator = tstd::allocator<Key>,
    typename UnderlyingTree = tstd::impl::rb_tree<Key, Key, tstd::impl::identity, false, Compare, Allocator>>
class set;

template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator==(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator!=(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator<(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator<=(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator>(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator>=(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);

template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
class set
{
    friend bool operator== <Key, Compare, Allocator, UnderlyingTree>(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);
    friend bool operator!= <Key, Compare, Allocator, UnderlyingTree>(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);
    friend bool operator<  <Key, Compare, Allocator, UnderlyingTree>(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);
    friend bool operator<= <Key, Compare, Allocator, UnderlyingTree>(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);
    friend bool operator>  <Key, Compare, Allocator, UnderlyingTree>(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);
    friend bool operator>= <Key, Compare, Allocator, UnderlyingTree>(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs);
public:
    using key_type = Key;
    using value_type = Key;
    using size_type = std::size_t;
    using didfference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using value_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = Allocator::pointer;
    using const_pointer = Allocator::const_pointer;
    using iterator = typename UnderlyingTree::iterator;
    using const_iterator = typename UnderlyingTree::const_iterator;
    using reverse_iterator = typename UnderlyingTree::reverse_iterator;
    using const_reverse_iterator = typename UnderlyingTree::const_reverse_iterator;
    // node_type and insert_return_type(since C++17) are not supported
private:
    UnderlyingTree tree;
public:
    set() // 1
        : tree()
    {
    }
    explicit set(const Compare& comp, const Allocator& _alloc = Allocator()) // 2
        : tree(comp, _alloc)
    {
    }
    explicit set(const Allocator& _alloc) // 3
        : set(Compare(), _alloc)
    {

    }
    template<typename InputIterator>
    set(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Allocator& _alloc = Allocator()) // 4
        : tree(comp, _alloc)
    {
        for (; first != last; ++first)
        {
            tree.insert(*first);
        }
    }
    template<typename InputIterator>
    set(InputIterator first, InputIterator last, const Allocator _alloc) // 5
        : set(first, last, Compare(), _alloc)
    {
    }
    set(const set& other) // 6
        : tree(other.tree)
    {
    }
    set(const set& other, const Allocator& _alloc) // 7
        : tree(other.tree, _alloc)
    {
    }
    set(set&& other) // 8
        : tree(std::move(other.tree))
    {
    }
    set(set&& other, const Allocator& _alloc) // 9
        : tree(std::move(other.tree), _alloc)
    {
    }
    set(std::initializer_list<value_type> il, const Compare& comp = Compare(), const Allocator& _alloc = Allocator()) // 10
        : set(il.begin(), il.end(), comp, _alloc)
    {
    }
    set(std::initializer_list<value_type> il, const Allocator& _alloc) // 11
        : set(il, Compare(), _alloc)
    {
    }
    ~set()
    {
    }
    // assignment
    set& operator=(const set& other) // 1
    {
        tree = other.tree;
        return *this;
    }
    set& operator=(set&& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_move_assignable<Compare>::value) // 2
    {
        tree = std::move(other.tree);
        return *this;
    }
    set& operator=(std::initializer_list<value_type> il) // 3
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
    std::pair<iterator, bool> insert(value_type&& value) // 2
    {
        return tree.insert(std::move(value));
    }
    iterator insert(const_iterator hint, const value_type& value) // 3
    {
        return tree.insert(value).first;
    }
    iterator insert(const_iterator hint, value_type&& value) // 4
    {
        return tree.insert(std::move(value)).first;
    }
    template<typename InputIterator>
    void insert(InputIterator first, InputIterator last) // 5
    {
        for (; first != last; ++first)
        {
            tree.insert(*first);
        }
    }
    void insert(std::initializer_list<value_type> il) // 6
    {
        insert(il.begin(), il.end());
    }
    // insert_return_type insert(node_type&& nh) // 7
    // {
    // }
    // iterator insert(const_iterator hint, node_type&& nh) // 8
    // {
    // }

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
    // participate in overload resolution only when K can not convert to iterator or const_iteraotr implicitly
    // since C++23
    template<typename K,
        typename = std::enable_if_t<!std::is_convertible_v<K, iterator> && !std::is_convertible_v<K, const_iterator>>>
    size_type erase(K&& k) // 4
    {
        return tree.erase(Key(std::forward<K>(k)));
    }
    // swap
    void swap(set& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable_v<Compare>)
    {
        tree.swap(other.tree);
    }
    // extract and merge: do not support

    // lookup
    // count
    size_type count(const Key& key) const // 1
    {
        return tree.find(key) != tree.end() ? 1 : 0;
        // another option: tstd::distance(tree.lower_bound(key), tree.upper_bound(key));
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
        return tree.value_comp();
    }
};

// non-member operations
// comparisons
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator==(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree == rhs.tree;
}
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator!=(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree != rhs.tree;
}
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator<(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree < rhs.tree;
}
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator<=(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree <= rhs.tree;
}
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator>(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree > rhs.tree;
}
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
bool operator>=(const tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, const tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs)
{
    return lhs.tree >= rhs.tree;
}

// global swap for tstd::set
template<typename Key, typename Compare, typename Allocator, typename UnderlyingTree>
void swap(tstd::set<Key, Compare, Allocator, UnderlyingTree>& lhs, tstd::set<Key, Compare, Allocator, UnderlyingTree>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace tstd

#endif // TSET_HPP