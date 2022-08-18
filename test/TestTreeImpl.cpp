#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <iterator>
#include <tstl_bst.hpp>
#include "TestUtil.hpp"

// test of the binary search implementation
// 1. bst
// 2. rb-tree: todo yet!
// 3. avl-tree: todo yet!

template<typename T>
class identity
{
public:
    T& operator()(T& val)
    {
        return val;
    }
};

// test with template argument: <int, int, identity<int>>
template<template<typename Key, typename Value, typename KeyOfValue, bool Multi = false, typename Compare = std::less<Key>, typename Allocator = tstd::allocator<Value>> class bst>
void testTreeImpl(bool showDetails, const std::string& treeType)
{
    TestUtil util(showDetails, treeType);
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::random_shuffle(vec.begin(), vec.end());
    {
        // constructors
        // 1: default constructor
        bst<int, int, identity<int>> t1;
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
        bst<int, int, identity<int>> t2(t1);
        util.assertSequenceEqual(t1, t2);
        util.assertSorted(t2.begin(), t2.end());
        util.assertEqual(t2.size(), t1.size());
        util.assertEqual(std::distance(t2.begin(), t2.end()), t2.size());
        util.assertEqual(std::distance(t2.rbegin(), t2.rend()), t2.size());
        // 3: move constructor
        bst<int, int, identity<int>> t3(std::move(t2));
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
        bst<int, int, identity<int>> t1;
        for (auto elem : vec)
        {
            t1.insert(elem);
        }
        bst<int, int, identity<int>> t2;
        t2 = t1;
        util.assertSequenceEqual(t1, t2);
        // move assignment
        bst<int, int, identity<int>> t3;
        t3 = std::move(t2);
        util.assertSequenceEqual(t1, t3);
        util.assertEqual(t2.size(), 0);
        util.assertEqual(std::distance(t2.begin(), t2.end()), t2.size());
        util.assertEqual(std::distance(t2.rbegin(), t2.rend()), t2.size());
    }
    {
        // allocator
        bst<int, int, identity<int>> t1;
        util.assertEqual(t1.get_allocator() == tstd::allocator<int>(), true);
    }
    {
        // iterators
        bst<int, int, identity<int>> t1;
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
        const bst<int, int, identity<int>> t2(t1);
        util.assertRangeEqual(t2.begin(), t2.end(), t2.cbegin());
        util.assertRangeEqual(t2.cbegin(), t2.cend(), t2.begin());
        util.assertRangeEqual(t2.rbegin(), t2.rend(), t2.crbegin());
        util.assertRangeEqual(t2.crbegin(), t2.crend(), t2.rbegin());
    }
    {
        // size and capacity
        bst<int, int, identity<int>> t1;
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
        bst<int, int, identity<int>> t1;
        for (auto elem : vec)
        {
            t1.insert(elem);
        }
        t1.clear();
        util.assertEqual(std::distance(t1.begin(), t1.end()), 0);
        util.assertEqual(t1.size(), 0);
        util.assertEqual(t1.empty(), true);
        // insert
        for (int i = 1; i <= 101; ++i)
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
    }
    {
        // modifiers
        // erase
        // 1
        {
            bst<int, int, identity<int>> t1;
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
            bst<int, int, identity<int>> t1;
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
            bst<int, int, identity<int>> t1;
            bst<int, int, identity<int>> t2;
            for (auto elem : vec)
            {
                t1.insert(elem);
                t2.insert(elem/2);
            }
            util.assertEqual(t2.size(), 51);
            bst<int, int, identity<int>> t1c(t1);
            bst<int, int, identity<int>> t2c(t2);
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
        bst<int, int, identity<int>> t1;
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
            const bst<int, int, identity<int>> t2(t1);
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
        // comparisons
        bst<int, int, identity<int>> t1;
        bst<int, int, identity<int>> t2;
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
    {
        // observers
        bst<int, int, identity<int>> t1;
        util.assertEqual(typeid(t1.key_comp()) == typeid(std::less<int>), true);
        util.assertEqual(typeid(t1.value_comp()) == typeid(std::less<int>), true);
    }
    // other template arguments test
    // support multi or not, customized compare/key/value
    util.showFinalResult();
}

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testTreeImpl<tstd::impl::bst>(showDetails, "tstd::impl::bst");
    return 0;
}
