#ifndef TSTL_BST_HPP
#define TSTL_BST_HPP

#include <cstddef>
#include <titerator.hpp>
#include <tstl_allocator.hpp>
#include <functional>
#include <limits>
#include <tqueue.hpp>

namespace tstd
{
namespace impl
{
// generic normal binary search tree implementation
// header: left point to first element, right point to right element, parent point to root
// root: parent is nullptr.

// node
struct __bst_node_base
{
    using base_ptr = __bst_node_base*;
    base_ptr parent;
    base_ptr left;
    base_ptr right;

    // find the minimum element(the leftmost element) of a tree 
    static base_ptr minimum(base_ptr x)
    {
        while (x->left)
        {
            x = x->left;
        }
        return x;
    }
    // find the maximum element(the rightmost element) of a tree 
    static base_ptr maximum(base_ptr x)
    {
        while (x->right)
        {
            x = x->right;
        }
        return x;
    }
};

template<typename T>
struct __bst_node : public __bst_node_base
{
    using link_type = __bst_node<T>*;
    T data; // value of node
};

// iterator
struct __bst_iterator_base
{
    using base_ptr = __bst_node_base::base_ptr;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    base_ptr node;
    base_ptr header;
    __bst_iterator_base(base_ptr _node, base_ptr _header) : node(_node), header(_header) {}
    __bst_iterator_base(const __bst_iterator_base& other) : node(other.node), header(other.header) {}
    void increment()
    {
        // a little bit tricky
        // case 1: node is header / end(), make it a circle
        if (node == header)
        {
            node = header->left;
        }
        // case 2: node has right child, get leftmost node of right child
        else if (node->right)
        {
            node = node->right;
            while (node->left)
            {
                node = node->left;
            }
        }
        else
        {
            base_ptr y = node->parent;
            while (y && node == y->right)
            {
                node = y;
                y = y->parent;
            }
            // case 3: y != nullptr, so node == y->left
            if (y)
            {
                node = y;
            }
            // case 4: y == nullptr, so next will be end()
            else
            {
                node = header;
            }
        }
    }
    void decrement()
    {
        // case 1: node is header
        if (node == header)
        {
            node = node->right;
        }
        // case 2: node has left child, get rightmost node of left child
        else if (node->left)
        {
            base_ptr y = node->left;
            while (y->right)
            {
                y = y->right;
            }
            node = y;
        }
        else
        {
            base_ptr y = node->parent;
            while (y && node == y->left)
            {
                node = y;
                y = y->parent;
            }
            // case 3: y != nullptr, so node == y->right
            if (y)
            {
                node = y;
            }
            // case 4: y == nullptr, so this iterator is begin(), make it a circle
            else
            {
                node = header;
            }
        }
    }
};

template<typename Value, typename Ref, typename Ptr>
struct __bst_iterator : public __bst_iterator_base
{
    using value_type = Value;
    using reference = Ref;
    using pointer = Ptr;

    using iterator = __bst_iterator<Value, Value&, Value*>;
    using const_iterator = __bst_iterator<Value, const Value&, const Value*>;
    using self = __bst_iterator<Value, Ref, Ptr>;
    using link_type = __bst_node<Value>*; // or __bst_node<Value>::link_type

    __bst_iterator() {}
    __bst_iterator(link_type x, link_type header) : __bst_iterator_base(x, header) {}
    __bst_iterator(const iterator& it) : __bst_iterator_base(it) {}
    self& operator=(const self& other)
    {
        node = other.node;
        header = other.header;
        return *this;
    }
    bool operator==(const self& other)
    {
        return node == other.node && header == other.header;
    }
    reference operator*() const
    {
        return link_type(node)->data;
    }
    pointer operator->() const
    {
        return &(operator*());
    }
    self& operator++()
    {
        increment();
        return *this;
    }
    self operator++(int)
    {
        self tmp = *this;
        increment();
        return tmp;
    }
    self& operator--()
    {
        decrement();
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        decrement();
        return tmp;
    }
};

// bst-tree implementation
template<typename Key, typename Value, typename KeyOfValue, bool Multi = false, typename Compare = std::less<Key>, typename Allocator = tstd::allocator<Value>>
class bst
{
    using base_ptr = __bst_node_base*;
    using bst_node = __bst_node<Value>;
    using bst_node_allocator = typename Allocator::template rebind<bst_node>::other;
    using link_type = bst_node*;
public:
    using key_type = Key;
    using value_type = Value;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = __bst_iterator<value_type, reference, pointer>;
    using const_iterator = __bst_iterator<value_type, const_reference, const_pointer>;
    using reverse_iterator = tstd::reverse_iterator<iterator>;
    using const_reverse_iterator = tstd::reverse_iterator<const_iterator>;
    // node_type and insert_return_type (since C++17) are not supported.
// data members
private:
    allocator_type alloc;
    bst_node_allocator node_alloc;
    size_type node_count;
    link_type header; // header node, whose parent point to root, left point to leftmost node, right point to rightmost node, represent end() iterator at the same time.
    Compare key_cmp;
private:
    // inner auxiliary functions
    // memory and construction/destruction
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
        parent(p) = left(p) = right(p) = nullptr;
        return p;
    }
    // clone a node
    [[nodiscard]] link_type clone_node(link_type p)
    {
        link_type tmp = construct_node(p->data);
        left(tmp) = right(tmp) = nullptr;
        return tmp;
    }
    void destroy_node(link_type p)
    {
        if (p)
        {
            node_alloc.destroy(&p->data);
            release_node(p);
        }
    }
    // get member of header
    link_type& root() const
    {
        return (link_type&)header->parent;
    }
    link_type& leftmost() const
    {
        return (link_type&)header->left;
    }
    link_type& rightmost() const
    {
        return (link_type&)header->right;
    }
    // get member of node x
    static link_type& left(link_type x)
    {
        return (link_type&)(x->left);
    }
    static link_type& right(link_type x)
    {
        return (link_type&)(x->right);
    }
    static link_type& parent(link_type x)
    {
        return (link_type&)(x->parent);
    }
    static reference value(link_type x)
    {
        return x->data;
    }
    static Key& key(link_type x)
    {
        return KeyOfValue()(value(x));
    }
    // get minimum and maximum
    static link_type minimum(link_type x)
    {
        return (link_type)__bst_node_base::minimum(x);
    }
    static link_type maximum(link_type x)
    {
        return (link_type)__bst_node_base::maximum(x);
    }
    // init an empty tree
    void init_empty()
    {
        header = get_node();
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }
    // copy from another tree
    void copy_node(link_type src, link_type& dest)
    {
        if (src)
        {
            dest = construct_node(value(src));
            copy_node(left(src), left(dest));
            copy_node(right(src), right(dest));
            if (left(dest))
            {
                parent(left(dest)) = dest;
            }
            if (right(dest))
            {
                parent(right(dest)) = dest;
            }
        }
        else
        {
            dest = nullptr;
        }
    }
    void copy_from(const bst& other)
    {
        copy_node(other.root(), root());
        node_count = other.node_count;
        if (root())
        {
            leftmost() = minimum(root());
            rightmost() = maximum(root());
        }
        else
        {
            leftmost() = header;
            rightmost() = header;
        }
    }
    // move from other bst
    void move_from(bst&& other)
    {
        header->left = other.header->left;
        header->right = other.header->right;
        header->parent = other.header->parent;
        other.root() = nullptr;
        other.leftmost() = other.header;
        other.rightmost() = other.header;
        node_count = other.node_count;
        other.node_count = 0;
    }
    // insert to set/map
    std::pair<iterator, bool> insert_unique(link_type new_node)
    {
        const key_type k = key(new_node);
        if (empty())
        {
            root() = leftmost() = rightmost() = new_node;
            node_count++;
            return {iterator(new_node, header), true};
        }
        else // not empty
        {
            link_type node = root();
            while (node)
            {
                if (key_cmp(k, key(node))) // insert to left
                {
                    if (left(node))
                    {
                        node = left(node);
                    }
                    else // left(node) == nullptr, insert to left(node)
                    {
                        left(node) = new_node;
                        parent(new_node) = node;
                        if (node == leftmost())
                        {
                            leftmost() = left(node);
                        }
                        node_count++;
                        return {iterator(new_node, header), true};
                    }
                }
                else if (key_cmp(key(node), k)) // insert ot right
                {
                    if (right(node))
                    {
                        node = right(node);
                    }
                    else // right(node) == nullptr, insert ot right(node)
                    {
                        right(node) = new_node;
                        parent(new_node) = node;
                        if (node == rightmost())
                        {
                            rightmost() = right(node);
                        }
                        node_count++;
                        return {iterator(new_node, header), true};
                    }
                }
                else
                {
                    destroy_node(new_node);
                    return {iterator(node, header), false};
                }
            }
        }
        return {end(), true}; // theoretically, it won't go here
    }
    // insert to multiset/multimap, second of return value has no meaning, just ignore the second
    std::pair<iterator, bool> insert_multi(link_type new_node)
    {
        const key_type k = key(new_node);
        if (empty())
        {
            root() = leftmost() = rightmost() = new_node;
            node_count++;
            return {iterator(new_node, header), true};
        }
        else // not empty
        {
            link_type node = root();
            while (node)
            {
                if (key_cmp(k, key(node))) // insert to left
                {
                    if (left(node))
                    {
                        node = left(node);
                    }
                    else // left(node) == nullptr, insert to left(node)
                    {
                        left(node) = new_node;
                        parent(new_node) = node;
                        if (node == leftmost())
                        {
                            leftmost() = left(node);
                        }
                        node_count++;
                        return {iterator(new_node, header), true};
                    }
                }
                else // insert ot right
                {
                    if (right(node))
                    {
                        node = right(node);
                    }
                    else // right(node) == nullptr, insert ot right(node)
                    {
                        right(node) = new_node;
                        parent(new_node) = node;
                        if (node == rightmost())
                        {
                            rightmost() = right(node);
                        }
                        node_count++;
                        return {iterator(new_node, header), true};
                    }
                }
            }
            return {end(), true}; // theoretically, it won't go here
        }
    }
    bool key_equal(const key_type& key1, const key_type& key2)
    {
        return !key_cmp(key1, key2) && !key_cmp(key1, key2);
    }
public:
    bst(const Compare& comp = Compare(), const Allocator& _alloc = Allocator()) // 1
        : node_count(0)
        , key_cmp(comp)
        , alloc(_alloc)
        , node_alloc(alloc)
        , header(nullptr)
    {
        init_empty();
    }
    bst(const bst& other, const Allocator& _alloc = Allocator()) // 2
        : node_count(0)
        , key_cmp(other.key_cmp)
        , alloc(_alloc)
        , node_alloc(alloc)
        , header(nullptr)
    {
        init_empty();
        copy_from(other);
    }
    bst(bst&& other, Allocator&& _alloc = Allocator()) // 3
        : node_count(0)
        , key_cmp(std::move(other.key_cmp))
        , alloc(std::move(_alloc))
        , node_alloc(alloc)
        , header(nullptr)
    {
        init_empty();
        move_from(std::move(other));
    }
    ~bst()
    {
        clear();
        release_node(header);
    }
    // assignment
    bst& operator=(const bst& other)
    {
        clear();
        release_node(header);
        alloc = other.alloc;
        node_alloc = other.node_alloc;
        key_cmp = other.key_cmp;
        node_count = other.node_count;
        header = nullptr;
        init_empty();
        copy_from(other);
        return *this;
    }
    bst& operator=(bst&& other)
    {
        clear();
        release_node(header);
        alloc = std::move(other.alloc);
        node_alloc = std::move(other.node_alloc);
        key_cmp = std::move(other.key_cmp);
        node_count = other.node_count;
        header = nullptr;
        init_empty();
        move_from(std::move(other));
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
        return iterator(leftmost(), header);
    }
    const_iterator begin() const noexcept
    {
        return const_iterator(leftmost(), header);
    }
    const_iterator cbegin() const noexcept
    {
        return const_iterator(leftmost(), header);
    }
    iterator end() noexcept
    {
        return iterator(header, header);
    }
    const_iterator end() const noexcept
    {
        return const_iterator(header, header);
    }
    const_iterator cend() const noexcept
    {
        return const_iterator(header, header);
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
        return node_count == 0;
    }
    size_type size() const noexcept
    {
        return node_count;
    }
    size_type max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }
    // modifiers
    void clear() noexcept
    {
        tstd::queue<link_type> q;
        if (root())
        {
            q.push(root());
        }
        while (!q.empty())
        {
            link_type p = q.front();
            if (left(p))
            {
                q.push(left(p));
            }
            if (right(p))
            {
                q.push(right(p));
            }
            destroy_node(p);
            q.pop();
        }
        node_count = 0;
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }
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
        if (pos == end())
        {
            return end();
        }
        iterator ret((link_type)tstd::next(pos).node, header);
        link_type node = (link_type)(pos.node);
        const key_type k = key(node);
        // update leftmost()/rightmost()
        if (leftmost() == node)
        {
            leftmost() = (link_type)(tstd::next(pos).node);
        }
        if (rightmost() == node)
        {
            rightmost() = (link_type)(tstd::prev(pos).node);
        }
        // no child
        if (left(node) == nullptr && right(node) == nullptr)
        {
            // node is not root
            if (parent(node))
            {
                link_type& target = left(parent(node)) == node ? left(parent(node)) : right(parent(node));
                target = nullptr;
            }
            // node is root
            else
            {
                root() = nullptr;
            }
            destroy_node(node);
        }
        // only left
        else if (left(node) && right(node) == nullptr)
        {
            parent(left(node)) = parent(node);
            // node is not root
            if (parent(node))
            {
                link_type& target = left(parent(node)) == node ? left(parent(node)) : right(parent(node));
                target = left(node);
            }
            // node is root
            else
            {
                root() = left(node);
            }
            destroy_node(node);
        }
        // only right
        else if (left(node) == nullptr && right(node))
        {
            parent(right(node)) = parent(node);
            // node is not root
            if (parent(node))
            {
                link_type& target = left(parent(node)) == node ? left(parent(node)) : right(parent(node));
                target = right(node);
            }
            // node is root
            else
            {
                root() = right(node);
            }
            destroy_node(node);
        }
        // has left and right child
        else
        {
            // replace node with minimum node of its right child
            // note: rep_node cannot have left child
            link_type rep_node = minimum(right(node));
            // rep_node is node's right, then rep_node must have no left child, just replace node with rep_node
            if (rep_node == right(node))
            {
                left(rep_node) = left(node);
                parent(left(rep_node)) = rep_node;
            }
            // rep_node is not node's right, move rep_node to node
            else
            {
                // rep_node has right child
                if (right(rep_node))
                {
                    left(parent(rep_node)) = right(rep_node);
                    parent(right(rep_node)) = parent(rep_node);
                }
                // rep_node has no child
                else
                {
                    left(parent(rep_node)) = nullptr;
                }
                left(rep_node) = left(node);
                parent(left(rep_node)) = rep_node;
                right(rep_node) = right(node);
                parent(right(rep_node)) = rep_node;
            }
            parent(rep_node) = parent(node);
            // node is not root
            if (parent(node))
            {
                link_type& target = left(parent(node)) == node ? left(parent(node)) : right(parent(node));
                target = rep_node;
            }
            // node is root
            else
            {
                root() = rep_node;
            }
            destroy_node(node);
        }
        node_count--;
        return ret;
    }
    size_type erase(const key_type& k) // 2
    {
        size_type count = 0;
        iterator iter = find(k);
        while (iter != end())
        {
            erase(iter);
            count++;
            iter = find(k);
        }
        return count;
    }
    // swap
    void swap(bst& other)
    {
        tstd::swap(alloc, other.alloc);
        tstd::swap(node_alloc, other.node_alloc);
        tstd::swap(header, other.header);
        tstd::swap(node_count, other.node_count);
        tstd::swap(key_cmp, other.key_cmp);
    }
    // lookup
    iterator find(const Key& k)
    {
        link_type node = root();
        if (node == nullptr)
        {
            return end();
        }
        while (node)
        {
            // k < key(node)
            if (key_cmp(k, key(node)))
            {
                node = left(node);
            }
            // k > key(node)
            else if (key_cmp(key(node), k))
            {
                node = right(node);
            }
            // k == key(node)
            else
            {
                return iterator(node, header);
            }
        }
        return end();
    }
    const_iterator find(const Key& k) const
    {
        link_type node = root();
        if (node == nullptr)
        {
            return end();
        }
        while (node)
        {
            // k < key(node)
            if (key_cmp(k, key(node)))
            {
                node = left(node);
            }
            // k > key(node)
            else if (key_cmp(key(node), k))
            {
                node = right(node);
            }
            // k == key(node)
            else
            {
                return const_iterator(node, header);
            }
        }
        return end();
    }
    // first node that >= k
    iterator lower_bound(const key_type& k)
    {
        link_type node = root();
        link_type prev = nullptr;
        if (node == nullptr)
        {
            return end();
        }
        while (node)
        {
            // key(node) < k
            if (key_cmp(key(node), k))
            {
                node = right(node);
            }
            // key(node) >= k
            else
            {
                prev = node;
                node = left(node);
            }
        }
        return prev ? iterator(prev, header) : end();
    }
    const_iterator lower_bound(const key_type& k) const
    {
        link_type node = root();
        link_type prev = nullptr;
        if (node == nullptr)
        {
            return end();
        }
        while (node)
        {
            // key(node) < k
            if (key_cmp(key(node), k))
            {
                node = right(node);
            }
            // key(node) >= k
            else
            {
                prev = node;
                node = left(node);
            }
        }
        return prev ? const_iterator(prev, header) : end();
    }
    // first node that > k
    iterator upper_bound(const key_type& k)
    {
        link_type node = root();
        link_type prev = nullptr;
        if (node == nullptr)
        {
            return end();
        }
        while (node)
        {
            // key(node) > k
            if (key_cmp(k, key(node)))
            {
                prev = node;
                node = left(node);
            }
            // key(node) <= k
            else
            {
                node = right(node);
            }
        }
        return prev ? iterator(prev, header) : end();
    }
    const_iterator upper_bound(const key_type& k) const
    {
        link_type node = root();
        link_type prev = nullptr;
        if (node == nullptr)
        {
            return end();
        }
        while (node)
        {
            // key(node) > k
            if (key_cmp(k, key(node)))
            {
                prev = node;
                node = left(node);
            }
            // key(node) <= k
            else
            {
                node = right(node);
            }
        }
        return prev ? const_iterator(prev, header) : end();
    }
    // observers
    Compare key_comp() const
    {
        return key_cmp;
    }
    Compare value_comp() const
    {
        return key_cmp;
    }
};

// non-member operations
// a non-standard compare function for tstd::impl::bst
// equal 0 less -1 greater 1
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr int _cmp_bst(const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
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

// comparisons
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr bool operator==(const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_bst(lhs, rhs) == 0;
}
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr bool operator!=(const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_bst(lhs, rhs) != 0;
}
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr bool operator<(const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_bst(lhs, rhs) < 0;
}
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr bool operator<=(const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_bst(lhs, rhs) <= 0;
}
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr bool operator>(const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_bst(lhs, rhs) > 0;
}
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr bool operator>=(const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_bst(lhs, rhs) >= 0;
}

// global swap for tstd::impl::bst
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr void swap(tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, tstd::impl::bst<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace impl
} // namespace tstd


#endif // TSTL_BST_HPP