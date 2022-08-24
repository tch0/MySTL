#ifndef TUNORDERED_MULTIMAP_HPP
#define TUNORDERED_MULTIMAP_HPP

#include <functional>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <tstl_allocator.hpp>
#include <tstl_hashtable.hpp>
#include <tutility.hpp>
#include <titerator.hpp>

namespace tstd
{

template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>, typename Allocator = tstd::allocator<std::pair<const Key, T>>,
    typename UnderlyingHashTable = tstd::impl::hash_table<const Key, std::pair<const Key, T>, tstd::impl::first_of_pair<const Key, T>, true, Hash, KeyEqual, Allocator>>
class unordered_multimap;

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator, typename UnderlyingHashTable>
bool operator==(const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs);
template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator, typename UnderlyingHashTable>
bool operator!=(const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs);

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator, typename UnderlyingHashTable>
class unordered_multimap
{
    friend bool operator== <Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>(const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs);
    friend bool operator!= <Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>(const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs);
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
    // node_type is not supported
private:
    inline constexpr static size_type initial_bucket_size = 16;
    UnderlyingHashTable table;
public:
    // constructors
    unordered_multimap() // 1
        : unordered_multimap(initial_bucket_size)
    {
    }
    explicit unordered_multimap(size_type bucket_count, const Hash& hash = Hash(), const KeyEqual& equal = KeyEqual(), const Allocator& alloc = Allocator()) // 2
        : table(bucket_count, hash, equal, alloc)
    {
    }
    unordered_multimap(size_type bucket_count, const Allocator& alloc) // 3
        : unordered_multimap(bucket_count, Hash(), KeyEqual(), alloc)
    {
    }
    unordered_multimap(size_type bucket_count, const Hash& hash, const Allocator& alloc) // 4
        : unordered_multimap(bucket_count, hash, KeyEqual(), alloc)
    {
    }
    explicit unordered_multimap(const Allocator& alloc) // 5
        : unordered_multimap(initial_bucket_size, Hash(), KeyEqual(), alloc)
    {
    }
    template<typename InputIterator>
    unordered_multimap(InputIterator first, InputIterator last, size_type bucket_count = initial_bucket_size,
        const Hash& hash = Hash(), const KeyEqual& equal = KeyEqual(), const Allocator& alloc = Allocator()) // 6
        : table(first, last, bucket_count, hash, equal, alloc)
    {
    }
    template<typename InputIterator>
    unordered_multimap(InputIterator first, InputIterator last, size_type bucket_count, const Allocator& alloc) // 7
        : unordered_multimap(first, last, bucket_count, Hash(), KeyEqual(), alloc)
    {
    }
    template<typename InputIterator>
    unordered_multimap(InputIterator first, InputIterator last, size_type bucket_count, const Hash& hash, const Allocator& alloc) // 8
        : unordered_multimap(first, last, bucket_count, hash, KeyEqual(), alloc)
    {
    }
    unordered_multimap(const unordered_multimap& other) // 9
        : table(other.table, other.get_allocator())
    {
    }
    unordered_multimap(const unordered_multimap& other, const Allocator& alloc) // 10
        : table(other.table, alloc)
    {
    }
    unordered_multimap(unordered_multimap&& other) // 11
        : table(std::move(other.table))
    {
    }
    unordered_multimap(unordered_multimap&& other, const Allocator& alloc) // 12
        : table(std::move(other.table), alloc)
    {
    }
    unordered_multimap(std::initializer_list<value_type> il, size_type bucket_count = initial_bucket_size,
        const Hash& hash = Hash(), const KeyEqual& equal = KeyEqual(), const Allocator& alloc = Allocator()) // 13
        : unordered_multimap(il.begin(), il.end(), bucket_count, hash, equal, alloc)
    {
    }
    unordered_multimap(std::initializer_list<value_type> il, size_type bucket_count, const  Allocator& alloc) // 14
        : unordered_multimap(il, bucket_count, Hash(), KeyEqual(), alloc)
    {
    }
    unordered_multimap(std::initializer_list<value_type> il, size_type bucket_count, const Hash& hash, const Allocator& alloc) // 15
        : unordered_multimap(il, bucket_count, hash, KeyEqual(), alloc)
    {
    }
    ~unordered_multimap()
    {
    }
    // assignment
    unordered_multimap& operator=(const unordered_multimap& other) // 1
    {
        table = other.table;
        return *this;
    }
    unordered_multimap& operator=(unordered_multimap&& other) // 2
        noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_move_assignable_v<Hash> && std::is_nothrow_move_assignable_v<KeyEqual>)
    {
        table = std::move(other.table);
        return *this;
    }
    unordered_multimap& operator=(std::initializer_list<value_type> il) // 3
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
    iterator insert(const value_type& value) // 1
    {
        return table.insert(value).first;
    }
    iterator insert(value_type&& value) // 2
    {
        return table.insert(std::move(value)).first;
    }
    template<typename P,
        typename = std::enable_if_t<std::is_constructible_v<value_type, P&&>>>
    iterator insert(P&& value) // 3
    {
        return table.emplace(std::forward<P>(value)).first;
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
    // iterator insert(node_type&& nh) // 9
    // {
    // }
    // iterator insert(const_iterator hint, node_type&& nh) // 10
    // {
    // }

    // emplace
    template<typename... Args>
    iterator emplace(Args&&... args)
    {
        return table.emplace(std::forward<Args>(args)...).first;
    }
    // emplace_hint
    template<typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args)
    {
        return table.emplace(std::forward<Args>(args)...).first;
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
    void swap(unordered_multimap& other)
        noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable_v<Hash> && std::is_nothrow_swappable_v<KeyEqual>)
    {
        table.swap(other.table);
    }
    // extract and merge: do not support

    // lookup
    // count
    size_type count(const Key& key) const // 1
    {
        auto p = equal_range(key);
        return (size_type)tstd::distance(p.first, p.second);
    }
    template<typename K>
    size_type count(const K& x) const // 2
    {
        auto p = equal_range(Key(x));
        return (size_type)tstd::distance(p.first, p.second);
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
    // unordered_multimap max load factor
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
bool operator==(const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs)
{
    return lhs.table == rhs.table;
}
template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator, typename UnderlyingHashTable>
bool operator!=(const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, const tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs)
{
    return lhs.table != rhs.table;
}

// global swap for tstd::unordered_multimap
template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator, typename UnderlyingHashTable>
void swap(tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& lhs, tstd::unordered_multimap<Key, T, Hash, KeyEqual, Allocator, UnderlyingHashTable>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace tstd


#endif // TUNORDERED_MULTIMAP_HPP