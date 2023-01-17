#ifndef TSTL_RBTREE_HPP
#define TSTL_RBTREE_HPP

#include <functional>
#include <iterator>
#include <memory>
#include <queue>
#include <type_traits>
#include <cstddef>
#include <titerator.hpp>
#include <tstl_allocator.hpp>
#include <limits>
#include <tqueue.hpp>

namespace tstd
{
namespace impl
{
// generic rb-tree utilities
// for all associative containers: map, set, multimap, multiset

// rb-tree implementation
template<typename Key, typename Value, typename KeyOfValue, bool Multi = false, typename Compare = std::less<Key>, typename Allocator = tstd::allocator<Value>>
class rb_tree
{
/*
attributes of red-black tree:
1. every node is either red or black
2. root node is always black
3. every leave node (NIL) is black
4. if a node is red, its children will be both black
5. for every node, the path to every offspring leave nodes contains the same number of black nodes.

Every inserted node is initialized to red, so only 2 and 4 may be broken, and it's either 2 or 4, not both.
breaking of 2: if inserted node is root.
breaking of 4: if the parent of inserted node is red.

implementation detail: Let a Nil node that painted to black be root's parent and leave nodes.
*/
private:
    enum NodeColor : unsigned char{ RED, BLACK };
    struct RbTreeNode
    {
        RbTreeNode(const Value& _data) : data(_data)
        {
        }
        RbTreeNode(Value&& _data) : data(std::move(_data))
        {
        }
        template<typename... Args>
        RbTreeNode(Args&&... args) : data(std::forward<Args>(args)...)
        {
        }
        RbTreeNode* left = nullptr;
        RbTreeNode* right = nullptr;
        RbTreeNode* parent = nullptr;
        Value data;
        NodeColor color = RED;
    };

    template<typename IterValue, typename IterRef, typename IterPtr, typename TreeType>
    struct RbTreeIterator
    {
        friend class rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>;
    private:
        using iterator = RbTreeIterator<IterValue, IterValue&, IterValue*, std::remove_const_t<TreeType>>;
    public:
        RbTreeIterator(TreeType* _tree = nullptr, RbTreeNode* _node = nullptr) : tree(_tree), node(_node)
        {
        }
        RbTreeIterator(const iterator& other) : tree(other.tree), node(other.node)
        {
        }
        IterRef operator*() const
        {
            return node->data;
        }
        IterPtr operator->() const
        {
            return &(node->data);
        }
        RbTreeIterator& operator++()
        {
            node = tree->successor(node);
            return *this;
        }
        RbTreeIterator operator++(int)
        {
            RbTreeIterator res(tree, node);
            ++*this;
            return res;
        }
        RbTreeIterator& operator--()
        {
            node = tree->predecessor(node);
            return *this;
        }
        RbTreeIterator operator--(int)
        {
            RbTreeIterator res(tree, node);
            --*this;
            return res;
        }
        bool operator==(const RbTreeIterator& other) const
        {
            return tree == other.tree && node == other.node;
        }
        RbTreeIterator& operator=(const RbTreeIterator& other)
        {
            tree = other.tree;
            node = other.node;
            return *this;
        }
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Value;
        using size_type = std::size_t;
        using reference = IterRef;
        using pointer = IterPtr;
    private:
        TreeType* tree;
        RbTreeNode* node;
    };
private:
    using rb_tree_node_allocator = typename Allocator::template rebind<RbTreeNode>::other;
    using link_type = RbTreeNode*;
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
    using iterator = RbTreeIterator<value_type, reference, pointer, rb_tree>;
    using const_iterator = RbTreeIterator<value_type, const_reference, const_pointer, const rb_tree>;
    using reverse_iterator = tstd::reverse_iterator<iterator>;
    using const_reverse_iterator = tstd::reverse_iterator<const_iterator>;
    // node_type and insert_return_type (since C++17) are not supported.
// data members
private:
    allocator_type m_alloc;
    rb_tree_node_allocator m_nodeAlloc;
    Compare m_keyCompare;
    size_type m_nodeCount = 0;
    link_type m_root = nullptr;
    link_type Nil = nullptr;
private:
    // inner auxiliary functions
    // memory and construction/destruction
    [[nodiscard]] link_type getNode()
    {
        return m_nodeAlloc.allocate(1);
    }
    void releaseNode(link_type p)
    {
        m_nodeAlloc.deallocate(p, 1);
    }
    template<typename... Args>
    [[nodiscard]] link_type constructNode(Args&&... args)
    {
        link_type p = getNode();
        m_nodeAlloc.construct(&p->data, std::forward<Args>(args)...);
        p->left = p->right = p->parent = Nil;
        return p;
    }
    void destroyNode(link_type p)
    {
        if (p)
        {
            m_nodeAlloc.destroy(&p->data);
            releaseNode(p);
        }
    }
    // Nil
    void initNil()
    {
        Nil = getNode();
        Nil->left = Nil->right = Nil->parent = nullptr;
        Nil->color = BLACK;
    }
    void freeNil()
    {
        m_nodeAlloc.deallocate(Nil, 1);
    }
    // inner operations, make sure input node is not nullptr
    static Key& key(link_type p)
    {
        static KeyOfValue keyOfValue;
        return keyOfValue(p->data);
    }
    // get minimum and maximum
    link_type leftMost(link_type node) const
    {
        while (node->left != Nil)
        {
            node = node->left;
        }
        return node;
    }
    link_type rightMost(link_type node) const
    {
        while (node->right != Nil)
        {
            node = node->right;
        }
        return node;
    }
    link_type successor(link_type node) const
    {
        if (node->right != Nil)
        {
            return leftMost(node->right);
        }
        link_type par = node->parent;
        while (par != Nil && node == par->right)
        {
            node = par;
            par = par->parent;
        }
        return par; // Nil if node is the right mpost
    }
    link_type predecessor(link_type node) const
    {
        if (node == Nil)
        {
            return rightMost(m_root);
        }
        if (node->left != Nil)
        {
            return rightMost(node->left);
        }
        link_type par = node->parent;
        while (par != Nil && node == par->left)
        {
            node = par;
            par = par->parent;
        }
        return par; // Nil if node is the left most
    }
    // init an empty tree
    void initEmpty()
    {
        initNil();
        m_root = Nil;
        m_nodeCount = 0;
    }
    // copy node and its children
    void copyNode(link_type src, const rb_tree& srcTree, link_type& dest, rb_tree& destTree)
    {
        if (src != srcTree.Nil)
        {
            dest = constructNode(src->data);
            copyNode(src->left, srcTree, dest->left, destTree);
            copyNode(src->right, srcTree, dest->right, destTree);
            if (dest->left != destTree.Nil)
            {
                dest->left->parent = dest;
            }
            if (dest->right != destTree.Nil)
            {
                dest->right->parent = dest;
            }
        }
        else
        {
            dest = destTree.Nil;
        }
    }
    // copy from another tree
    void copyFrom(const rb_tree& other)
    {
        copyNode(other.m_root, other, m_root, *this);
        m_nodeCount = other.m_nodeCount;
    }
    // move from other rb_tree
    void moveFrom(rb_tree&& other)
    {
        m_root = other.m_root;
        Nil = other.Nil;
        m_nodeCount = other.m_nodeCount;
        other.initEmpty();
    }
    // move node and its children
    void moveNode(link_type src, const rb_tree& srcTree, link_type& dest, const rb_tree& destTree)
    {
        if (src != srcTree.Nil)
        {
            dest = constructNode(std::move(src->data));
            moveNode(src->left, srcTree, dest->left, destTree);
            moveNode(src->right, srcTree, dest->left, destTree);
            if (dest->left != destTree.Nil)
            {
                dest->left->parent = dest;
            }
            if (dest->right != destTree.Nil)
            {
                dest->right->parent = dest;
            }
        }
        else
        {
            dest = destTree.Nil;
        }
    }
    // move every elements from other rb_tree
    void moveElementsFrom(rb_tree&& other)
    {
        moveNode(other.m_root, other, m_root, *this);
        m_nodeCount = other.m_nodeCount;
    }
    // search
    RbTreeNode* search(RbTreeNode* node, const Key& val) const
    {
        while (node != Nil)
        {
            if (m_keyCompare(key(node), val))
            {
                node = node->right;
            }
            else if (m_keyCompare(val, key(node)))
            {
                node = node->left;
            }
            else // val == key(node)
            {
                return node;
            }
        }
        return Nil;
    }
    // rotate
    // left rotate: make sure node is not Nil and node has right child
    //  node  to   y
    //   \        /
    //    y      node
    void leftRotate(RbTreeNode* node)
    {
        RbTreeNode* y = node->right;
        node->right = y->left;
        if (y->left != Nil)
        {
            y->left->parent = node;
        }
        y->parent = node->parent;
        if (node->parent == Nil) // node is root
        {
            m_root = y;
        }
        else if (node == node->parent->left)
        {
            node->parent->left = y;
        }
        else
        {
            node->parent->right = y;
        }
        y->left = node;
        node->parent = y;
    }
    // right rotate: make sure node is not Nil and node has left child
    //  node  to  y
    //  /          \_
    // y           node
    void rightRotate(RbTreeNode* node)
    {
        RbTreeNode* y = node->left;
        node->left = y->right;
        if (y->right != Nil)
        {
            y->right->parent = node;
        }
        y->parent = node->parent;
        if (node->parent == Nil) // node is root
        {
            m_root = y;
        }
        else if (node == node->parent->left)
        {
            node->parent->left = y;
        }
        else
        {
            node->parent->right = y;
        }
        y->right = node;
        node->parent = y;
    }
    // insert a new node, return existed node with same key or the input new node
    std::pair<link_type, bool> insertNode(link_type newNode)
    {
        link_type node = m_root;
        link_type prev = Nil;
        while (node != Nil)
        {
            prev = node;
            if (m_keyCompare(key(node), key(newNode)))
            {
                node = node->right;
            }
            else if (m_keyCompare(key(newNode), key(node)))
            {
                node = node->left;
            }
            else // key(newNode) == key(node)
            {
                if constexpr (Multi)
                {
                    node = node->right;
                }
                else
                {
                    destroyNode(newNode);
                    return { node, false};
                }
            }
        }
        newNode->parent = prev;
        if (prev == Nil) // root is Nil, the tree is empty
        {
            m_root = newNode;
        }
        else if (m_keyCompare(key(newNode), key(prev)))
        {
            prev->left = newNode;
        }
        else
        {
            prev->right = newNode;
        }
        newNode->color = RED;
        newNode->left = newNode->right = Nil;
        insertFixUp(newNode);
        m_root->parent = Nil;   // insert process may change m_root, m_root->parent should keep Nil all the time.
        m_nodeCount++;
        return { newNode, true };
    }
    // fix up the attributes of Red-Black tree after inserting: make sure node is not nullptr or Nil
    void insertFixUp(RbTreeNode* node)
    {
        while (node->parent->color == RED) // root is always black, so node->parent is not root and has a parent for sure.
        {
            if (node->parent == node->parent->parent->left)
            {
                RbTreeNode* uncle = node->parent->parent->right; // node's uncle
                if (uncle->color == RED) // case 1: node's uncle is red
                {
                    // paint it's parent and uncle to black, paint it's grandparent to red, then go up to it's grandfather
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else if (node == node->parent->right) // case 2: node's uncle is black, and node is a right child
                {
                    // go up to it's parent and left rotate (possible to become case 3)
                    node = node->parent;
                    leftRotate(node);
                }
                else // case 3: node's uncle is black, and node is a left child
                {
                    // if parent is not root, paint parent to black, paint grandparent to red, then right rotate
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rightRotate(node->parent->parent);
                }
            }
            else // just change the direction
            {
                RbTreeNode* uncle = node->parent->parent->left;
                if (uncle->color == RED) // case 1
                {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else if (node == node->parent->left) // case 2
                {
                    node = node->parent;
                    rightRotate(node);
                }
                else // case 3
                {
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    leftRotate(node->parent->parent);
                }
            }
        }
        m_root->color = BLACK;
    }
    // auxiliary function: replace node with newNode, make sure node is not Nil or nullptr
    void transplant(RbTreeNode* node, RbTreeNode* newNode)
    {
        if (node->parent == Nil) // node is root
        {
            m_root = newNode;
        }
        else if (node == node->parent->left) // node is left child of it's parent
        {
            node->parent->left = newNode;
        }
        else // node is right child
        {
            node->parent->right = newNode;
        }
        newNode->parent = node->parent; // set parent even if newNode is Nil. (this is a key point !!!)
    }
    // remove specified node: make sure node is not Nil or nullptr
    RbTreeNode* removeNode(RbTreeNode* node)
    {
        RbTreeNode* ret = successor(node);
        RbTreeNode* y = node;
        RbTreeNode* x = Nil;
        NodeColor yOriginalColor = y->color;
        if (node->left == Nil) // node has no left child, (include the case of no child)
        {
            x = node->right;
            transplant(node, node->right);
        }
        else if (node->right == Nil) // node has no right child
        {
            x = node->left;
            transplant(node, node->left);
        }
        else // node has both left and right child
        {
            y = leftMost(node->right); // y has no left child for sure
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == node) // y is right child of node
            {
                x->parent = y; // set parent even if x is Nil.
            }
            else
            {
                transplant(y, y->right);
                y->right = node->right;
                y->right->parent = y;
            }
            transplant(node, y);
            y->left = node->left;
            y->left->parent = y;
            y->color = node->color;
        }
        destroyNode(node);
        if (yOriginalColor == BLACK)
        {
            // if node has 0 or 1 child, x is current node in node's original location
            // if node has 2 children, x is current node in node's successor's original location
            // x could be Nil in both two cases, in those cases, x->parent will point to it's parent (see transplant), not Nil.
            // general speaking, x is the location that break the attributes of red-black tree.
            removeFixUp(x);
        }
        m_root->parent = Nil; // delete process may change m_root, m_root->parent should keep Nil all the time (include the case m_root is Nil).
        m_nodeCount--;
        return ret;
    }
    // fix up the attributes of Red-Black tree after inserting: make sure node is not nullptr or Nil
    void removeFixUp(RbTreeNode* node)
    {
        // attribute 2, 4, 5 could be broken
        while (node != m_root && node->color == BLACK)
        {
            if (node == node->parent->left)
            {
                // w is node's sibling
                // and w is not Nil for sure, because the path from w to Nil(exclude Nil itself) definitely has at least one black node.
                RbTreeNode* w = node->parent->right;
                if (w->color == RED) // case 1
                {
                    // case 1: w is red, so w has two black non-Nil children for sure.
                    w->color = BLACK;
                    node->parent->color = RED;
                    leftRotate(node->parent);
                    w = node->parent->right; // set w to node's sibling, then become case 2,3,4
                }
                if (w->left->color == BLACK && w->right->color == BLACK) // case 2
                {
                    // case 2: w is black, and w has two black chilren
                    // change w to red is enough, and node become it's parent, node's parent break attribute 5 now.
                    w->color = RED;
                    node = node->parent;
                }
                else if (w->right->color == BLACK) // case 3
                {
                    // case 3: w is black, and w has a red left child and a black right child.
                    // convert it to case 4.
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = node->parent->right;
                }
                else // case 4
                {
                    // case 4: w is black, and w has a red right child.
                    // repaint and rotate node's parent to fulfill attribute 5, jump out loop next.
                    w->color = node->parent->color;
                    node->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(node->parent);
                    node = m_root;
                }
            }
            else // node is right child of its parent
            {
                RbTreeNode* w = node->parent->left;
                if (w->color == RED) // case 1
                {
                    w->color = BLACK;
                    node->parent->color = RED;
                    rightRotate(node->parent);
                    w = node->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) // case 2
                {
                    w->color = RED;
                    node = node->parent;
                }
                else if (w->left->color == BLACK) // case 3
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = node->parent->left;
                }
                else // case 4
                {
                    w->color = node->parent->color;
                    node->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(node->parent);
                    node = m_root;
                }
            }
        }
        node->color = BLACK;
    }
public:
    rb_tree(const Compare& comp = Compare(), const Allocator& _alloc = Allocator()) // 1
        : m_alloc(_alloc)
        , m_nodeAlloc(m_alloc)
        , m_keyCompare(comp)
        , m_nodeCount(0)
        , m_root(nullptr)
        , Nil(nullptr)
    {
        initEmpty();
    }
    rb_tree(const rb_tree& other, const Allocator& _alloc = Allocator()) // 2
        : m_alloc(_alloc)
        , m_nodeAlloc(m_alloc)
        , m_keyCompare(other.m_keyCompare)
        , m_nodeCount(0)
        , m_root(nullptr)
        , Nil(nullptr)
    {
        initEmpty();
        copyFrom(other);
    }
    rb_tree(rb_tree&& other) // 3
        : m_alloc(std::move(other.m_alloc))
        , m_nodeAlloc(m_alloc)
        , m_keyCompare(std::move(other.m_keyCompare))
        , m_nodeCount(0)
        , m_root(nullptr)
        , Nil(nullptr)
    {
        moveFrom(std::move(other));
    }
    rb_tree(rb_tree&& other, const Allocator& _alloc) // 4
        : m_alloc(_alloc)
        , m_nodeAlloc(m_alloc)
        , m_keyCompare(std::move(other.m_keyCompare))
        , m_nodeCount(0)
        , m_root(nullptr)
        , Nil(nullptr)
    {
        if (_alloc == other.get_allocator()) // move the whole tree
        {
            moveFrom(std::move(other));
        }
        else // move every element
        {
            initEmpty();
            moveElementsFrom(std::move(other));
        }
    }
    ~rb_tree()
    {
        clear();
        freeNil();
    }
    // assignment
    rb_tree& operator=(const rb_tree& other)
    {
        clear();
        freeNil();
        m_alloc = other.m_alloc;
        m_nodeAlloc = other.m_nodeAlloc;
        m_keyCompare = other.m_keyCompare;
        m_nodeCount = other.m_nodeCount;
        initEmpty();
        copyFrom(other);
        return *this;
    }
    rb_tree& operator=(rb_tree&& other)
    {
        clear();
        freeNil();
        m_alloc = std::move(other.m_alloc);
        m_nodeAlloc = std::move(other.m_nodeAlloc);
        m_keyCompare = std::move(other.m_keyCompare);
        m_nodeCount = other.m_nodeCount;
        moveFrom(std::move(other));
        return *this;
    }
    // allocator
    allocator_type get_allocator() const noexcept
    {
        return m_alloc;
    }
    // iterators
    iterator begin() noexcept
    {
        return iterator(this, m_root == Nil ? Nil : leftMost(m_root));
    }
    const_iterator begin() const noexcept
    {
        return const_iterator(this, m_root == Nil ? Nil : leftMost(m_root));
    }
    const_iterator cbegin() const noexcept
    {
        return const_iterator(this, m_root == Nil ? Nil : leftMost(m_root));
    }
    iterator end() noexcept
    {
        return iterator(this, Nil);
    }
    const_iterator end() const noexcept
    {
        return const_iterator(this, Nil);
    }
    const_iterator cend() const noexcept
    {
        return const_iterator(this, Nil);
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
        return m_nodeCount == 0;
    }
    size_type size() const noexcept
    {
        return m_nodeCount;
    }
    size_type max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }
    // modifiers
    void clear() noexcept
    {
        tstd::queue<link_type> q;
        if (m_root != Nil)
        {
            q.push(m_root);
        }
        while (!q.empty())
        {
            link_type p = q.front();
            if (p->left != Nil)
            {
                q.push(p->left);
            }
            if (p->right != Nil)
            {
                q.push(p->right);
            }
            destroyNode(p);
            q.pop();
        }
        m_nodeCount = 0;
        m_root = Nil;
    }
    std::pair<iterator, bool> insert(const value_type& value) // 1
    {
        auto res = insertNode(constructNode(value));
        return { iterator(this, res.first), res.second };
    }
    std::pair<iterator, bool> insert(value_type&& value) // 2
    {
        auto res = insertNode(constructNode(std::move(value)));
        return { iterator(this, res.first), res.second };
    }
    template<typename...Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        auto res = insertNode(constructNode(std::forward<Args>(args)...));
        return { iterator(this, res.first), res.second };
    }
    // erase, return the element after erased element
    iterator erase(const_iterator pos) // 1
    {
        return { this, removeNode(pos.node) };
    }
    iterator erase(const_iterator first, const_iterator last) // 2
    {
        iterator ret{ this, first.node };
        while (first != last)
        {
            ret = erase(first);
            first = ret;
        }
        return ret;
    }
    size_type erase(const key_type& k) // 3
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
    void swap(rb_tree& other)
    {
        tstd::swap(m_alloc, other.m_alloc);
        tstd::swap(m_nodeAlloc, other.m_nodeAlloc);
        tstd::swap(m_keyCompare, other.m_keyCompare);
        tstd::swap(m_root, other.m_root);
        tstd::swap(m_nodeCount, other.m_nodeCount);
        tstd::swap(Nil, other.Nil);
    }
    // lookup
    iterator find(const Key& k)
    {
        return { this, search(m_root, k) };
    }
    const_iterator find(const Key& k) const
    {
        return { this, search(m_root, k) };
    }
    // first node that >= k
    iterator lower_bound(const key_type& k)
    {
        link_type node = m_root;
        link_type prev = nullptr;
        while (node != Nil)
        {
            // key(node) < k
            if (m_keyCompare(key(node), k))
            {
                node = node->right;
            }
            // k(node) >= k
            else
            {
                prev = node;
                node = node->left;
            }
        }
        return prev ? iterator(this, prev) : end();
    }
    const_iterator lower_bound(const key_type& k) const
    {
        link_type node = m_root;
        link_type prev = nullptr;
        while (node != Nil)
        {
            // key(node) < k
            if (m_keyCompare(key(node), k))
            {
                node = node->right;
            }
            // k(node) >= k
            else
            {
                prev = node;
                node = node->left;
            }
        }
        return prev ? const_iterator(this, prev) : end();
    }
    // first node that > k
    iterator upper_bound(const key_type& k)
    {
        link_type node = m_root;
        link_type prev = nullptr;
        while (node != Nil)
        {
            // key(node) > k
            if (m_keyCompare(k, key(node)))
            {
                prev = node;
                node = node->left;
            }
            // key(node) <= k
            else
            {
                node = node->right;
            }
        }
        return prev ? iterator(this, prev) : end();
    }
    const_iterator upper_bound(const key_type& k) const
    {
        link_type node = m_root;
        link_type prev = nullptr;
        while (node != Nil)
        {
            // key(node) > k
            if (m_keyCompare(k, key(node)))
            {
                prev = node;
                node = node->left;
            }
            // key(node) <= k
            else
            {
                node = node->right;
            }
        }
        return prev ? const_iterator(this, prev) : end();
    }
    // observers
    Compare key_comp() const
    {
        return m_keyCompare;
    }
    Compare value_comp() const
    {
        return m_keyCompare;
    }
};

// non-member operations
// a non-standard compare function for tstd::impl::rb_tree
// equal 0 less -1 greater 1
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr int _cmp_rb_tree(const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
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
constexpr bool operator==(const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_rb_tree(lhs, rhs) == 0;
}
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr bool operator!=(const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_rb_tree(lhs, rhs) != 0;
}
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr bool operator<(const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_rb_tree(lhs, rhs) < 0;
}
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr bool operator<=(const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_rb_tree(lhs, rhs) <= 0;
}
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr bool operator>(const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_rb_tree(lhs, rhs) > 0;
}
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr bool operator>=(const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, const tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs)
{
    return _cmp_rb_tree(lhs, rhs) >= 0;
}

// global swap for tstd::impl::rb_tree
template<typename Key, typename Value, typename KeyOfValue, bool Multi, typename Compare, typename Allocator>
constexpr void swap(tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& lhs, tstd::impl::rb_tree<Key, Value, KeyOfValue, Multi, Compare, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace impl
} // namespace tstd

#endif // TSTL_RBTREE_HPP