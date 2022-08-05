#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <iterator>
#include <algorithm>
#include <functional>
#include <tmemory.hpp>
#include <tvector.hpp>
#include <tarray.hpp>
#include <tlist.hpp>
#include "TestUtil.hpp"

void testVector(bool showDetails)
{
    TestUtil util(showDetails, "vector");
    // constructors
    {
        tstd::vector<int> vec1;
        std::vector<int> vec2;
        util.assertSequenceEqual(vec1, vec2);
    }
    {
        tstd::allocator<int> alloc;
        tstd::vector<int> vec1(10, 99, alloc);
        std::vector<int, tstd::allocator<int>> vec2(10, 99, alloc);
        util.assertSequenceEqual(vec1, vec2);
    }
    {
        tstd::allocator<int> alloc;
        tstd::vector<int> vec1(10);
        std::vector<int, tstd::allocator<int>> vec2(10);
        util.assertSequenceEqual(vec1, vec2);
    }
    {
        tstd::allocator<int> alloc;
        tstd::vector<int> vec1(10, alloc);
        std::vector<int, tstd::allocator<int>> vec2(10, alloc);
        util.assertSequenceEqual(vec1, vec2);
    }
    {
        std::vector<int> vec(100);
        std::iota(vec.begin(), vec.end(), 1);
        std::random_shuffle(vec.begin(), vec.end());
        tstd::vector<int> vec1(vec.begin(), vec.end());
        std::vector<int> vec2(vec.begin(), vec.end());
        util.assertSequenceEqual(vec1, vec2);
        // copy constuctor
        tstd::vector<int> cvec1(vec1);
        std::vector<int> cvec2(vec2);
        util.assertSequenceEqual(cvec1, cvec2);
        tstd::vector<int> cvec3(cvec1, tstd::allocator<int>());
        std::vector<int> cvec4(cvec2, std::allocator<int>());
        util.assertSequenceEqual(cvec3, cvec4);
        // move constructor
        tstd::vector mvec1(std::move(cvec1));
        std::vector mvec2(std::move(cvec2));
        util.assertSequenceEqual(mvec1, mvec2);
        tstd::vector<int> mvec3(std::move(mvec1), tstd::allocator<int>());
        std::vector<int> mvec4(std::move(mvec2), std::allocator<int>());
        util.assertSequenceEqual(mvec3, mvec4);
    }
    {
        tstd::vector<int> vec1{1, 2, 3, 4};
        std::vector<int> vec2{1, 2, 3, 4};
        util.assertSequenceEqual(vec1, vec2);
        {
            // operator= / assign
            tstd::vector<int> vec1c{1, 2, 3, 100, 99999};
            std::vector<int> vec2c{1, 2, 3, 100, 99999};
            vec1 = vec1c;
            vec2 = vec2c;
            util.assertSequenceEqual(vec1, vec2);
            vec1 = std::move(vec1c);
            vec2 = std::move(vec2c);
            util.assertSequenceEqual(vec1, vec2);
            vec1 = {1, 2, 3, 4, 5, 10};
            vec2 = {1, 2, 3, 4, 5, 10};
            util.assertSequenceEqual(vec1, vec2);
            vec1.assign(10, 99);
            vec2.assign(10, 99);
            util.assertSequenceEqual(vec1, vec2);
            vec1.assign(vec1c.begin(), vec1c.end());
            vec2.assign(vec2c.begin(), vec2c.end());
            util.assertSequenceEqual(vec1, vec2);
            vec1.assign({1, 2, 3, 4, 5, 10});
            vec2.assign({1, 2, 3, 4, 5, 10});
            util.assertSequenceEqual(vec1, vec2);
        }
        // elements access
        util.assertEqual(vec1.at(0), vec2.at(0));
        util.assertEqual(vec1.at(1), vec2.at(1));
        util.assertEqual(vec1.at(2), vec2.at(2));
        util.assertEqual(vec1.at(5), vec2.at(5));
        util.assertEqual(vec1[0], vec2[0]);
        util.assertEqual(vec1[1], vec2[1]);
        util.assertEqual(vec1[2], vec2[2]);
        util.assertEqual(vec1[5], vec2[5]);
        util.assertEqual(vec1.front(), vec2.front());
        util.assertEqual(vec1.back(), vec2.back());
        vec1.back() = vec2.back() = 9999;
        vec1.front() = vec2.front() = -9999;
        util.assertSequenceEqual(vec1, vec2);
        util.assertArrayEqual(vec1.data(), vec2.data(), vec1.size());
        // iterators
        util.assertRangeEqual(vec1.begin(), vec1.end(), vec2.begin());
        util.assertRangeEqual(vec2.begin(), vec2.end(), vec1.begin());
        util.assertRangeEqual(vec1.cbegin(), vec1.cend(), vec2.cbegin());
        util.assertRangeEqual(vec1.rbegin(), vec1.rend(), vec1.rbegin());
        util.assertRangeEqual(vec1.crbegin(), vec1.crend(), vec2.crbegin());
        util.assertRangeEqual(vec2.crbegin(), vec2.crend(), vec1.crbegin());
        // size and capacity
        util.assertEqual(vec1.empty(), vec2.empty());
        util.assertEqual(vec1.size(), vec2.size());
        {
            tstd::vector<int> vec1;
            std::vector<int> vec2;
            vec1.reserve(100);
            vec2.reserve(100);
            util.assertEqual(vec1.capacity(), vec2.capacity());
            util.assertEqual(vec1.capacity(), 100);
        }
        // modifiers
        vec1.clear();
        vec2.clear();
        util.assertEqual(vec1.empty(), vec2.empty());
    }
    {
        // modifiers again
        // insert
        tstd::vector<int> vec1{1, 2, 3, 4};
        std::vector<int> vec2{1, 2, 3, 4};
        vec1.insert(vec1.begin() + 2, 10);
        vec2.insert(vec2.begin() + 2, 10);
        util.assertSequenceEqual(vec1, vec2);
        vec1.insert(vec1.begin(), 100, 9999);
        vec2.insert(vec2.begin(), 100, 9999);
        util.assertSequenceEqual(vec1, vec2);
        {
            std::vector<int> tmp(100);
            std::iota(tmp.begin(), tmp.end(), 1);
            std::random_shuffle(tmp.begin(), tmp.end());
            vec1.insert(vec1.begin() + 33, tmp.begin(), tmp.end());
            vec2.insert(vec2.begin() + 33, tmp.begin(), tmp.end());
            util.assertSequenceEqual(vec1, vec2);
        }
        // emplace
        vec1.emplace(vec1.begin() + 100, -1024);
        vec2.emplace(vec2.begin() + 100, -1024);
        util.assertSequenceEqual(vec1, vec2);
        // erase
        vec1.erase(vec1.begin());
        vec2.erase(vec2.begin());
        util.assertSequenceEqual(vec1, vec2);
        vec1.erase(vec1.begin() + 100, vec1.begin() + 150);
        vec2.erase(vec2.begin() + 100, vec2.begin() + 150);
        util.assertSequenceEqual(vec1, vec2);
        // push_back
        vec1.push_back(1);
        vec1.push_back(100);
        vec1.push_back(1001);
        vec2.push_back(1);
        vec2.push_back(100);
        vec2.push_back(1001);
        util.assertSequenceEqual(vec1, vec2);
        // emplace_back
        vec1.emplace_back(1);
        vec1.emplace_back(100);
        vec1.emplace_back(1001);
        vec2.emplace_back(1);
        vec2.emplace_back(100);
        vec2.emplace_back(1001);
        util.assertSequenceEqual(vec1, vec2);
        // pop_back
        for (int i = 0; i < 20; ++i)
        {
            vec1.pop_back();
            vec2.pop_back();
        }
        util.assertSequenceEqual(vec1, vec2);
        // resize
        vec1.resize(vec1.size() / 2);
        vec2.resize(vec2.size() / 2);
        util.assertSequenceEqual(vec1, vec2);
        vec1.resize(200, 9999);
        vec2.resize(200, 9999);
        util.assertSequenceEqual(vec1, vec2);
        vec1.resize(5000, -9999);
        vec2.resize(5000, -9999);
        util.assertSequenceEqual(vec1, vec2);
        {
            // swap
            tstd::vector<int> tmp1{1, 2, 3};
            std::vector<int> tmp2{1, 2, 3};
            vec1.swap(tmp1);
            vec2.swap(tmp2);
            util.assertSequenceEqual(vec1, vec2);
            util.assertSequenceEqual(tmp1, tmp2);
            swap(vec1, tmp1);
            swap(vec2, tmp2);
            util.assertSequenceEqual(vec1, vec2);
            util.assertSequenceEqual(tmp1, tmp2);
        }
    }
    {
        // comparison
        tstd::vector<int> vec1{1, 2, 3, 4};
        tstd::vector<int> vec2{1, 2, 3, 4};
        util.assertSequenceEqual(vec1, vec2);
        util.assertEqual(vec1 == vec2, true);
        util.assertNotEqual(vec1 != vec2, true);
        vec2.push_back(5);
        util.assertEqual(vec1 < vec2, true);
        util.assertEqual(vec1 <= vec2, true);
        util.assertEqual(vec2 > vec1, true);
        util.assertEqual(vec2 >= vec2, true);
        vec1.push_back(6);
        util.assertEqual(vec1 > vec2, true);
        util.assertEqual(vec1 >= vec2, true);
        util.assertEqual(vec1 < vec2, false);
        util.assertEqual(vec1 <= vec2, false);
    }
    util.showFinalResult();
}

void testArray(bool showDetails)
{
    TestUtil util(showDetails, "array");
    {
        // implicit constructors
        tstd::array<int, 10> arr1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::array<int, 10> arr2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        util.assertSequenceEqual(arr1, arr2);
        arr1 = {-1, -2, -3};
        arr2 = {-1, -2, -3};
        util.assertSequenceEqual(arr1, arr2);
        tstd::array<int, 10> arr1c = {1, 2, 3};
        std::array<int, 10> arr2c = {1, 2, 3};
        arr1 = arr1c;
        arr2 = arr2c;
        util.assertSequenceEqual(arr1, arr2);
        {
            tstd::array<int, 10> arr1(arr1c);
            std::array<int, 10> arr2(arr2c);
            util.assertSequenceEqual(arr1, arr2);
        }
    }
    {
        // elements access
        tstd::array<int, 10> arr1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::array<int, 10> arr2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        arr1.at(0) = 10;
        arr2.at(0) = 10;
        arr1[9] = 99;
        arr2[9] = 99;
        util.assertSequenceEqual(arr1, arr2);
        arr1.front() = -1;
        arr2.front() = -1;
        arr1.back() = -10;
        arr2.back() = -10;
        util.assertSequenceEqual(arr1, arr2);
        // iterators
        util.assertRangeEqual(arr1.begin(), arr1.end(), arr2.begin());
        util.assertRangeEqual(arr1.cbegin(), arr1.cend(), arr2.cbegin());
        util.assertRangeEqual(arr1.rbegin(), arr1.rend(), arr2.rbegin());
        util.assertRangeEqual(arr1.crbegin(), arr1.crend(), arr2.crbegin());
        *arr1.begin() = 1000;
        *arr2.begin() = 1000;
        util.assertSequenceEqual(arr1, arr2);
        util.assertEqual(arr1.empty(), arr2.empty());
        util.assertEqual(arr1.empty(), false);
        util.assertEqual(arr1.size(), arr2.size());
        util.assertEqual(arr1.size(), 10);
        util.assertEqual(arr1.max_size(), arr2.max_size());
        util.assertEqual(arr1.max_size(), 10);
        // operations
        arr1.fill(0);
        arr2.fill(0);
        util.assertSequenceEqual(arr1, arr2);
        {
            tstd::array<int, 10> arr3{1, 2, 3};
            std::array<int, 10> arr4{1, 2, 3};
            arr1.swap(arr3);
            arr2.swap(arr4);
            util.assertSequenceEqual(arr1, arr2);
            util.assertSequenceEqual(arr3, arr4);
        }
    }
    {
        // non-member operations
        tstd::array<int, 3> arr1{1, 2, 3};
        tstd::array<int, 3> arr2{1, 2, 4};
        util.assertEqual(arr1 == arr2, false);
        util.assertEqual(arr1 != arr2, true);
        util.assertEqual(arr1 < arr2, true);
        util.assertEqual(arr1 <= arr2, true);
        util.assertEqual(arr1 > arr2, false);
        util.assertEqual(arr1 >= arr2, false);
        util.assertEqual(arr1[0], tstd::get<0>(arr1));
        auto arr3(arr1);
        auto arr4(arr2);
        swap(arr1, arr2);
        util.assertSequenceEqual(arr1, arr4);
        util.assertSequenceEqual(arr2, arr3);
        {
            // to_array
            int A[3] = {1, 2, 3};
            auto arr1 = tstd::to_array(A);
            auto arr2 = std::to_array(A);
            util.assertSequenceEqual(arr1, arr2);
        }
    }
    util.showFinalResult();
}

void testList(bool showDetails)
{
    TestUtil util(showDetails, "list");
    {
        // constructors
        {
            tstd::list<int> li;
            util.assertEqual(li.empty(), true);
            util.assertEqual(li.begin() == li.end(), true);
            util.assertEqual(li.size(), 0);
        }
        {
            tstd::list<int> li((tstd::allocator<int>()));
            util.assertEqual(li.empty(), true);
            util.assertEqual(li.begin() == li.end(), true);
            util.assertEqual(li.size(), 0);
        }
        {
            tstd::list<int> list1(2, 99); // can not use std::allocator for tstd::list<int> in C++20 because of compatibility reasons
            std::list<int> list2(2, 99);
            util.assertSequenceEqual(list1, list2);
        }
        {
            tstd::list<int> list1(10, 99, tstd::allocator<int>());
            std::list<int> list2(10, 99, std::allocator<int>());
            util.assertSequenceEqual(list1, list2);
        }
        {
            tstd::list<int> list1(100);
            std::list<int> list2(100);
            util.assertSequenceEqual(list1, list2);
        }
        {
            std::vector<int> vec(100);
            std::iota(vec.begin(), vec.end(), 1);
            std::random_shuffle(vec.begin(), vec.end());
            tstd::list<int> list1(vec.begin(), vec.end());
            std::list<int> list2(vec.begin(), vec.end());
            util.assertSequenceEqual(list1, list2);
        }
        {
            std::vector<int> vec(100);
            std::iota(vec.begin(), vec.end(), 1);
            std::random_shuffle(vec.begin(), vec.end());
            tstd::list<int> tmp1(vec.begin(), vec.end());
            std::list<int> tmp2(vec.begin(), vec.end());
            {
                tstd::list<int> list1(tmp1);
                std::list<int> list2(tmp2);
                util.assertSequenceEqual(list1, list2);
            }
            {
                tstd::list<int> list1(tmp1, tstd::allocator<int>());
                std::list<int> list2(tmp2, std::allocator<int>());
                util.assertSequenceEqual(list1, list2);
            }
            {
                // move constructors
                tstd::list<int> list1(std::move(tmp1));
                std::list<int> list2(std::move(tmp2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tstd::list<int> list3(std::move(list1), tstd::allocator<int>());
                std::list<int> list4(std::move(list2), std::allocator<int>());
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(list3, list4);
            }
            {
                tstd::list<int> list1{1, 2, 3, 4, 5};
                std::list<int> list2{1, 2, 3, 4, 5};
                util.assertSequenceEqual(list1, list2);
            }
        }
    }
    {
        std::vector<int> vec(1000);
        std::iota(vec.begin(), vec.end(), 1);
        std::random_shuffle(vec.begin(), vec.end());

        // assignment: operator=/assign
        tstd::list<int> list1;
        std::list<int> list2;
        tstd::list<int> tmp1(vec.begin(), vec.end());
        std::list<int> tmp2(vec.begin(), vec.end());
        list1 = tmp1;
        list2 = tmp2;
        util.assertSequenceEqual(list1, list2);
        list1 = std::move(tmp1);
        list2 = std::move(tmp2);
        util.assertSequenceEqual(list1, list2);
        util.assertSequenceEqual(tmp1, tmp2);
        list1.assign(100, 9999);
        list2.assign(100, 9999);
        util.assertSequenceEqual(list1, list2);
        list1.assign(vec.begin(), vec.end());
        list2.assign(vec.begin(), vec.end());
        util.assertSequenceEqual(list1, list2);
        list1.assign({1, 2, 3, 4, 5, 100});
        list2.assign({1, 2, 3, 4, 5, 100});
        util.assertSequenceEqual(list1, list2);
    }
    {
        // allocator
        tstd::list<int> tmp;
        util.assertEqual(tstd::allocator<int>() == tmp.get_allocator(), true);
    }
    {
        // element access
        tstd::list<int> list1{1, 2, 3, 4, 5};
        std::list<int> list2{1, 2, 3, 4, 5};
        util.assertEqual(list1.front(), list2.front());
        util.assertEqual(list1.front(), 1);
        list1.front() = 10;
        list2.front() = 10;
        util.assertEqual(list1.front(), list2.front());
        util.assertEqual(list1.front(), 10);
        util.assertEqual(list1.back(), list2.back());
        util.assertEqual(list1.back(), 5);
        list1.back() = 99;
        list2.back() = 99;
        util.assertEqual(list1.back(), list2.back());
        util.assertEqual(list1.back(), 99);
    }
    {
        // iterators
        tstd::list<int> list1{1, 2, 3, 4, 5};
        std::list<int> list2{1, 2, 3, 4, 5};
        util.assertEqual(*list2.begin(), *list1.begin());
        util.assertEqual(*list2.cbegin(), *list1.cbegin());
        util.assertEqual(*tstd::prev(list1.end()), *std::prev(list2.end()));
        util.assertEqual(*tstd::prev(list1.cend()), *std::prev(list2.cend()));
        util.assertEqual(*list2.rbegin(), *list1.rbegin());
        util.assertEqual(*list2.crbegin(), *list1.crbegin());
        util.assertEqual(*tstd::prev(list1.rend()), *std::prev(list2.rend()));
        util.assertEqual(*tstd::prev(list1.crend()), *std::prev(list2.crend()));
    }
    {
        // size and capacity
        tstd::list<int> list1{1, 2, 3, 4, 5};
        std::list<int> list2{1, 2, 3, 4, 5};
        util.assertEqual(list1.empty(), list2.empty());
        util.assertEqual(list1.size(), list2.size());

        // modifiers
        list1.clear();
        list2.clear();
        util.assertEqual(list1.empty(), true);
        util.assertEqual(list1.empty(), list2.empty());
    }
    {
        std::vector<int> vec(1000);
        std::iota(vec.begin(), vec.end(), 1);
        std::random_shuffle(vec.begin(), vec.end());

        // modifiers
        tstd::list<int> list1{1, 2, 3, 4, 5};
        std::list<int> list2{1, 2, 3, 4, 5};
        // insert
        int a = 100;
        list1.insert(list1.begin(), a);
        list2.insert(list2.begin(), a);
        list1.insert(std::next(list1.begin(), 2), 99);
        list2.insert(std::next(list2.begin(), 2), 99);
        list1.insert(list1.end(), 42);
        list2.insert(list2.end(), 42);
        util.assertSequenceEqual(list1, list2);
        list1.insert(tstd::next(list1.begin(), 5), 10, 1024);
        list2.insert(tstd::next(list2.begin(), 5), 10, 1024);
        util.assertSequenceEqual(list1, list2);
        list1.insert(tstd::next(list1.begin(), 5), vec.begin(), vec.end());
        list2.insert(tstd::next(list2.begin(), 5), vec.begin(), vec.end());
        util.assertSequenceEqual(list1, list2);
        list1.insert(tstd::next(list1.begin(), 5), {1, 2, 3, 4, 5});
        list2.insert(tstd::next(list2.begin(), 5), {1, 2, 3, 4, 5});
        // emplace
        list1.emplace(tstd::next(list1.begin(), 5), 2048);
        list2.emplace(tstd::next(list2.begin(), 5), 2048);
        util.assertSequenceEqual(list1, list2);
        // erase
        list1.erase(list1.begin());
        list2.erase(list2.begin());
        list1.erase(tstd::next(list1.begin(), 10), tstd::next(list1.begin(), 100));
        list2.erase(tstd::next(list2.begin(), 10), tstd::next(list2.begin(), 100));
        util.assertSequenceEqual(list1, list2);
        // push_back
        list1.push_back(a);
        list2.push_back(a);
        list1.push_back(100);
        list2.push_back(100);
        util.assertSequenceEqual(list1, list2);
        // emplace_back
        list1.emplace_back(a);
        list2.emplace_back(a);
        auto res1 = list1.emplace_back(100);
        auto res2 = list2.emplace_back(100);
        util.assertSequenceEqual(list1, list2);
        util.assertEqual(res1, res2);
        // pop_back
        for (int i = 0; i < 10; ++i)
        {
            list1.pop_back();
            list2.pop_back();
        }
        util.assertSequenceEqual(list1, list2);
        // push_front
        for (int i = 0; i < 100; ++i)
        {
            list1.push_front(1000001);
            list2.push_front(1000001);
        }
        util.assertSequenceEqual(list1, list2);
        // emplace_front
        for (int i = 0; i < 3; ++i)
        {
            auto res1 = list1.emplace_front(a);
            auto res2 = list2.emplace_front(a);
            res1 = 10;
            res2 = 10;
            list1.emplace_front(0);
            list2.emplace_front(0);
        }
        util.assertSequenceEqual(list1, list2);
        // pop_front
        for (int i = 0; i < 100; ++i)
        {
            list1.pop_front();
            list2.pop_front();
        }
        util.assertSequenceEqual(list1, list2);
        // resize
        list1.resize(100);
        list2.resize(100);
        util.assertSequenceEqual(list1, list2);
        list1.resize(1000, 1024);
        list2.resize(1000, 1024);
        util.assertSequenceEqual(list1, list2);
        // swap
        {
            tstd::list<int> tmp1{1, 2, 3, 4, 5};
            std::list<int> tmp2{1, 2, 3, 4, 5};
            list1.swap(tmp1);
            list2.swap(tmp2);
            util.assertSequenceEqual(list1, list2);
            util.assertSequenceEqual(tmp1, tmp2);
            tstd::swap(list1, tmp1);
            std::swap(list2, tmp2);
            util.assertSequenceEqual(list1, list2);
            util.assertSequenceEqual(tmp1, tmp2);
            // case of empty list
            tmp1.clear();
            tmp2.clear();
            list1.swap(tmp1);
            list2.swap(tmp2);
            util.assertSequenceEqual(list1, list2);
            util.assertSequenceEqual(tmp1, tmp2);
            swap(tmp1, list1);
            swap(tmp2, list2);
            util.assertSequenceEqual(list1, list2);
            util.assertSequenceEqual(tmp1, tmp2);
        }
    }
    {
        std::vector<int> vec(1000);
        std::iota(vec.begin(), vec.end(), 1);
        std::random_shuffle(vec.begin(), vec.end());

        // oeprations/algorithms
        // merge
        {
            tstd::list<int> list1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            std::list<int> list2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            {
                tstd::list<int> tmp1{1, 2, 3, 4, 5};
                std::list<int> tmp2{1, 2, 3, 4, 5};
                list1.merge(tmp1);
                list2.merge(tmp2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            {
                tstd::list<int> tmp1{1, 2, 3, 4, 5};
                std::list<int> tmp2{1, 2, 3, 4, 5};
                list1.merge(std::move(tmp1));
                list2.merge(std::move(tmp2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            {
                // one of the lists is empty
                tstd::list<int> tmp1;
                std::list<int> tmp2;
                list1.merge(tmp1);
                list2.merge(tmp2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tmp1.merge(list1);
                tmp2.merge(list2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
        }
        // splice
        {
            {
                tstd::list<int> list1(vec.begin(), vec.end());
                std::list<int> list2(vec.begin(), vec.end());
                tstd::list<int> tmp1{1, 2, 3, 4, 5};
                std::list<int> tmp2{1, 2, 3, 4, 5};
                list1.splice(std::next(list1.begin(), 20), tmp1);
                list2.splice(std::next(list2.begin(), 20), tmp2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                list1.splice(std::next(list1.begin(), 20), tmp1);
                list2.splice(std::next(list2.begin(), 20), tmp2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tmp1.splice(tmp1.begin(), list1);
                tmp2.splice(tmp2.begin(), list2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tmp1.clear();
                tmp2.clear();
                list1.splice(list1.begin(), tmp1);
                list2.splice(list2.begin(), tmp2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            {
                tstd::list<int> list1(vec.begin(), vec.end());
                std::list<int> list2(vec.begin(), vec.end());
                tstd::list<int> tmp1{1, 2, 3, 4, 5};
                std::list<int> tmp2{1, 2, 3, 4, 5};
                list1.splice(std::next(list1.begin(), 20), std::move(tmp1));
                list2.splice(std::next(list2.begin(), 20), std::move(tmp2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                list1.splice(std::next(list1.begin(), 20), std::move(tmp1));
                list2.splice(std::next(list2.begin(), 20), std::move(tmp2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tmp1.splice(tmp1.begin(), std::move(list1));
                tmp2.splice(tmp2.begin(), std::move(list2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tmp1.clear();
                tmp2.clear();
                list1.splice(list1.begin(), std::move(tmp1));
                list2.splice(list2.begin(), std::move(tmp2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            {
                tstd::list<int> list1(vec.begin(), vec.end());
                std::list<int> list2(vec.begin(), vec.end());
                tstd::list<int> tmp1{1, 2, 3, 4, 5};
                std::list<int> tmp2{1, 2, 3, 4, 5};
                list1.splice(tstd::next(list1.begin(), 20), tmp1, tstd::next(tmp1.begin(), 2));
                list2.splice(tstd::next(list2.begin(), 20), tmp2, tstd::next(tmp2.begin(), 2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                list1.splice(tstd::next(list1.begin(), 20), std::move(tmp1), tstd::next(tmp1.begin(), 2));
                list2.splice(tstd::next(list2.begin(), 20), std::move(tmp2), tstd::next(tmp2.begin(), 2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                list1.splice(tstd::next(list1.begin(), 20), tmp1, tmp1.begin(), tmp1.end());
                list2.splice(tstd::next(list2.begin(), 20), tmp2, tmp2.begin(), tmp2.end());
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                list1.splice(tstd::next(list1.begin(), 20), std::move(tmp1), tmp1.begin(), tmp1.end());
                list2.splice(tstd::next(list2.begin(), 20), std::move(tmp2), tmp2.begin(), tmp2.end());
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
        }
        // remove, remove_if
        {
            tstd::list<int> list1(vec.begin(), vec.end());
            std::list<int> list2(vec.begin(), vec.end());
            for (int i = 10; i < 20; ++i)
            {
                auto res1 = list1.remove(i);
                auto res2 = list2.remove(i);
                util.assertEqual(res1, res2);
            }
            util.assertSequenceEqual(list1, list2);
            auto res1 = list1.remove_if([](int a) -> bool { return a >= 50; });
            auto res2 = list2.remove_if([](int a) -> bool { return a >= 50; });
            util.assertEqual(res1, res2);
            util.assertSequenceEqual(list1, list2);
        }
        // reverse
        {
            tstd::list<int> list1(vec.begin(), vec.end());
            std::list<int> list2(vec.begin(), vec.end());
            list1.reverse();
            list2.reverse();
            util.assertSequenceEqual(list1, list2);
        }
        // unique
        {
            tstd::list<int> list1{1, 1, 1, 2, 3, 4, 1, 3, 4, 4, 10, 10, 1};
            std::list<int> list2{1, 1, 1, 2, 3, 4, 1, 3, 4, 4, 10, 10, 1};
            auto res1 = list1.unique();
            auto res2 = list2.unique();
            util.assertEqual(res1, res2);
            util.assertSequenceEqual(list1, list2);
            res1 = list1.unique([](int a, int b) -> bool { return a < b; });
            res2 = list2.unique([](int a, int b) -> bool { return a < b; });
            util.assertEqual(res1, res2);
            util.assertSequenceEqual(list1, list2);
        }
        // sort
        {
            tstd::list<int> list1(vec.begin(), vec.end());
            std::list<int> list2(vec.begin(), vec.end());
            list1.sort();
            list2.sort();
            util.assertSequenceEqual(list1, list2);
            list1.sort(std::greater<int>());
            list2.sort(std::greater<int>());
            util.assertSequenceEqual(list1, list2);
        }
    }
    // non-member operations
    {
        // comparison
        tstd::list<int> list1{1, 2, 3, 4, 5, 6};
        tstd::list<int> list2{1, 2, 3, 4, 5, 6};
        util.assertEqual(list1 == list2, true);
        util.assertEqual(list1 != list2, false);
        list1.front() = 2;
        util.assertEqual(list1 > list2, true);
        util.assertEqual(list1 >= list2, true);
        util.assertEqual(list2 < list1, true);
        util.assertEqual(list2 <= list1, true);
    }
    util.showFinalResult();
}

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testVector(showDetails);
    testArray(showDetails);
    testList(showDetails);
    std::cout << std::endl;
    return 0;
}
