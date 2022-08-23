#ifndef TSTL_HASHTABLE_HPP
#define TSTL_HASHTABLE_HPP

#include <functional>
#include <iterator>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <algorithm> // todo: replace with <talgorihtm.hpp>
#include <cmath>
#include <tstl_allocator.hpp>
#include <tvector.hpp>
#include <tutility.hpp>

namespace tstd
{
namespace impl
{

// forward declaration
template<typename Key, typename Value, typename KeyOfValue, bool Multi = false, typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>, typename Allocator = tstd::allocator<Value>>
class hash_table
{
private:
    // list node
    template<typename _Value>
    struct __hash_table_node
    {
        __hash_table_node* next;
        _Value data;
    };
    // iterator
    template<typename _Value, typename HashTable, typename Ref, typename Ptr>
    struct __hash_table_iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using value_type = _Value;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = Ref;
        using pointer = Ptr;

        using hash_table = HashTable;
        using iterator = __hash_table_iterator<_Value, std::remove_cv_t<HashTable>, _Value&, _Value*>;
        using self = __hash_table_iterator<_Value, HashTable, Ref, Ptr>;
        using node = __hash_table_node<_Value>;

        node* cur;
        hash_table* ht;

        __hash_table_iterator(node* _n, hash_table* _tbl) : cur(_n), ht(_tbl) {}
        __hash_table_iterator() : cur(nullptr), ht(nullptr) {}
        __hash_table_iterator(const iterator& it) : cur(it.cur), ht(it.ht) {}
        reference operator*() const { return cur->data; }
        pointer operator->() const { return &(operator*()); }
        self& operator++()
        {
            if (cur == nullptr)
            {
                return *this; // protection on end() iterator
            }
            const node* old = cur;
            cur = cur->next;
            if (cur == nullptr)
            {
                size_type bucket = ht->bucket(hash_table::key(old)); // the index of bucket of old value
                while (cur == nullptr && ++bucket < ht->bucket_count())
                {
                    cur = ht->buckets[bucket];
                }
            }
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        bool operator==(const self& it) const
        {
            return cur == it.cur;
        }
        bool operator!=(const self& it) const
        {
            return cur != it.cur;
        }
    };
    // local iterator
    template<typename _Value, typename Ref, typename Ptr>
    struct __hash_table_local_iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using value_type = _Value;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = Ref;
        using pointer = Ptr;

        using self = __hash_table_local_iterator<_Value, Ref, Ptr>;
        using local_iterator = __hash_table_local_iterator<_Value, _Value&, _Value*>;
        using node = __hash_table_node<_Value>;

        node* cur;
        __hash_table_local_iterator() : cur(nullptr) {} // end of local_iterator
        __hash_table_local_iterator(node* p) : cur(p) {}
        __hash_table_local_iterator(const local_iterator& it) : cur(it.cur) {}
        reference operator*() { return cur->data; }
        pointer operator->() const { return &(operator*()); }
        self& operator++()
        {
            cur = cur->next;
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        bool operator==(const self& it) const
        {
            return cur == it.cur;
        }
        bool operator!=(const self& it) const
        {
            return cur != it.cur;
        }
    };
public:
    using key_type = Key;
    using value_type = Value;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = Allocator::pointer;
    using const_pointer = Allocator::const_pointer;
    using iterator = __hash_table_iterator<Value, hash_table, reference, pointer>;
    using const_iterator = __hash_table_iterator<Value, const hash_table, const_reference, const_pointer>;
    using local_iterator = __hash_table_local_iterator<value_type, reference, pointer>;
    using const_local_iterator = __hash_table_local_iterator<value_type, const_reference, const_pointer>;
    // node_type and insert_return_type are not supported
private:
    using node = __hash_table_node<Value>;
    using vec_allocator = Allocator::template rebind<node*>::other;
    using node_allocator = Allocator::template rebind<node>::other;
    using link_type = node*;
    using buckets_type = tstd::vector<link_type, vec_allocator>;
private:
    hasher hash_func;
    key_equal k_equal;
    allocator_type alloc;
    vec_allocator vec_alloc; // todo: consider could get rid of vec_alloc?
    node_allocator node_alloc;
    size_type elem_count;
    buckets_type buckets;
    float max_load_factor_value;
private:
    // inner auxiliary functions
    // memory and object construction/destruction
    [[nodiscard]] link_type get_node()
    {
        return node_alloc.allocate(1);
    }
    void release_node(link_type p)
    {
        node_alloc.deallocate(p, 1);
    }
    template<typename... Args>
    [[nodiscard]] link_type construct_node(Args&&... args)
    {
        link_type p = get_node();
        node_alloc.construct(&p->data, std::forward<Args>(args)...);
        p->next = nullptr;
        return p;
    }
    void destroy_node(link_type p)
    {
        if (p)
        {
            node_alloc.destroy(&p->data);
            release_node(p);
        }
    }
    // init an empty table, buckets has no elements when call init_empty
    void init_empty(size_type bucket_count)
    {
        buckets.resize(bucket_count); // will set every value to 0/nullptr, it's important.
    }
    // insert to set/map, do not insert if exist.
    std::pair<iterator, bool> insert_unique(link_type new_node)
    {
        if ((elem_count + 1) * 1.0 / bucket_count() > max_load_factor())
        {
            rehash(2 * bucket_count());
        }
        return insert_node_to_buckets_unique(new_node, buckets);
    }
    // insert to unordered_multimap/unordered_multimap, second of return value will always be true.
    std::pair<iterator, bool> insert_multi(link_type new_node)
    {
        if ((elem_count + 1) * 1.0 / bucket_count() > max_load_factor())
        {
            rehash(2 * bucket_count());
        }
        return insert_node_to_buckets_multi(new_node, buckets);
    }
    // insert to specific buckets, unique mode
    std::pair<iterator, bool> insert_node_to_buckets_unique(link_type p, buckets_type& vec)
    {
        size_type index = hash_func(key(p)) % vec.size();
        if (vec[index] == nullptr)
        {
            vec[index] = p;
            p->next = nullptr;
        }
        else
        {
            link_type prev = nullptr;
            link_type tmp = vec[index];
            while (tmp)
            {
                // have same key, do not insert, and destroy the input node.
                if (k_equal(key(p), key(tmp)))
                {
                    destroy_node(p);
                    return {iterator(tmp, this), false};
                }
                prev = tmp;
                tmp = tmp->next;
            }
            // do not have same key, insert to the end of the list.
            prev->next = p;
            p->next = nullptr;
        }
        ++elem_count;
        return {iterator(p, this), true};
    }
    // insert to specific buckets, multi mode
    std::pair<iterator, bool> insert_node_to_buckets_multi(link_type p, buckets_type& vec)
    {
        size_type index = hash_func(key(p)) % vec.size();
        if (vec[index] == nullptr)
        {
            vec[index] = p;
            p->next = nullptr;
        }
        else
        {
            link_type prev_equal = nullptr;
            link_type prev = nullptr;
            link_type tmp = vec[index];
            while (tmp)
            {
                if (k_equal(key(p), key(tmp)))
                {
                    prev_equal = tmp;
                }
                else if (prev_equal)
                {
                    break;
                }
                prev = tmp;
                tmp = tmp->next;
            }
            // have same key, insert after prev_equal.
            if (prev_equal)
            {
                p->next = prev_equal->next;
                prev_equal->next = p;
            }
            // do not have same key, insert to the end of the list.
            else
            {
                prev->next = p;
                p->next = nullptr;
            }
        }
        ++elem_count;
        return {iterator(p, this), true};
    }
    // insert node to buckets, make sure p isn't nullptr
    void insert_node_to_buckets(link_type p, buckets_type& vec)
    {
        if constexpr (Multi)
        {
            insert_node_to_buckets_multi(p, vec);
        }
        else
        {
            insert_node_to_buckets_unique(p, vec);
        }
    }
    // get key of value
    static const key_type& key(const node* p)
    {
        static KeyOfValue get_key;
        return get_key(p->data);
    }
public:
    // constuctors
    // initialize an empty hash table
    hash_table(size_type bucket_count, const Hash& hash = Hash(),
        const KeyEqual& equal = KeyEqual(), const Allocator& _alloc = Allocator()) // 1
        : hash_func(hash)
        , k_equal(equal)
        , alloc(_alloc)
        , vec_alloc(alloc)
        , node_alloc(alloc)
        , elem_count(0)
        , buckets(vec_alloc)
        , max_load_factor_value(1.0)
    {
        init_empty(bucket_count);
    }
    // initialize with a sequence
    template<typename InputIterator>
    hash_table(InputIterator first, InputIterator last, size_type bucket_count, const Hash& hash = Hash(),
        const KeyEqual& equal = KeyEqual(), const Allocator& _alloc = Allocator()) // 2
        : hash_func(hash)
        , k_equal(equal)
        , alloc(_alloc)
        , vec_alloc(alloc)
        , node_alloc(alloc)
        , elem_count(0)
        , buckets(vec_alloc)
        , max_load_factor_value(1.0)
    {
        init_empty(bucket_count);
        for (; first != last; ++first)
        {
            insert(*first);
        }
    }
    // copy initialization
    hash_table(const hash_table& other, const Allocator _alloc = Allocator()) // 3
        : hash_func(other.hash_func)
        , k_equal(other.k_equal)
        , alloc(_alloc)
        , vec_alloc(alloc)
        , node_alloc(alloc)
        , elem_count(other.elem_count)
        , buckets(vec_alloc)
        , max_load_factor_value(other.max_load_factor_value)
    {
        init_empty(other.bucket_count());
        for (auto& elem : other)
        {
            insert(elem);
        }
    }
    // move initialization, move the whole hash table
    hash_table(hash_table&& other) // 4
        : hash_func(std::move(other.hash_func))
        , k_equal(std::move(other.k_equal))
        , alloc(std::move(other.alloc))
        , vec_alloc(std::move(other.vec_alloc))
        , node_alloc(std::move(other.node_alloc))
        , elem_count(other.elem_count)
        , buckets(std::move(other.buckets))
        , max_load_factor_value(other.max_load_factor_value)
    {
        other.elem_count = 0;
    }
    // move initialization, move the whole hash table or move every element
    hash_table(hash_table&& other, const Allocator& _alloc) // 5
        : hash_func(std::move(other.hash_func))
        , k_equal(std::move(other.k_equal))
        , alloc(_alloc)
        , vec_alloc(alloc)
        , node_alloc(alloc)
        , elem_count(0)
        , buckets(vec_alloc)
        , max_load_factor_value(other.max_load_factor_value)
    {
        if (alloc == other.get_allocator()) // move the whole hash table
        {
            buckets = std::move(other.buckets);
            elem_count = other.elem_count;
            other.elem_count = 0;
        }
        else // move every element
        {
            init_empty(other.bucket_count());
            for (auto& elem : other)
            {
                insert(std::move(elem));
            }
            other.clear();
        }
    }
    ~hash_table()
    {
        clear();
    }
    // assignment
    hash_table& operator=(const hash_table& other) // 1
    {
        clear();
        hash_func = other.hash_func;
        k_equal = other.k_equal;
        alloc = other.alloc;
        vec_alloc = other.vec_alloc;
        node_alloc = other.node_alloc;
        max_load_factor_value = other.max_load_factor_value;
        buckets = buckets_type(other.bucket_count(), vec_alloc);
        for (auto& elem : other)
        {
            insert(elem);
        }
        return *this;
    }
    hash_table& operator=(hash_table&& other) // 2
    {
        hash_func = std::move(other.hash_func);
        k_equal = std::move(other.k_equal);
        alloc = std::move(other.alloc);
        vec_alloc = std::move(other.vec_alloc);
        node_alloc = std::move(node_alloc);
        elem_count = other.elem_count;
        buckets = std::move(other.buckets);
        max_load_factor_value = other.max_load_factor_value;
        other.elem_count = 0;
        return *this;
    }
    hash_table& operator=(std::initializer_list<value_type> il) // 3
    {
        clear();
        for (auto& elem : il)
        {
            insert(elem);
        }
        return *this;
    }
    // allocator
    allocator_type get_allocator() const noexcept
    {
        return alloc;
    }

    // iterators
    iterator begin() noexcept
    {
        for (link_type p : buckets)
        {
            if (p)
            {
                return iterator(p, this);
            }
        }
        return iterator(nullptr, this);
    }
    const_iterator begin() const noexcept
    {
        for (link_type p : buckets)
        {
            if (p)
            {
                return const_iterator(p, this);
            }
        }
        return const_iterator(nullptr, this);
    }
    const_iterator cbegin() const noexcept
    {
        for (link_type p : buckets)
        {
            if (p)
            {
                return const_iterator(p, this);
            }
        }
        return const_iterator(nullptr, this);
    }
    iterator end() noexcept
    {
        return iterator(nullptr, this);
    }
    const_iterator end() const noexcept
    {
        return const_iterator(nullptr, this);
    }
    const_iterator cend() const noexcept
    {
        return const_iterator(nullptr, this);
    }
    // size and capacity
    [[nodiscard]] bool empty() const noexcept
    {
        return elem_count == 0;
    }
    size_type size() const noexcept
    {
        return elem_count;
    }
    size_type max_size() const noexcept
    {
        return buckets.max_size();
    }
    // modifiers
    void clear() noexcept
    {
        elem_count = 0;
        for (link_type& p : buckets)
        {
            if (p)
            {
                link_type tmp = p;
                p = nullptr;
                while (tmp)
                {
                    link_type next = tmp->next;
                    destroy_node(tmp);
                    tmp = next;
                }
            }
        }
    }
    // insert
    std::pair<iterator, bool> insert(const value_type& value) // 1
    {
        if constexpr (Multi)
        {
            return insert_multi(construct_node(value));
        }
        else
        {
            return insert_unique(construct_node(value));
        }
    }
    std::pair<iterator, bool> insert(value_type&& value) // 2
    {
        if constexpr (Multi)
        {
            return insert_multi(construct_node(std::move(value)));
        }
        else
        {
            return insert_unique(construct_node(std::move(value)));
        }
    }
    template<typename...Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        if constexpr (Multi)
        {
            return insert_multi(construct_node(args...));
        }
        else
        {
            return insert_unique(construct_node(args...));
        }
    }
    // erase, return the element after erased element
    iterator erase(const_iterator pos) // 1
    {
        if (pos.cur == nullptr)
        {
            return end();
        }
        iterator ret = ++iterator(pos.cur, this);
        link_type p = pos.cur;
        size_type index = bucket(key(p));
        link_type tmp = buckets[index];
        link_type prev = nullptr;
        // pos is first node of bucket
        if (tmp == p)
        {
            buckets[index] = p->next;
            destroy_node(p);
            --elem_count;
            return ret;
        }
        // pos is not first node of bucket
        while (tmp)
        {
            if (tmp->next == p)
            {
                prev = tmp;
                break;
            }
            tmp = tmp->next;
        }
        if (prev)
        {
            prev->next = p->next;
            destroy_node(p);
            --elem_count;
            return ret;
        }
        // else the element do not exist, do nothing, theoretically it won't go here.
        return end();
    }
    iterator erase(const_iterator first, const_iterator last) // 2
    {
        iterator ret = iterator(first.cur, this);
        while (first != last)
        {
            ret = erase(first);
            first = ret;
        }
        return ret;
    }
    // erase, return the numbers of elements removed
    size_type erase(const key_type& k) // 3
    {
        size_type count = 0;
        size_type index = bucket(k);
        link_type tmp = buckets[index];
        if (k_equal(k, key(tmp))) // at the begin of list
        {
            while (tmp && k_equal(k, key(tmp)))
            {
                buckets[index] = tmp->next;
                destroy_node(tmp);
                --elem_count;
                tmp = buckets[index];
                ++count;
            }
        }
        else // at the middle of list
        {
            link_type prev = tmp;
            while (tmp)
            {
                if (k_equal(k, key(tmp)))
                {
                    prev->next = tmp->next;
                    destroy_node(tmp);
                    --elem_count;
                    tmp = prev->next;
                    ++count;
                }
                else
                {
                    prev = tmp;
                    tmp = tmp->next;
                }
            }
        }
        return count;
    }
    // swap
    void swap(hash_table& other)
    {
        tstd::swap(hash_func, other.hash_func);
        tstd::swap(k_equal, other.k_equal);
        tstd::swap(alloc, other.alloc);
        tstd::swap(vec_alloc, other.vec_alloc);
        tstd::swap(node_alloc, other.node_alloc);
        tstd::swap(elem_count, other.elem_count);
        tstd::swap(buckets, other.buckets);
        tstd::swap(max_load_factor_value, other.max_load_factor_value);
    }
    // lookup
    // find
    iterator find(const Key& k) // 1
    {
        size_type index = bucket(k);
        link_type tmp = buckets[index];
        while (tmp)
        {
            if (k_equal(k, key(tmp)))
            {
                return iterator(tmp, this);
            }
            tmp = tmp->next;
        }
        return end();
    }
    const_iterator find(const Key& k) const // 2
    {
        size_type index = bucket(k);
        link_type tmp = buckets[index];
        while (tmp)
        {
            if (k_equal(k, key(tmp)))
            {
                return const_iterator(tmp, this);
            }
            tmp = tmp->next;
        }
        return end();
    }
    // equal_range
    std::pair<iterator, iterator> equal_range(const Key& k) // 1
    {
        size_type index = bucket(k);
        link_type tmp = buckets[index];
        link_type first = nullptr;
        link_type last = nullptr;
        while (tmp)
        {
            if (k_equal(k, key(tmp)))
            {
                if (first == nullptr)
                {
                    first = tmp;
                }
                last = tmp;
            }
            else if (first)
            {
                last = tmp;
                break;
            }
            tmp = tmp->next;
        }
        if (first)
        {
            if (k_equal(k, key(last)))
            {
                return {iterator(first, this), ++iterator(last, this)};
            }
            else
            {
                return {iterator(first, this), iterator(last, this)};
            }
        }
        return {end(), end()};
    }
    std::pair<const_iterator, const_iterator> equal_range(const Key& k) const // 2
    {
        size_type index = bucket(k);
        link_type tmp = buckets[index];
        link_type first = nullptr;
        link_type last = nullptr;
        while (tmp)
        {
            if (k_equal(k, key(tmp)))
            {
                if (first == nullptr)
                {
                    first = tmp;
                }
                last = tmp;
            }
            else if (first)
            {
                last = tmp;
                break;
            }
            tmp = tmp->next;
        }
        if (first)
        {
            if (k_equal(k, key(last)))
            {
                return {const_iterator(first, this), ++const_iterator(last, this)};
            }
            else
            {
                return {const_iterator(first, this), const_iterator(last, this)};
            }
        }
        return {end(), end()};
    }
    // bucket interfaces
    // local iterators
    local_iterator begin(size_type n)
    {
        return local_iterator(buckets[n]);
    }
    const_local_iterator begin(size_type n) const
    {
        return const_local_iterator(buckets[n]);
    }
    const_local_iterator cbegin(size_type n) const
    {
        return const_local_iterator(buckets[n]);
    }
    local_iterator end(size_type n)
    {
        return local_iterator(nullptr);
    }
    const_local_iterator end(size_type n) const
    {
        return const_local_iterator(nullptr);
    }
    const_local_iterator cend(size_type n) const
    {
        return const_local_iterator(nullptr);
    }
    // bucket_count
    size_type bucket_count() const
    {
        return buckets.size();
    }
    // max_bucket_count
    size_type max_bucket_count() const
    {
        return buckets.max_size();
    }
    // bucket_size
    size_type bucket_size(size_type n) const
    {
        link_type p = buckets[n];
        size_type count = 0;
        while (p)
        {
            ++count;
            p = p->next;
        }
        return count;
    }
    // bucket
    size_type bucket(const Key& k) const
    {
        return hash_func(k) % bucket_count();
    }
    
    // hash policy
    float load_factor() const
    {
        return size() * 1.0 / bucket_count();
    }
    // get max load factor
    float max_load_factor() const
    {
        return max_load_factor_value;
    }
    // unordered_map max load factor
    void max_load_factor(float ml)
    {
        max_load_factor_value = ml;
    }
    // rehash
    void rehash(size_type count)
    {
        size_type new_bucket_count = std::max(count, size_type(std::ceil(size() / max_load_factor_value)));
        new_bucket_count = std::max(new_bucket_count, size_type(16));
        if (new_bucket_count != bucket_count())
        {
            elem_count = 0;
            buckets_type new_vec(new_bucket_count, vec_alloc);
            for (link_type p : buckets)
            {
                while (p)
                {
                    link_type tmp = p->next;
                    insert_node_to_buckets(p, new_vec);
                    p = tmp;
                }
            }
            buckets = std::move(new_vec);
        }
        // else do nothing.
    }
    void reserve(size_type count)
    {
        rehash(std::ceil(count * 1.0 / max_load_factor_value));
    }

    // observers
    hasher hash_function() const
    {
        return hash_func;
    }
    key_equal key_eq() const
    {
        return k_equal;
    }
};

// non-member operations
// comparisons
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Hash, typename KeyEqual, typename Allocator>
bool operator==(const tstd::impl::hash_table<Key, Value, KeyOfValue, Multi, Hash, KeyEqual, Allocator>& lhs, const tstd::impl::hash_table<Key, Value, KeyOfValue, Multi, Hash, KeyEqual, Allocator>& rhs)
{
    static KeyOfValue get_key;
    if (lhs.size() != rhs.size())
    {
        return false;
    }
    for (auto& elem : lhs)
    {
        auto pl = lhs.equal_range(get_key(elem));
        auto pr = rhs.equal_range(get_key(elem));
        if (tstd::distance(pl.first, pl.second) != tstd::distance(pr.first, pr.second)
            || !std::is_permutation(pl.first, pl.second, pr.first))
        {
            return false;
        }
    }
    return true;
}
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Hash, typename KeyEqual, typename Allocator>
bool operator!=(const tstd::impl::hash_table<Key, Value, KeyOfValue, Multi, Hash, KeyEqual, Allocator>& lhs, const tstd::impl::hash_table<Key, Value, KeyOfValue, Multi, Hash, KeyEqual, Allocator>& rhs)
{
    return !(lhs == rhs);
}

// global swap for tstd::impl::hash_table
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Hash, typename KeyEqual, typename Allocator>
void swap(tstd::impl::hash_table<Key, Value, KeyOfValue, Multi, Hash, KeyEqual, Allocator>& lhs, tstd::impl::hash_table<Key, Value, KeyOfValue, Multi, Hash, KeyEqual, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace impl
} // namespace tstd


#endif // TSTL_HASHTABLE_HPP