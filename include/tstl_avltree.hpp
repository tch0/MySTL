#ifndef TSTL_AVLTREE_HPP
#define TSTL_AVLTREE_HPP

#include <cstddef>
#include <titerator.hpp>
#include <tstl_allocator.hpp>
#include <functional>
#include <limits>

namespace tstd
{

// generic avl-tree utilities
// for all associative containers: map, set, multimap, multiset

// color
using __rb_tree_color_type = bool;
constexpr __rb_tree_color_type __rb_tree_red = true;
constexpr __rb_tree_color_type __rb_tree_black = false;

// node
struct __rb_tree_node_base
{
    using color_type = __rb_tree_color_type;
    using base_ptr = __rb_tree_node_base*;
    color_type color;
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
struct __rb_tree_node : public __rb_tree_node_base
{
    using link_type = __rb_tree_node<T>*;
    T data; // value of node
};

// iterator
struct __rb_tree_iterator_base
{
    using base_ptr = __rb_tree_node_base::base_ptr;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    base_ptr node;
    void increment()
    {
        // have right node, get leftmost node of right child
        if (node->right)
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
            while (node == y->right)
            {
                node = y;
                y = y->parent;
            }
            if (node->right != y)
            {
                node = y;
            }
        }
    }
    void decrement()
    {
        // node is header (node is end())
        if (node->color == __rb_tree_red && node->parent->parent == node)
        {
            node = node->right;
        }
        // have left child, get rightmost node of left child
        else if (node->left)
        {
            base_ptr y = node->left;
            while (y->right)
            {
                y = y->right;
            }
            node = y;
        }
        // not root node, no left child
        else
        {
            base_ptr y = node->parent;
            while (node == y->left)
            {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

template<typename Value, typename Ref, typename Ptr>
struct __rb_tree_iterator : public __rb_tree_iterator_base
{
    using value_type = Value;
    using reference = Ref;
    using poinetr = Ptr;

    using iterator = __rb_tree_iterator<Value, Value&, Value*>;
    using const_iterator = __rb_tree_iterator<Value, const Value&, const Value*>;
    using self = __rb_tree_iterator<Value, Ref, Ptr>;
    using link_type = __rb_tree_node<Value>*; // or __rb_tree_node<Value>::link_type

    __rb_tree_iterator() {}
    __rb_tree_iterator(link_type x) : node(x) {}
    __rb_tree_iterator(const iterator& it) : node(it.node) {}

    reference operator*() const
    {
        return link_type(node)->data;
    }
    pointer operator->() const
    {
        return &(operator*);
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

// rb-tree
template<typename Key, typename Value, typename KeyOfValue, typename Compare = std::less<Key>, typename Allocator = tstd::allocator<__rb_tree_node<Value>>>
class rb_tree
{
private:
    using base_ptr = __rb_tree_node_base*;
    using rb_tree_node = __rb_tree_node<Value>;
    using rb_tree_node_allocator = Allocator;
    using color_type = __rb_tree_color_type;
public:
    using key_type = Key;
    using value_type = Value;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = __rb_tree_iterator<value_type, reference, pointer>;
    using link_type = rb_tree_node*;
// data members
private:
    rb_tree_node_allocator node_alloc;
    size_type node_count;
    link_type header; // header node, whose parent point to root, left point to leftmost node, right point to rightmost node, represent end() iterator at the same time.
    Compare key_compare;
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
        node_alloc.constrcut(&p->data, std::forward<Args>(args)...);
        return p;
    }
    // clone a node, include color and value
    [[nodiscard]] link_type clone_node(link_type p)
    {
        link_type tmp = construct_node(p->data);
        tmp->color = p->color;
        tmp->left = tmp->right = nullptr;
        return tmp;
    }
    void destroy_node(link_type p)
    {
        if (p)
        {
            node_alloc.destroy(p->data);
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
    static color_type& color(link_type x)
    {
        return (color_type&)(x->color);
    }
    // get minimum and maximum
    static link_type minimum(link_type x)
    {
        return (link_type)__rb_tree_node_base::minimum(x);
    }
    static link_type maximum(link_type x)
    {
        return (link_type)__rb_tree_node_base::maximum(x);
    }
    // init an empty tree
    void init()
    {
        header = get_node();
        color(header) = __rb_tree_red;
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }
public:
    rb_tree(const Compare& comp = Compare())
        : node_count(0), key_compare(comp)
    {
        init();
    }
    ~rb_tree()
    {
        clear();
        release_node(header);
    }
    // assignment
    rb_tree& operator=(const rb_tree& other)
    {

    }
    // 
    Compare key_comp() const
    {
        return key_compare;
    }
    iterator begin()
    {
        return leftmost();
    }
    iterator end()
    {
        return header;
    }
    bool empty() const
    {
        return node_count == 0;
    }
    size_type size() const
    {
        return node_count;
    }
    size_type max_size() const
    {
        return std::numeric_limits<difference_type>::max();
    }
    // insertion
    
};

} // namespace tstd


#endif // TSTL_AVLTREE_HPP