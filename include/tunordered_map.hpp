#ifndef TUNORDERED_MAP_HPP
#define TUNORDERED_MAP_HPP

#include <functional>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <tuple>
#include <tstl_allocator.hpp>
#include <tstl_hashtable.hpp>
#include <tutility.hpp>
#include <titerator.hpp>
#include <tunordered_multimap.hpp>

namespace tstd
{

template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>, typename Allocator = tstd::allocator<std::pair<const Key, T>>,
    typename UnderlyingHashTable = tstd::impl::hash_table<const Key, std::pair<const Key, T>, tstd::impl::first_of_pair<const Key, T>, false, Hash, KeyEqual, Allocator>>
class unordered_map;

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator, typename UnderlyingHashTable>
bool operator==(const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs);
template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator, typename UnderlyingHashTable>
bool operator!=(const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs);

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator, typename UnderlyingHashTable>
class unordered_map
{
    friend bool operator== <Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>(const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs);
    friend bool operator!= <Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>(const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs);
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = Allocator::pointer;
    using const_pointer = Allocator::const_pointer;
    using iterator = typename UnderlyingHashTable::iterator;
    using const_iterator = typename UnderlyingHashTable::const_iterator;
    using local_iterator = typename UnderlyingHashTable::local_iterator;
    using const_local_iterator = typename UnderlyingHashTable::const_local_iterator;
    // node_type and insert_return_type are not supported
private:
    inline constexpr static size_type initial_bucket_size = 16;
    UnderlyingHashTable table;
public:
    // constructors
    unordered_map() // 1
        : unordered_map(initial_bucket_size)
    {
    }
    explicit unordered_map(size_type bucket_count, const Hash& hash = Hash(), const KeyEqual& equal = KeyEqual(), const Allocator& alloc = Allocator()) // 2
        : table(bucket_count, hash, equal, alloc)
    {
    }
    unordered_map(size_type bucket_count, const Allocator& alloc) // 3
        : unordered_map(bucket_count, Hash(), KeyEqual(), alloc)
    {
    }
    unordered_map(size_type bucket_count, const Hash& hash, const Allocator& alloc) // 4
        : unordered_map(bucket_count, hash, KeyEqual(), alloc)
    {
    }
    explicit unordered_map(const Allocator& alloc) // 5
        : unordered_map(initial_bucket_size, Hash(), KeyEqual(), alloc)
    {
    }
    template<typename InputIterator>
    unordered_map(InputIterator first, InputIterator last, size_type bucket_count = initial_bucket_size,
        const Hash& hash = Hash(), const KeyEqual& equal = KeyEqual(), const Allocator& alloc = Allocator()) // 6
        : table(first, last, bucket_count, hash, equal, alloc)
    {
    }
    template<typename InputIterator>
    unordered_map(InputIterator first, InputIterator last, size_type bucket_count, const Allocator& alloc) // 7
        : unordered_map(first, last, bucket_count, Hash(), KeyEqual(), alloc)
    {
    }
    template<typename InputIterator>
    unordered_map(InputIterator first, InputIterator last, size_type bucket_count, const Hash& hash, const Allocator& alloc) // 8
        : unordered_map(first, last, bucket_count, hash, KeyEqual(), alloc)
    {
    }
    unordered_map(const unordered_map& other) // 9
        : table(other.table, other.get_allocator())
    {
    }
    unordered_map(const unordered_map& other, const Allocator& alloc) // 10
        : table(other.table, alloc)
    {
    }
    unordered_map(unordered_map&& other) // 11
        : table(std::move(other.table))
    {
    }
    unordered_map(unordered_map&& other, const Allocator& alloc) // 12
        : table(std::move(other.table), alloc)
    {
    }
    unordered_map(std::initializer_list<value_type> il, size_type bucket_count = initial_bucket_size,
        const Hash& hash = Hash(), const KeyEqual& equal = KeyEqual(), const Allocator& alloc = Allocator()) // 13
        : unordered_map(il.begin(), il.end(), bucket_count, hash, equal, alloc)
    {
    }
    unordered_map(std::initializer_list<value_type> il, size_type bucket_count, const  Allocator& alloc) // 14
        : unordered_map(il, bucket_count, Hash(), KeyEqual(), alloc)
    {
    }
    unordered_map(std::initializer_list<value_type> il, size_type bucket_count, const Hash& hash, const Allocator& alloc) // 15
        : unordered_map(il, bucket_count, hash, KeyEqual(), alloc)
    {
    }
    ~unordered_map()
    {
    }
    // assignment
    unordered_map& operator=(const unordered_map& other) // 1
    {
        table = other.table;
        return *this;
    }
    unordered_map& operator=(unordered_map&& other) // 2
        noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_move_assignable_v<Hash> && std::is_nothrow_move_assignable_v<KeyEqual>)
    {
        table = std::move(other.table);
        return *this;
    }
    unordered_map& operator=(std::initializer_list<value_type> il) // 3
    {
        table = il;
        return *this;
    }
    // allocator
    allocator_type get_allocator() const noexcept
    {
        return table.get_allocator();
    }
    // iterators
    iterator begin() noexcept
    {
        return table.begin();
    }
    const_iterator begin() const noexcept
    {
        return table.begin();
    }
    const_iterator cbegin() const noexcept
    {
        return table.cbegin();
    }
    iterator end() noexcept
    {
        return table.end();
    }
    const_iterator end() const noexcept
    {
        return table.end();
    }
    const_iterator cend() const noexcept
    {
        return table.cend();
    }
    // size and capacity
    [[nodiscard]] bool empty() const noexcept
    {
        return table.empty();
    }
    size_type size() const noexcept
    {
        return table.size();
    }
    size_type max_size() const noexcept
    {
        return table.max_size();
    }
    // modifiers
    void clear() noexcept
    {
        table.clear();
    }
    // insert
    std::pair<iterator, bool> insert(const value_type& value) // 1
    {
        return table.insert(value);
    }
    std::pair<iterator, bool> insert(value_type&& value) // 2
    {
        return table.insert(std::move(value));
    }
    template<typename P,
        typename = std::enable_if_t<std::is_constructible_v<value_type, P&&>>>
    std::pair<iterator, bool> insert(P&& value) // 3
    {
        return table.emplace(std::forward<P>(value));
    }
    iterator insert(const_iterator hint, const value_type& value) // 4
    {
        return table.insert(value).first;
    }
    iterator insert(const_iterator hint, value_type&& value) // 5
    {
        return table.insert(std::move(value)).first;
    }
    template<typename P,
        typename = std::enable_if_t<std::is_constructible_v<value_type, P&&>>>
    iterator insert(const_iterator hint, P&& value) // 6
    {
        return table.emplace(std::forward<P>(value)).first;
    }
    template<typename InputIterator>
    void insert(InputIterator first, InputIterator last) // 7
    {
        for (; first != last; ++first)
        {
            table.insert(*first);
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
        return table.emplace(std::forward<Args>(args)...);
    }
    // emplace_hint
    template<typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args)
    {
        return table.emplace(std::forward<Args>(args)...).first;
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
            return table.emplace(std::piecewise_construct, std::forward_as_tuple(k), std::forward_as_tuple(std::forward<Args>(args)...));
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
            return table.emplace(std::piecewise_construct, std::forward_as_tuple(k), std::forward_as_tuple(std::forward<Args>(args)...));
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
        return table.erase(pos);
    }
    iterator erase(const_iterator first, const_iterator last) // 2
    {
        return table.erase(first, last);
    }
    size_type erase(const key_type& key) // 3
    {
        return table.erase(key);
    }
    // since C++23
    template<typename K,
        typename = std::enable_if_t<!std::is_convertible_v<K, iterator> && !std::is_convertible_v<K, const_iterator>>>
    size_type erase(K&& k) // 4
    {
        return table.erase(Key(std::forward<K>(k)));
    }
    // swap
    void swap(unordered_map& other)
        noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable_v<Hash> && std::is_nothrow_swappable_v<KeyEqual>)
    {
        table.swap(other.table);
    }
    // extract and merge: do not support

    // lookup
    // at
    T& at(const Key& key) // 1
    {
        auto iter = find(key);
        if (iter == end())
        {
            throw std::out_of_range("unordered_map::at: do not have this key");
        }
        return iter->second;
    }
    const T& at(const Key& key) const // 2
    {
        auto iter = find(key);
        if (iter == end())
        {
            throw std::out_of_range("unordered_map::at: do not have this key");
        }
        return iter->second;
    }
    // operator[]
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
    // count
    size_type count(const Key& key) const // 1
    {
        return table.find(key) != table.end() ? 1 : 0;
    }
    template<typename K>
    size_type count(const K& x) const // 2
    {
        return table.find(Key(x)) != table.end() ? 1 : 0;
    }
    // find
    iterator find(const Key& key) // 1
    {
        return table.find(key);
    }
    const_iterator find(const Key& key) const // 2
    {
        return table.find(key);
    }
    template<typename K>
    iterator find(const K& x) // 3
    {
        return table.find(Key(x));
    }
    template<typename K>
    const_iterator find(const K& x) const // 4
    {
        return table.find(Key(x));
    }
    // contains
    bool contains(const Key& key) const // 1
    {
        return table.find(key) != table.end();
    }
    template<typename K>
    bool contains(const K& x) const // 2
    {
        return table.find(Key(x)) != table.end();
    }
    // equal_range
    std::pair<iterator, iterator> equal_range(const Key& key) // 1
    {
        return table.equal_range(key);
    }
    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const // 2
    {
        return table.equal_range(key);
    }
    template<typename K>
    std::pair<iterator, iterator> equal_range(const K& x) // 3
    {
        return table.equal_range(Key(x));
    }
    template<typename K>
    std::pair<const_iterator, const_iterator> equal_range(const K& x) const // 4
    {
        return table.equal_range(Key(x));
    }
    
    // bucket interfaces
    // local iterators
    local_iterator begin(size_type n)
    {
        return table.begin(n);
    }
    const_local_iterator begin(size_type n) const
    {
        return table.begin(n);
    }
    const_local_iterator cbegin(size_type n) const
    {
        return table.cbegin(n);
    }
    local_iterator end(size_type n)
    {
        return table.end(n);
    }
    const_local_iterator end(size_type n) const
    {
        return table.end(n);
    }
    const_local_iterator cend(size_type n) const
    {
        return table.cend(n);
    }
    // bucket_count
    size_type bucket_count() const
    {
        return table.bucket_count();
    }
    // max_bucket_count
    size_type max_bucket_count() const
    {
        return table.max_bucket_count();
    }
    // bucket_size
    size_type bucket_size(size_type n) const
    {
        return table.bucket_size(n);
    }
    // bucket
    size_type bucket(const Key& key) const
    {
        return table.bucket(key);
    }
    
    // hash policy
    float load_factor() const
    {
        return table.load_factor();
    }
    // get max load factor
    float max_load_factor() const
    {
        return table.max_load_factor();
    }
    // unordered_map max load factor
    void max_load_factor(float ml)
    {
        return table.max_load_factor(ml);
    }
    void rehash(size_type count)
    {
        return table.rehash(count);
    }
    void reserve(size_type count)
    {
        return table.reserve(count);
    }

    // observers
    hasher hash_function() const
    {
        return table.hash_function();
    }
    key_equal key_eq() const
    {
        return table.key_eq();
    }
};

// non-member operations
// comparisons
template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator, typename UnderlyingHashTable>
bool operator==(const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs)
{
    return lhs.table == rhs.table;
}
template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator, typename UnderlyingHashTable>
bool operator!=(const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs)
{
    return lhs.table != rhs.table;
}

// global swap for tstd::unordered_map
template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator, typename UnderlyingHashTable>
void swap(tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, tstd::unordered_map<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace tstd


#endif // TUNORDERED_MAP_HPP