#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <iterator>
#include <tstl_bst.hpp>
#include <tstl_rbtree.hpp>
#include "TestUtil.hpp"

// test of the binary search tree implementation
// 1. bst
// 2. rb-tree
// 3. avl-tree: todo yet!

template<template<typename Key, typename Value, typename KeyOfValue, bool Multi = false, typename Compare = std::less<Key>, typename Allocator = tstd::allocator<Value>> class treeimpl>
void testTreeImpl(bool showDetails, const std::string& treeType);

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testTreeImpl<tstd::impl::bst>(showDetails, "tstd::impl::bst");
    testTreeImpl<tstd::impl::rb_tree>(showDetails, "tstd::impl::rb_tree");
    std::cout << std::endl;
    return 0;
}

template<typename T>
class identity
{
public:
    T& operator()(T& val)
    {
        return val;
    }
};

// get first of pair
template<typename T1, typename T2>
class FirstOfPair
{
public:
    T1& operator()(std::pair<T1, T2>& p)
    {
        return p.first;
    }
};

// compare according key, descending order
template<typename T1, typename T2>
class CmpFirstOfPair
{
public:
    bool operator()(const std::pair<T1, T2>& p1, const std::pair<T1, T2>& p2)
    {
        return std::greater<T1>()(p1.first, p2.first);
    }
};


template<template<typename Key, typename Value, typename KeyOfValue, bool Multi = false, typename Compare = std::less<Key>, typename Allocator = tstd::allocator<Value>> class treeimpl>
void testTreeImpl(bool showDetails, const std::string& treeType)
{
    TestUtil util(showDetails, treeType);
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());
    {
        // constructors
        // 1: default constructor
        treeimpl<int, int, identity<int>> t1;
        util.assertEqual(t1.size(), 0);
        util.assertEqual(std::distance(t1.begin(), t1.end()), t1.size());
        util.assertEqual(std::distance(t1.rbegin(), t1.rend()), t1.size());
        for (auto elem : vec)
        {
            t1.insert(elem);
        }
        util.assertSorted(t1.begin(), t1.end());
        util.assertSorted(t1.rbegin(), t1.rend(), std::greater<int>());
        util.assertEqual(t1.size(), 100);
        util.assertEqual(std::distance(t1.begin(), t1.end()), t1.size());
        util.assertEqual(std::distance(t1.rbegin(), t1.rend()), t1.size());
        // 2: copy constructor
        treeimpl<int, int, identity<int>> t2(t1);
        util.assertSequenceEqual(t1, t2);
        util.assertSorted(t2.begin(), t2.end());
        util.assertEqual(t2.size(), t1.size());
        util.assertEqual(std::distance(t2.begin(), t2.end()), t2.size());
        util.assertEqual(std::distance(t2.rbegin(), t2.rend()), t2.size());
        // 3: move constructor
        treeimpl<int, int, identity<int>> t3(std::move(t2));
        util.assertSequenceEqual(t1, t3);
        util.assertSorted(t3.begin(), t3.end());
        util.assertEqual(t3.size(), t1.size());
        util.assertEqual(std::distance(t3.begin(), t3.end()), t3.size());
        util.assertEqual(std::distance(t3.rbegin(), t3.rend()), t3.size());
        util.assertEqual(t2.size(), 0);
        util.assertEqual(std::distance(t2.begin(), t2.end()), t2.size());
        util.assertEqual(std::distance(t2.rbegin(), t2.rend()), t2.size());
    }
    {
        // assignment
        // copy assignment
        treeimpl<int, int, identity<int>> t1;
        for (auto elem : vec)
        {
            t1.insert(elem);
        }
        treeimpl<int, int, identity<int>> t2;
        t2 = t1;
        util.assertSequenceEqual(t1, t2);
        // move assignment
        treeimpl<int, int, identity<int>> t3;
        t3 = std::move(t2);
        util.assertSequenceEqual(t1, t3);
        util.assertEqual(t2.size(), 0);
        util.assertEqual(std::distance(t2.begin(), t2.end()), t2.size());
        util.assertEqual(std::distance(t2.rbegin(), t2.rend()), t2.size());
    }
    {
        // allocator
        treeimpl<int, int, identity<int>> t1;
        util.assertEqual(t1.get_allocator() == tstd::allocator<int>(), true);
    }
    {
        // iterators
        treeimpl<int, int, identity<int>> t1;
        for (auto elem : vec)
        {
            t1.insert(elem);
        }
        util.assertRangeEqual(t1.begin(), t1.end(), t1.cbegin());
        util.assertRangeEqual(t1.cbegin(), t1.cend(), t1.begin());
        util.assertRangeEqual(t1.rbegin(), t1.rend(), t1.crbegin());
        util.assertRangeEqual(t1.crbegin(), t1.crend(), t1.rbegin());
        // bidirectional iterator
        auto iter = t1.begin();
        util.assertEqual(*iter, 1);
        util.assertEqual(*iter++, 1);
        util.assertEqual(*iter, 2);
        util.assertEqual(*++iter, 3);
        util.assertEqual(*iter, 3);
        util.assertEqual(*--iter, 2);
        util.assertEqual(*iter, 2);
        util.assertEqual(*iter--, 2);
        util.assertEqual(*iter, 1);
        // const version
        const treeimpl<int, int, identity<int>> t2(t1);
        util.assertRangeEqual(t2.begin(), t2.end(), t2.cbegin());
        util.assertRangeEqual(t2.cbegin(), t2.cend(), t2.begin());
        util.assertRangeEqual(t2.rbegin(), t2.rend(), t2.crbegin());
        util.assertRangeEqual(t2.crbegin(), t2.crend(), t2.rbegin());
    }
    {
        // size and capacity
        treeimpl<int, int, identity<int>> t1;
        util.assertEqual(t1.size(), 0);
        util.assertEqual(t1.empty(), true);
        for (auto elem : vec)
        {
            t1.insert(elem);
        }
        util.assertEqual(t1.size(), 100);
        util.assertEqual(t1.empty(), false);
    }
    {
        // modifiers
        // clear
        treeimpl<int, int, identity<int>> t1;
        for (auto elem : vec)
        {
            t1.insert(elem);
        }
        t1.clear();
        util.assertEqual(std::distance(t1.begin(), t1.end()), 0);
        util.assertEqual(t1.size(), 0);
        util.assertEqual(t1.empty(), true);
        // insert
        for (int i = 1; i <= 100; ++i)
        {
            t1.insert(i);
        }
        for (int i = 101; i <= 200; ++i)
        {
            t1.insert(std::move(i));
        }
        util.assertEqual(t1.size(), 200);
        util.assertSorted(t1.begin(), t1.end());
        // emplace
        for (int i = 201; i <= 300; ++i)
        {
            t1.emplace(i);
            t1.emplace(std::move(i)); // duplicate key
        }
        util.assertEqual(t1.size(), 300);
        util.assertSorted(t1.begin(), t1.end());
        // return value
        auto p = t1.insert(150);
        util.assertEqual(*p.first, 150);
        util.assertEqual(p.second, false); // exist
        p = t1.insert(500);
        util.assertEqual(*p.first, 500);
        util.assertEqual(p.second, true); // inserted
    }
    {
        // modifiers
        // erase
        // 1
        {
            treeimpl<int, int, identity<int>> t1;
            for (auto elem : vec)
            {
                t1.insert(elem);
            }
            for (int i = 1; i < 100; i += 10)
            {
                auto iter = t1.erase(t1.find(i));
                util.assertEqual(*iter, i+1);
            }
            util.assertSorted(t1.begin(), t1.end());
            util.assertEqual(t1.size(), 90);
            auto iter = t1.erase(t1.begin());
            util.assertEqual(iter == t1.begin(), true);
            iter = t1.erase(tstd::prev(t1.end()));
            util.assertEqual(iter == t1.end(), true);
            util.assertEqual(t1.size(), 88);
            util.assertSorted(t1.begin(), t1.end());
        }
        // 2
        {
            treeimpl<int, int, identity<int>> t1;
            for (auto elem : vec)
            {
                t1.insert(elem);
            }
            auto iter = t1.erase(t1.find(10), t1.find(30));
            util.assertEqual(t1.size(), 80);
            util.assertSorted(t1.begin(), t1.end());
            util.assertEqual(*iter, 30);
        }
        // 3
        {
            treeimpl<int, int, identity<int>> t1;
            for (auto elem : vec)
            {
                t1.insert(elem);
            }
            for (int i = 10; i < 30; i++)
            {
                auto count = t1.erase(i);
                util.assertEqual(count, 1);
            }
            util.assertEqual(t1.size(), 80);
            util.assertSorted(t1.begin(), t1.end());
        }
        // swap
        {
            treeimpl<int, int, identity<int>> t1;
            treeimpl<int, int, identity<int>> t2;
            for (auto elem : vec)
            {
                t1.insert(elem);
                t2.insert(elem/2);
            }
            util.assertEqual(t2.size(), 51);
            treeimpl<int, int, identity<int>> t1c(t1);
            treeimpl<int, int, identity<int>> t2c(t2);
            t1.swap(t2);
            util.assertSequenceEqual(t1, t2c);
            util.assertSequenceEqual(t2, t1c);
            swap(t1, t2);
            util.assertSequenceEqual(t1, t1c);
            util.assertSequenceEqual(t2, t2c);
        }
    }
    {
        // lookup
        treeimpl<int, int, identity<int>> t1;
        for (auto elem : vec)
        {
            t1.insert(elem);
        }
        // find
        auto iter = t1.find(10);
        util.assertEqual(*iter, 10);
        iter = t1.find(0);
        util.assertEqual(iter == t1.end(), true);
        iter = t1.find(200);
        util.assertEqual(iter == t1.end(), true);
        // lower_bound, upper_bound
        auto l = t1.lower_bound(10);
        auto u = t1.upper_bound(20);
        util.assertEqual(*l, 10);
        util.assertEqual(*u, 21);
        util.assertEqual(tstd::distance(l, u), 11);

        // const version
        {
            const treeimpl<int, int, identity<int>> t2(t1);
            // find
            auto iter = t2.find(10);
            util.assertEqual(*iter, 10);
            iter = t2.find(0);
            util.assertEqual(iter == t2.end(), true);
            iter = t2.find(200);
            util.assertEqual(iter == t2.end(), true);
            // lower_bound, upper_bound
            auto l = t2.lower_bound(10);
            auto u = t2.upper_bound(20);
            util.assertEqual(*l, 10);
            util.assertEqual(*u, 21);
            util.assertEqual(tstd::distance(l, u), 11);
        }
    }
    {
        // observers
        treeimpl<int, int, identity<int>> t1;
        util.assertEqual(typeid(t1.key_comp()) == typeid(std::less<int>), true);
        util.assertEqual(typeid(t1.value_comp()) == typeid(std::less<int>), true);
    }
    {
        // comparisons
        treeimpl<int, int, identity<int>> t1;
        treeimpl<int, int, identity<int>> t2;
        for (auto elem : vec)
        {
            t1.insert(elem);
            t2.insert(elem);
        }
        util.assertEqual(t1 == t2, true);
        util.assertEqual(t1 != t2, false);
        t1.erase(10);
        util.assertEqual(t1 != t2, true);
        util.assertEqual(t1 > t2, true);
        util.assertEqual(t1 >= t2, true);
        util.assertEqual(t1 < t2, false);
        util.assertEqual(t1 <= t2, false);
    }
    // other template arguments test
    // support multi, customized compare/key/value
    using bst_map = treeimpl<const int, std::pair<const int, std::string>, FirstOfPair<const int, std::string>, true, std::greater<int>>;
    CmpFirstOfPair<const int, std::string> cmp;
    {
        // constructors
        // 1: default constructor
        bst_map m1;
        util.assertEqual(m1.size(), 0);
        util.assertEqual(std::distance(m1.begin(), m1.end()), m1.size());
        util.assertEqual(std::distance(m1.rbegin(), m1.rend()), m1.size());
        for (auto elem : vec)
        {
            m1.insert(std::make_pair(elem, std::to_string(elem)));
            m1.insert(std::make_pair(elem, std::to_string(elem) + "__2"));
        }
        util.assertEqual(m1.size(), 200);
        util.assertEqual(std::distance(m1.begin(), m1.end()), m1.size());
        util.assertEqual(std::distance(m1.rbegin(), m1.rend()), m1.size());
        // 2: copy constructor
        bst_map m2(m1);
        util.assertSequenceEqual(m1, m2);
        util.assertSorted(m2.begin(), m2.end(), cmp);
        util.assertEqual(m2.size(), m1.size());
        util.assertEqual(std::distance(m2.begin(), m2.end()), m2.size());
        util.assertEqual(std::distance(m2.rbegin(), m2.rend()), m2.size());
        // 3: move constructor
        bst_map m3(std::move(m2));
        util.assertSequenceEqual(m1, m3);
        util.assertSorted(m3.begin(), m3.end(), cmp);
        util.assertEqual(m3.size(), m1.size());
        util.assertEqual(std::distance(m3.begin(), m3.end()), m3.size());
        util.assertEqual(std::distance(m3.rbegin(), m3.rend()), m3.size());
        util.assertEqual(m2.size(), 0);
        util.assertEqual(std::distance(m2.begin(), m2.end()), m2.size());
        util.assertEqual(std::distance(m2.rbegin(), m2.rend()), m2.size());
    }
    {
        // assignment
        bst_map m1;
        for (auto elem : vec)
        {
            m1.insert(std::make_pair(elem, std::to_string(elem)));
            m1.insert(std::make_pair(elem, std::to_string(elem) + "__2"));
        }
        // copy assignment
        bst_map m2;
        m2 = m1;
        util.assertSequenceEqual(m1, m2);
        // move assignment
        bst_map m3;
        m3 = std::move(m2);
        util.assertSequenceEqual(m1, m3);
        util.assertEqual(m2.size(), 0);
        util.assertEqual(std::distance(m2.begin(), m2.end()), m2.size());
        util.assertEqual(std::distance(m2.rbegin(), m2.rend()), m2.size());
    }
    {
        // allocator
        bst_map m1;
        util.assertEqual(m1.get_allocator() == tstd::allocator<int>(), true);
    }
    {
        // iterators
        bst_map m1;
        for (auto elem : vec)
        {
            m1.insert(std::make_pair(elem, std::to_string(elem)));
            m1.insert(std::make_pair(elem, std::to_string(elem) + "__2"));
        }
        util.assertRangeEqual(m1.begin(), m1.end(), m1.cbegin());
        util.assertRangeEqual(m1.cbegin(), m1.cend(), m1.begin());
        util.assertRangeEqual(m1.rbegin(), m1.rend(), m1.crbegin());
        util.assertRangeEqual(m1.crbegin(), m1.crend(), m1.rbegin());
    }
    {
        // size and capacity
        bst_map m1;
        util.assertEqual(m1.size(), 0);
        util.assertEqual(m1.empty(), true);
        for (auto elem : vec)
        {
            m1.insert(std::make_pair(elem, std::to_string(elem)));
            m1.insert(std::make_pair(elem, std::to_string(elem) + "__2"));
        }
        util.assertEqual(m1.size(), 200);
        util.assertEqual(m1.empty(), false);
    }
    {
        // modifiers
        // clear
        bst_map m1;
        for (auto elem : vec)
        {
            m1.insert(std::make_pair(elem, std::to_string(elem)));
            m1.insert(std::make_pair(elem, std::to_string(elem) + "__2"));
        }
        m1.clear();
        util.assertEqual(std::distance(m1.begin(), m1.end()), 0);
        util.assertEqual(m1.size(), 0);
        util.assertEqual(m1.empty(), true);
        // insert
        for (int i = 1; i <= 100; ++i)
        {
            auto p = std::make_pair(i, std::to_string(i));
            m1.insert(p);
        }
        for (int i = 101; i <= 200; ++i)
        {
            m1.insert(std::make_pair(i, std::to_string(i)));
        }
        util.assertEqual(m1.size(), 200);
        util.assertSorted(m1.begin(), m1.end(), cmp);
        // emplace
        for (int i = 201; i <= 300; ++i)
        {
            auto p = std::make_pair(i, std::to_string(i));
            m1.emplace(p);
            m1.emplace(std::move(p)); // duplicate key
        }
        util.assertEqual(m1.size(), 400); // Multi == true, so 400
        util.assertSorted(m1.begin(), m1.end(), cmp);
        // return value
        auto p = m1.insert(std::make_pair(150, std::string("150")));
        util.assertEqual((*p.first).first, 150);
        util.assertEqual(p.second, true); // always true
    }
    {
        // modifiers
        // erase
        // 1
        {
            bst_map m1;
            for (auto elem : vec)
            {
                m1.insert(std::make_pair(elem, std::to_string(elem)));
                m1.insert(std::make_pair(elem, std::to_string(elem) + "__2"));
            }
            for (int i = 1; i < 100; i += 10)
            {
                auto iter = m1.erase(m1.find(i));
            }
            util.assertSorted(m1.begin(), m1.end(), cmp);
            util.assertEqual(m1.size(), 190);
            auto iter = m1.erase(m1.begin());
            util.assertEqual(iter == m1.begin(), true);
            iter = m1.erase(tstd::prev(m1.end()));
            util.assertEqual(iter == m1.end(), true);
            util.assertEqual(m1.size(), 188);
            util.assertSorted(m1.begin(), m1.end(), cmp);
        }
        // 2
        {
            bst_map m1;
            for (auto elem : vec)
            {
                m1.insert(std::make_pair(elem, std::to_string(elem)));
                m1.insert(std::make_pair(elem, std::to_string(elem) + "__2"));
            }
            // in descending order
            auto iter = m1.erase(m1.lower_bound(30), m1.lower_bound(10));
            util.assertEqual(m1.size(), 160);
            util.assertSorted(m1.begin(), m1.end(), cmp);
            util.assertEqual((*iter).first, 10);
        }
        // 3
        {
            bst_map m1;
            for (auto elem : vec)
            {
                m1.insert(std::make_pair(elem, std::to_string(elem)));
                m1.insert(std::make_pair(elem, std::to_string(elem) + "__2"));
            }
            for (int i = 10; i < 30; i++)
            {
                auto count = m1.erase(i);
                util.assertEqual(count, 2);
            }
            util.assertEqual(m1.size(), 160);
            util.assertSorted(m1.begin(), m1.end(), cmp);
        }
        // swap
        {
            bst_map m1;
            bst_map m2;
            for (auto elem : vec)
            {
                m1.insert(std::make_pair(elem, std::to_string(elem)));
                m1.insert(std::make_pair(elem, std::to_string(elem) + "__2"));
                m2.insert(std::make_pair(elem, std::to_string(elem)));
            }
            util.assertEqual(m2.size(), 100);
            bst_map m1c(m1);
            bst_map m2c(m2);
            m1.swap(m2);
            util.assertSequenceEqual(m1, m2c);
            util.assertSequenceEqual(m2, m1c);
            swap(m1, m2);
            util.assertSequenceEqual(m1, m1c);
            util.assertSequenceEqual(m2, m2c);
        }
    }
    {
        // lookup
        bst_map m1;
        for (auto elem : vec)
        {
            m1.insert(std::make_pair(elem, std::to_string(elem)));
            m1.insert(std::make_pair(elem, std::to_string(elem) + "__2"));
        }
        // find
        auto iter = m1.find(10);
        util.assertEqual((*iter).first, 10);
        iter = m1.find(0);
        util.assertEqual(iter == m1.end(), true);
        iter = m1.find(200);
        util.assertEqual(iter == m1.end(), true);
        // lower_bound, upper_bound
        // m1 in descending order
        auto l = m1.lower_bound(20);
        auto u = m1.upper_bound(10);
        util.assertEqual((*l).first, 20);
        util.assertEqual((*u).first, 9);
        util.assertEqual(tstd::distance(l, u), 22); // double of 11(10 to 20 both included)

        // const version
        {
            const bst_map m2(m1);
            // find
            auto iter = m2.find(10);
            util.assertEqual((*iter).first, 10);
            iter = m2.find(0);
            util.assertEqual(iter == m2.end(), true);
            iter = m2.find(200);
            util.assertEqual(iter == m2.end(), true);
            // lower_bound, upper_bound
            // m1 in descending order
            auto l = m1.lower_bound(20);
            auto u = m1.upper_bound(10);
            util.assertEqual((*l).first, 20);
            util.assertEqual((*u).first, 9);
            util.assertEqual(tstd::distance(l, u), 22); // double of 11(10 to 20 both included)
        }
    }
    {
        // comparisons
        bst_map m1;
        bst_map m2;
        for (auto elem : vec)
        {
            m1.insert(std::make_pair(elem, std::to_string(elem)));
            m2.insert(std::make_pair(elem, std::to_string(elem)));
        }
        util.assertEqual(m1 == m2, true);
        util.assertEqual(m1 != m2, false);
        m1.erase(10);
        util.assertEqual(m1 != m2, true);
        // descending order
        util.assertEqual(m1 < m2, true);
        util.assertEqual(m1 <= m2, true);
        util.assertEqual(m1 > m2, false);
        util.assertEqual(m1 >= m2, false);
    }
    util.showFinalResult();
}
