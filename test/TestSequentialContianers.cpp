#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <deque>
#include <iterator>
#include <algorithm>
#include <functional>
#include <string>
#include <random>
#include <tmemory.hpp>
#include <tvector.hpp>
#include <tarray.hpp>
#include <tlist.hpp>
#include <tforward_list.hpp>
#include <tdeque.hpp>
#include "TestUtil.hpp"

void testVector(bool showDetails);
void testArray(bool showDetails);
void testVector(bool showDetails);
void testArray(bool showDetails);
void testList(bool showDetails);
void testForwardList(bool showDetails);
void testDeque(bool showDetails);

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testVector(showDetails);
    testArray(showDetails);
    testList(showDetails);
    testForwardList(showDetails);
    testDeque(showDetails);
    std::cout << std::endl;
    return 0;
}

void testVector(bool showDetails)
{
    TestUtil util(showDetails, "vector");
    // constructors
    {
        // 1
        tstd::vector<int> vec1;
        std::vector<int> vec2;
        util.assertSequenceEqual(vec1, vec2);
    }
    {
        // 2
        tstd::vector<int> vec1((tstd::allocator<int>()));
        std::vector<int> vec2((std::allocator<int>()));
        util.assertSequenceEqual(vec1, vec2);
    }
    {
        // 3
        tstd::allocator<int> alloc;
        tstd::vector<int> vec1(10, 99, alloc);
        std::vector<int, tstd::allocator<int>> vec2(10, 99, alloc);
        util.assertSequenceEqual(vec1, vec2);
    }
    {
        // 4
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
        std::shuffle(vec.begin(), vec.end(), std::mt19937());
        // 5
        tstd::vector<int> vec1(vec.begin(), vec.end());
        std::vector<int> vec2(vec.begin(), vec.end());
        util.assertSequenceEqual(vec1, vec2);
        // copy constuctor
        // 6
        tstd::vector<int> cvec1(vec1);
        std::vector<int> cvec2(vec2);
        util.assertSequenceEqual(cvec1, cvec2);
        // 7
        tstd::vector<int> cvec3(cvec1, tstd::allocator<int>());
        std::vector<int> cvec4(cvec2, std::allocator<int>());
        util.assertSequenceEqual(cvec3, cvec4);
        // move constructor
        // 8
        tstd::vector mvec1(std::move(cvec1));
        std::vector mvec2(std::move(cvec2));
        util.assertSequenceEqual(mvec1, mvec2);
        // 9
        tstd::vector<int> mvec3(std::move(mvec1), tstd::allocator<int>());
        std::vector<int> mvec4(std::move(mvec2), std::allocator<int>());
        util.assertSequenceEqual(mvec3, mvec4);
    }
    {
        // 10
        tstd::vector<int> vec1{1, 2, 3, 4};
        std::vector<int> vec2{1, 2, 3, 4};
        util.assertSequenceEqual(vec1, vec2);
        // assignment
        {
            tstd::vector<int> vec1c{1, 2, 3, 100, 99999};
            std::vector<int> vec2c{1, 2, 3, 100, 99999};
            // operator =
            // 1
            vec1 = vec1c;
            vec2 = vec2c;
            util.assertSequenceEqual(vec1, vec2);
            // 2
            vec1 = std::move(vec1c);
            vec2 = std::move(vec2c);
            util.assertSequenceEqual(vec1, vec2);
            // 3
            vec1 = {1, 2, 3, 4, 5, 10};
            vec2 = {1, 2, 3, 4, 5, 10};
            util.assertSequenceEqual(vec1, vec2);
            // assign
            // 1
            vec1.assign(10, 99);
            vec2.assign(10, 99);
            util.assertSequenceEqual(vec1, vec2);
            // 2
            vec1.assign(vec1c.begin(), vec1c.end());
            vec2.assign(vec2c.begin(), vec2c.end());
            util.assertSequenceEqual(vec1, vec2);
            // 3
            vec1.assign({1, 2, 3, 4, 5, 10});
            vec2.assign({1, 2, 3, 4, 5, 10});
            util.assertSequenceEqual(vec1, vec2);
        }
        // allocator 
        {
            util.assertEqual(vec1.get_allocator() == tstd::allocator<int>(), true);
        }
        // elements access
        // at
        util.assertEqual(vec1.at(0), vec2.at(0));
        util.assertEqual(vec1.at(1), vec2.at(1));
        util.assertEqual(vec1.at(2), vec2.at(2));
        util.assertEqual(vec1.at(5), vec2.at(5));
        // operator[]
        util.assertEqual(vec1[0], vec2[0]);
        util.assertEqual(vec1[1], vec2[1]);
        util.assertEqual(vec1[2], vec2[2]);
        util.assertEqual(vec1[5], vec2[5]);
        // front, back
        util.assertEqual(vec1.front(), vec2.front());
        util.assertEqual(vec1.back(), vec2.back());
        vec1.back() = vec2.back() = 9999;
        vec1.front() = vec2.front() = -9999;
        util.assertSequenceEqual(vec1, vec2);
        // data
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
        // clear
        vec1.clear();
        vec2.clear();
        util.assertEqual(vec1.empty(), vec2.empty());
    }
    {
        // modifiers again
        // insert
        tstd::vector<int> vec1{1, 2, 3, 4};
        std::vector<int> vec2{1, 2, 3, 4};
        // 1
        int a = 10;
        vec1.insert(vec1.begin() + 2, a);
        vec2.insert(vec2.begin() + 2, a);
        util.assertSequenceEqual(vec1, vec2);
        // 2
        vec1.insert(vec1.begin() + 2, 10);
        vec2.insert(vec2.begin() + 2, 10);
        util.assertSequenceEqual(vec1, vec2);
        // 3
        vec1.insert(vec1.begin(), 100, 9999);
        vec2.insert(vec2.begin(), 100, 9999);
        util.assertSequenceEqual(vec1, vec2);
        {
            // 4
            std::vector<int> tmp(100);
            std::iota(tmp.begin(), tmp.end(), 1);
            std::shuffle(tmp.begin(), tmp.end(), std::mt19937());
            vec1.insert(vec1.begin() + 33, tmp.begin(), tmp.end());
            vec2.insert(vec2.begin() + 33, tmp.begin(), tmp.end());
            util.assertSequenceEqual(vec1, vec2);
        }
        // 5
        vec1.insert(vec1.begin(), {1, 2, 3});
        vec2.insert(vec2.begin(), {1, 2, 3});
        util.assertSequenceEqual(vec1, vec2);
        // emplace
        vec1.emplace(vec1.begin() + 100, -1024);
        vec2.emplace(vec2.begin() + 100, -1024);
        util.assertSequenceEqual(vec1, vec2);
        // erase
        // 1
        vec1.erase(vec1.begin());
        vec2.erase(vec2.begin());
        util.assertSequenceEqual(vec1, vec2);
        // 2
        vec1.erase(vec1.begin() + 100, vec1.begin() + 150);
        vec2.erase(vec2.begin() + 100, vec2.begin() + 150);
        util.assertSequenceEqual(vec1, vec2);
        // push_back
        // 1, 2
        int b = 1;
        vec1.push_back(b);
        vec1.push_back(100);
        vec1.push_back(1001);
        vec2.push_back(b);
        vec2.push_back(100);
        vec2.push_back(1001);
        util.assertSequenceEqual(vec1, vec2);
        // emplace_back
        vec1.emplace_back(b);
        vec1.emplace_back(100);
        vec1.emplace_back(1001);
        vec2.emplace_back(b);
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
        // 1
        vec1.resize(vec1.size() / 2);
        vec2.resize(vec2.size() / 2);
        util.assertSequenceEqual(vec1, vec2);
        // 2
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
    // non-member operations
    {
        // comparisons
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
        // at
        arr1.at(0) = 10;
        arr2.at(0) = 10;
        // operator []
        arr1[9] = 99;
        arr2[9] = 99;
        util.assertSequenceEqual(arr1, arr2);
        // front
        arr1.front() = -1;
        arr2.front() = -1;
        // back
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
        // size and capacity
        util.assertEqual(arr1.empty(), arr2.empty());
        util.assertEqual(arr1.empty(), false);
        util.assertEqual(arr1.size(), arr2.size());
        util.assertEqual(arr1.size(), 10);
        util.assertEqual(arr1.max_size(), arr2.max_size());
        util.assertEqual(arr1.max_size(), 10);
        // modifiers
        // fill
        arr1.fill(0);
        arr2.fill(0);
        // swap
        util.assertSequenceEqual(arr1, arr2);
        {
            tstd::array<int, 10> arr3{1, 2, 3};
            std::array<int, 10> arr4{1, 2, 3};
            arr1.swap(arr3);
            arr2.swap(arr4);
            util.assertSequenceEqual(arr1, arr2);
            util.assertSequenceEqual(arr3, arr4);
            swap(arr1, arr3);
            swap(arr2, arr4);
            util.assertSequenceEqual(arr1, arr2);
            util.assertSequenceEqual(arr3, arr4);
            tstd::swap(arr1, arr3);
            std::swap(arr2, arr4);
            util.assertSequenceEqual(arr1, arr2);
            util.assertSequenceEqual(arr3, arr4);
        }
    }
    // non-member operations
    {
        // comparisons
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
            // 1
            tstd::list<int> list1;
            std::list<int> list2;
            util.assertEqual(list1.empty(), true);
            util.assertEqual(list1.begin() == list1.end(), true);
            util.assertEqual(list1.size(), 0);
            util.assertSequenceEqual(list1, list2);
        }
        {
            // 2
            tstd::list<int> list1((tstd::allocator<int>()));
            std::list<int> list2((std::allocator<int>()));
            util.assertEqual(list1.empty(), true);
            util.assertEqual(list1.begin() == list1.end(), true);
            util.assertEqual(list1.size(), 0);
            util.assertSequenceEqual(list1, list2);
        }
        {
            // 3
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
            // 4
            tstd::list<int> list1(100);
            std::list<int> list2(100);
            util.assertSequenceEqual(list1, list2);
        }
        {
            // 5
            std::vector<int> vec(100);
            std::iota(vec.begin(), vec.end(), 1);
            std::shuffle(vec.begin(), vec.end(), std::mt19937());
            tstd::list<int> list1(vec.begin(), vec.end());
            std::list<int> list2(vec.begin(), vec.end());
            util.assertSequenceEqual(list1, list2);
        }
        {
            std::vector<int> vec(100);
            std::iota(vec.begin(), vec.end(), 1);
            std::shuffle(vec.begin(), vec.end(), std::mt19937());
            tstd::list<int> tmp1(vec.begin(), vec.end());
            std::list<int> tmp2(vec.begin(), vec.end());
            // 6
            {
                tstd::list<int> list1(tmp1);
                std::list<int> list2(tmp2);
                util.assertSequenceEqual(list1, list2);
            }
            // 7
            {
                tstd::list<int> list1(tmp1, tstd::allocator<int>());
                std::list<int> list2(tmp2, std::allocator<int>());
                util.assertSequenceEqual(list1, list2);
            }
            // 8, 9
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
            // 10
            {
                tstd::list<int> list1{1, 2, 3, 4, 5};
                std::list<int> list2{1, 2, 3, 4, 5};
                util.assertSequenceEqual(list1, list2);
            }
        }
    }
    {
        // assignment
        std::vector<int> vec(1000);
        std::iota(vec.begin(), vec.end(), 1);
        std::shuffle(vec.begin(), vec.end(), std::mt19937());

        // operator=
        tstd::list<int> list1;
        std::list<int> list2;
        tstd::list<int> tmp1(vec.begin(), vec.end());
        std::list<int> tmp2(vec.begin(), vec.end());
        // 1
        list1 = tmp1;
        list2 = tmp2;
        util.assertSequenceEqual(list1, list2);
        // 2
        list1 = std::move(tmp1);
        list2 = std::move(tmp2);
        util.assertSequenceEqual(list1, list2);
        util.assertSequenceEqual(tmp1, tmp2);
        // 3
        list1 = {1, 2, 3, 4, 5};
        list2 = {1, 2, 3, 4, 5};
        util.assertSequenceEqual(list1, list2);
        // assign
        // 1
        list1.assign(100, 9999);
        list2.assign(100, 9999);
        util.assertSequenceEqual(list1, list2);
        // 2
        list1.assign(vec.begin(), vec.end());
        list2.assign(vec.begin(), vec.end());
        util.assertSequenceEqual(list1, list2);
        // 3
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
        // front
        tstd::list<int> list1{1, 2, 3, 4, 5};
        std::list<int> list2{1, 2, 3, 4, 5};
        util.assertEqual(list1.front(), list2.front());
        util.assertEqual(list1.front(), 1);
        list1.front() = 10;
        list2.front() = 10;
        util.assertEqual(list1.front(), list2.front());
        util.assertEqual(list1.front(), 10);
        // back
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
        // clear
        list1.clear();
        list2.clear();
        util.assertEqual(list1.empty(), true);
        util.assertEqual(list1.empty(), list2.empty());
    }
    {
        std::vector<int> vec(1000);
        std::iota(vec.begin(), vec.end(), 1);
        std::shuffle(vec.begin(), vec.end(), std::mt19937());

        // modifiers
        tstd::list<int> list1{1, 2, 3, 4, 5};
        std::list<int> list2{1, 2, 3, 4, 5};
        // insert
        // 1
        int a = 100;
        list1.insert(list1.begin(), a);
        list2.insert(list2.begin(), a);
        // 2
        list1.insert(std::next(list1.begin(), 2), 99);
        list2.insert(std::next(list2.begin(), 2), 99);
        list1.insert(list1.end(), 42);
        list2.insert(list2.end(), 42);
        util.assertSequenceEqual(list1, list2);
        // 3
        list1.insert(tstd::next(list1.begin(), 5), 10, 1024);
        list2.insert(tstd::next(list2.begin(), 5), 10, 1024);
        util.assertSequenceEqual(list1, list2);
        // 4
        list1.insert(tstd::next(list1.begin(), 5), vec.begin(), vec.end());
        list2.insert(tstd::next(list2.begin(), 5), vec.begin(), vec.end());
        util.assertSequenceEqual(list1, list2);
        // 5
        list1.insert(tstd::next(list1.begin(), 5), {1, 2, 3, 4, 5});
        list2.insert(tstd::next(list2.begin(), 5), {1, 2, 3, 4, 5});
        util.assertSequenceEqual(list1, list2);
        // emplace
        list1.emplace(tstd::next(list1.begin(), 5), 2048);
        list2.emplace(tstd::next(list2.begin(), 5), 2048);
        util.assertSequenceEqual(list1, list2);
        // erase
        // 1
        list1.erase(list1.begin());
        list2.erase(list2.begin());
        // 2
        list1.erase(tstd::next(list1.begin(), 10), tstd::next(list1.begin(), 100));
        list2.erase(tstd::next(list2.begin(), 10), tstd::next(list2.begin(), 100));
        util.assertSequenceEqual(list1, list2);
        // push_back
        // 1
        list1.push_back(a);
        list2.push_back(a);
        // 2
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
        // 1
        list1.resize(100);
        list2.resize(100);
        util.assertSequenceEqual(list1, list2);
        // 2
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
        std::shuffle(vec.begin(), vec.end(), std::mt19937());

        // oeprations/algorithms
        // merge
        {
            tstd::list<int> list1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            std::list<int> list2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            // 1
            {
                tstd::list<int> tmp1{1, 2, 3, 4, 5};
                std::list<int> tmp2{1, 2, 3, 4, 5};
                list1.merge(tmp1);
                list2.merge(tmp2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            // 2
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
            // 3
            {
                tstd::list<int> list1{10, 9, 5, 4, 2, 1};
                std::list<int> list2{10, 9, 5, 4, 2, 1};
                tstd::list<int> tmp1{5, 4, 3, 2, 1};
                std::list<int> tmp2{5, 4, 3, 2, 1};
                list1.merge(tmp1, std::greater<int>());
                list2.merge(tmp2, std::greater<int>());
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            // 4
            {
                tstd::list<int> list1{10, 9, 5, 4, 2, 1};
                std::list<int> list2{10, 9, 5, 4, 2, 1};
                tstd::list<int> tmp1{5, 4, 3, 2, 1};
                std::list<int> tmp2{5, 4, 3, 2, 1};
                list1.merge(std::move(tmp1), std::greater<int>());
                list2.merge(std::move(tmp2), std::greater<int>());
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
        }
        // splice
        {
            {
                // 1
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
                // 2
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
                // 3
                list1.splice(tstd::next(list1.begin(), 20), tmp1, tstd::next(tmp1.begin(), 2));
                list2.splice(tstd::next(list2.begin(), 20), tmp2, tstd::next(tmp2.begin(), 2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                // 4
                list1.splice(tstd::next(list1.begin(), 20), std::move(tmp1), tstd::next(tmp1.begin(), 2));
                list2.splice(tstd::next(list2.begin(), 20), std::move(tmp2), tstd::next(tmp2.begin(), 2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                // 5
                list1.splice(tstd::next(list1.begin(), 20), tmp1, tmp1.begin(), tmp1.end());
                list2.splice(tstd::next(list2.begin(), 20), tmp2, tmp2.begin(), tmp2.end());
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                // 6
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
            // 1
            auto res1 = list1.unique();
            auto res2 = list2.unique();
            util.assertEqual(res1, res2);
            util.assertSequenceEqual(list1, list2);
            // 2
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
        // comparisons
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

void testForwardList(bool showDetails)
{
    TestUtil util(showDetails, "forward_list");

    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());
    {
        // constructors
        {
            // 1
            tstd::forward_list<int> li;
            util.assertEqual(li.empty(), true);
            util.assertEqual(li.begin() == li.end(), true);
        }
        {
            // 2
            tstd::forward_list<int> li((tstd::allocator<int>()));
            util.assertEqual(li.empty(), true);
            util.assertEqual(li.begin() == li.end(), true);
        }
        {
            // 3
            {
                tstd::forward_list<int> list1(2, 99); // can not use std::allocator for tstd::forward_list<int> in C++20 because of compatibility reasons
                std::forward_list<int> list2(2, 99);
                util.assertSequenceEqual(list1, list2);
            }
            {
                tstd::forward_list<int> list1(10, 99, tstd::allocator<int>());
                std::forward_list<int> list2(10, 99, std::allocator<int>());
                util.assertSequenceEqual(list1, list2);
            }
        }
        {
            // 4
            tstd::forward_list<int> list1(100);
            std::forward_list<int> list2(100);
            util.assertSequenceEqual(list1, list2);
        }
        {
            // 5
            tstd::forward_list<int> list1(vec.begin(), vec.end());
            std::forward_list<int> list2(vec.begin(), vec.end());
            util.assertSequenceEqual(list1, list2);
        }
        {
            // 6, 7, 8, 9
            tstd::forward_list<int> tmp1(vec.begin(), vec.end());
            std::forward_list<int> tmp2(vec.begin(), vec.end());
            {
                tstd::forward_list<int> list1(tmp1);
                std::forward_list<int> list2(tmp2);
                util.assertSequenceEqual(list1, list2);
            }
            {
                tstd::forward_list<int> list1(tmp1, tstd::allocator<int>());
                std::forward_list<int> list2(tmp2, std::allocator<int>());
                util.assertSequenceEqual(list1, list2);
            }
            {
                // move constructors
                tstd::forward_list<int> list1(std::move(tmp1));
                std::forward_list<int> list2(std::move(tmp2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tstd::forward_list<int> list3(std::move(list1), tstd::allocator<int>());
                std::forward_list<int> list4(std::move(list2), std::allocator<int>());
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(list3, list4);
            }
            // 10
            {
                tstd::forward_list<int> list1{1, 2, 3, 4, 5};
                std::forward_list<int> list2{1, 2, 3, 4, 5};
                util.assertSequenceEqual(list1, list2);
            }
        }
    }
    {
        // assignment: operator=/assign
        tstd::forward_list<int> list1;
        std::forward_list<int> list2;
        tstd::forward_list<int> tmp1(vec.begin(), vec.end());
        std::forward_list<int> tmp2(vec.begin(), vec.end());
        // operator=
        // 1
        list1 = tmp1;
        list2 = tmp2;
        util.assertSequenceEqual(list1, list2);
        // 2
        list1 = std::move(tmp1);
        list2 = std::move(tmp2);
        util.assertSequenceEqual(list1, list2);
        util.assertSequenceEqual(tmp1, tmp2);
        // 3
        list1 = {1, 2, 3, 4, 5};
        list2 = {1, 2, 3, 4, 5};
        util.assertSequenceEqual(list1, list2);
        // assign
        // 1
        list1.assign(100, 9999);
        list2.assign(100, 9999);
        util.assertSequenceEqual(list1, list2);
        // 2
        list1.assign(vec.begin(), vec.end());
        list2.assign(vec.begin(), vec.end());
        util.assertSequenceEqual(list1, list2);
        // 3
        list1.assign({1, 2, 3, 4, 5, 100});
        list2.assign({1, 2, 3, 4, 5, 100});
        util.assertSequenceEqual(list1, list2);
    }
    {
        // allocator
        tstd::forward_list<int> tmp;
        util.assertEqual(tstd::allocator<int>() == tmp.get_allocator(), true);
    }
    {
        // element access
        tstd::forward_list<int> list1{1, 2, 3, 4, 5};
        std::forward_list<int> list2{1, 2, 3, 4, 5};
        util.assertEqual(list1.front(), list2.front());
        util.assertEqual(list1.front(), 1);
        list1.front() = 10;
        list2.front() = 10;
        util.assertEqual(list1.front(), list2.front());
        util.assertEqual(list1.front(), 10);
    }
    {
        // iterators
        tstd::forward_list<int> list1{1, 2, 3, 4, 5};
        std::forward_list<int> list2{1, 2, 3, 4, 5};
        util.assertEqual(*tstd::next(list1.before_begin()), *std::next(list2.before_begin()));
        util.assertEqual(*tstd::next(list1.cbefore_begin()), *std::next(list2.cbefore_begin()));
        util.assertEqual(*list1.begin(), *list2.begin());
        util.assertEqual(*list1.cbegin(), *list2.cbegin());
        util.assertEqual(tstd::distance(list1.before_begin(), list1.end()), std::distance(list2.before_begin(), list2.end()));
        util.assertEqual(tstd::distance(list1.cbefore_begin(), list1.cend()), std::distance(list2.cbefore_begin(), list2.cend()));
    }
    {
        // size and capacity
        tstd::forward_list<int> list1{1, 2, 3, 4, 5};
        std::forward_list<int> list2{1, 2, 3, 4, 5};
        util.assertEqual(list1.empty(), list2.empty());

        // modifiers
        list1.clear();
        list2.clear();
        util.assertEqual(list1.empty(), true);
        util.assertEqual(list1.empty(), list2.empty());
    }
    {
        // modifiers
        tstd::forward_list<int> list1{1, 2, 3, 4, 5};
        std::forward_list<int> list2{1, 2, 3, 4, 5};
        // insert_after
        // 1
        int a = 100;
        list1.insert_after(list1.before_begin(), a);
        list2.insert_after(list2.before_begin(), a);
        // 2
        list1.insert_after(std::next(list1.begin(), 2), 99);
        list2.insert_after(std::next(list2.begin(), 2), 99);
        util.assertSequenceEqual(list1, list2);
        // 3
        list1.insert_after(tstd::next(list1.begin(), 5), 10, 1024);
        list2.insert_after(tstd::next(list2.begin(), 5), 10, 1024);
        util.assertSequenceEqual(list1, list2);
        // 4
        list1.insert_after(tstd::next(list1.begin(), 5), vec.begin(), vec.end());
        list2.insert_after(tstd::next(list2.begin(), 5), vec.begin(), vec.end());
        util.assertSequenceEqual(list1, list2);
        // 5
        list1.insert_after(tstd::next(list1.begin(), 5), {1, 2, 3, 4, 5});
        list2.insert_after(tstd::next(list2.begin(), 5), {1, 2, 3, 4, 5});
        // emplace_after
        list1.emplace_after(tstd::next(list1.begin(), 5), 2048);
        list2.emplace_after(tstd::next(list2.begin(), 5), 2048);
        util.assertSequenceEqual(list1, list2);
        // erase_after
        // 1
        list1.erase_after(list1.begin());
        list2.erase_after(list2.begin());
        util.assertSequenceEqual(list1, list2);

        // 2
        list1.erase_after(tstd::next(list1.begin(), 10), tstd::next(list1.begin(), 100));
        list2.erase_after(tstd::next(list2.begin(), 10), tstd::next(list2.begin(), 100));
        util.assertSequenceEqual(list1, list2);
        // push_front
        for (int i = 0; i < 100; ++i)
        {
            // 1, 2
            int a = 1;
            list1.push_front(a);
            list2.push_front(a);
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
        // 1
        list1.resize(100);
        list2.resize(100);
        util.assertSequenceEqual(list1, list2);
        // 2
        list1.resize(1000, 1024);
        list2.resize(1000, 1024);
        util.assertSequenceEqual(list1, list2);
        // swap
        {
            tstd::forward_list<int> tmp1{1, 2, 3, 4, 5};
            std::forward_list<int> tmp2{1, 2, 3, 4, 5};
            list1.swap(tmp1);
            list2.swap(tmp2);
            util.assertSequenceEqual(list1, list2);
            util.assertSequenceEqual(tmp1, tmp2);
            tstd::swap(list1, tmp1);
            std::swap(list2, tmp2);
            util.assertSequenceEqual(list1, list2);
            util.assertSequenceEqual(tmp1, tmp2);
            // case of empty forward_list
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
        // oeprations/algorithms
        // merge
        {
            tstd::forward_list<int> list1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            std::forward_list<int> list2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            {
                // 1
                tstd::forward_list<int> tmp1{1, 2, 3, 4, 5};
                std::forward_list<int> tmp2{1, 2, 3, 4, 5};
                list1.merge(tmp1);
                list2.merge(tmp2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            {
                // 2
                tstd::forward_list<int> tmp1{1, 2, 3, 4, 5};
                std::forward_list<int> tmp2{1, 2, 3, 4, 5};
                list1.merge(std::move(tmp1));
                list2.merge(std::move(tmp2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            {
                // one of the lists is empty
                tstd::forward_list<int> tmp1;
                std::forward_list<int> tmp2;
                list1.merge(tmp1);
                list2.merge(tmp2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tmp1.merge(list1);
                tmp2.merge(list2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            {
                // 3
                tstd::forward_list<int> list1{10, 9, 5, 4, 2, 1};
                std::forward_list<int> list2{10, 9, 5, 4, 2, 1};
                tstd::forward_list<int> tmp1{5, 4, 3, 2, 1};
                std::forward_list<int> tmp2{5, 4, 3, 2, 1};
                list1.merge(tmp1, std::greater<int>());
                list2.merge(tmp2, std::greater<int>());
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            {
                // 4
                tstd::forward_list<int> list1{10, 9, 5, 4, 2, 1};
                std::forward_list<int> list2{10, 9, 5, 4, 2, 1};
                tstd::forward_list<int> tmp1{5, 4, 3, 2, 1};
                std::forward_list<int> tmp2{5, 4, 3, 2, 1};
                list1.merge(std::move(tmp1), std::greater<int>());
                list2.merge(std::move(tmp2), std::greater<int>());
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
        }
        // splice_after
        {
            {
                // 1
                tstd::forward_list<int> list1(vec.begin(), vec.end());
                std::forward_list<int> list2(vec.begin(), vec.end());
                tstd::forward_list<int> tmp1{1, 2, 3, 4, 5};
                std::forward_list<int> tmp2{1, 2, 3, 4, 5};
                list1.splice_after(std::next(list1.begin(), 20), tmp1);
                list2.splice_after(std::next(list2.begin(), 20), tmp2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                list1.splice_after(std::next(list1.begin(), 20), tmp1);
                list2.splice_after(std::next(list2.begin(), 20), tmp2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tmp1.splice_after(tmp1.before_begin(), list1);
                tmp2.splice_after(tmp2.before_begin(), list2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tmp1.clear();
                tmp2.clear();
                list1.splice_after(list1.before_begin(), tmp1);
                list2.splice_after(list2.before_begin(), tmp2);
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            {
                // 2
                tstd::forward_list<int> list1(vec.begin(), vec.end());
                std::forward_list<int> list2(vec.begin(), vec.end());
                tstd::forward_list<int> tmp1{1, 2, 3, 4, 5};
                std::forward_list<int> tmp2{1, 2, 3, 4, 5};
                list1.splice_after(std::next(list1.begin(), 20), std::move(tmp1));
                list2.splice_after(std::next(list2.begin(), 20), std::move(tmp2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                list1.splice_after(std::next(list1.begin(), 20), std::move(tmp1));
                list2.splice_after(std::next(list2.begin(), 20), std::move(tmp2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tmp1.splice_after(tmp1.before_begin(), std::move(list1));
                tmp2.splice_after(tmp2.before_begin(), std::move(list2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                tmp1.clear();
                tmp2.clear();
                list1.splice_after(list1.before_begin(), std::move(tmp1));
                list2.splice_after(list2.before_begin(), std::move(tmp2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
            {
                tstd::forward_list<int> list1(vec.begin(), vec.end());
                std::forward_list<int> list2(vec.begin(), vec.end());
                tstd::forward_list<int> tmp1{1, 2, 3, 4, 5};
                std::forward_list<int> tmp2{1, 2, 3, 4, 5};
                // 3
                list1.splice_after(tstd::next(list1.begin(), 20), tmp1, tstd::next(tmp1.begin(), 2));
                list2.splice_after(tstd::next(list2.begin(), 20), tmp2, tstd::next(tmp2.begin(), 2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                // 4
                list1.splice_after(tstd::next(list1.begin(), 20), std::move(tmp1), tstd::next(tmp1.begin(), 2));
                list2.splice_after(tstd::next(list2.begin(), 20), std::move(tmp2), tstd::next(tmp2.begin(), 2));
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                // 5
                list1.splice_after(tstd::next(list1.begin(), 20), tmp1, tmp1.begin(), tmp1.end());
                list2.splice_after(tstd::next(list2.begin(), 20), tmp2, tmp2.begin(), tmp2.end());
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
                // 6
                list1.splice_after(tstd::next(list1.begin(), 20), std::move(tmp1), tmp1.begin(), tmp1.end());
                list2.splice_after(tstd::next(list2.begin(), 20), std::move(tmp2), tmp2.begin(), tmp2.end());
                util.assertSequenceEqual(list1, list2);
                util.assertSequenceEqual(tmp1, tmp2);
            }
        }
        // remove, remove_if
        {
            tstd::forward_list<int> list1(vec.begin(), vec.end());
            std::forward_list<int> list2(vec.begin(), vec.end());
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
            tstd::forward_list<int> list1(vec.begin(), vec.end());
            std::forward_list<int> list2(vec.begin(), vec.end());
            list1.reverse();
            list2.reverse();
            util.assertSequenceEqual(list1, list2);
        }
        // unique
        {
            tstd::forward_list<int> list1{1, 1, 1, 2, 3, 4, 1, 3, 4, 4, 10, 10, 1};
            std::forward_list<int> list2{1, 1, 1, 2, 3, 4, 1, 3, 4, 4, 10, 10, 1};
            // 1
            auto res1 = list1.unique();
            auto res2 = list2.unique();
            util.assertEqual(res1, res2);
            util.assertSequenceEqual(list1, list2);
            // 2
            res1 = list1.unique([](int a, int b) -> bool { return a < b; });
            res2 = list2.unique([](int a, int b) -> bool { return a < b; });
            util.assertEqual(res1, res2);
            util.assertSequenceEqual(list1, list2);
        }
        // sort
        {
            tstd::forward_list<int> list1(vec.begin(), vec.end());
            std::forward_list<int> list2(vec.begin(), vec.end());
            // 1
            list1.sort();
            list2.sort();
            util.assertSequenceEqual(list1, list2);
            // 2
            list1.sort(std::greater<int>());
            list2.sort(std::greater<int>());
            util.assertSequenceEqual(list1, list2);
        }
    }
    // non-member operations
    {
        // comparisons
        tstd::forward_list<int> list1{1, 2, 3, 4, 5, 6};
        tstd::forward_list<int> list2{1, 2, 3, 4, 5, 6};
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

void testDeque(bool showDetails)
{
    TestUtil util(showDetails, "deque");
    // a range for test
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());
    {
        // constructors
        // 1
        {
            tstd::deque<int> dq1;
            std::deque<int> dq2;
            util.assertSequenceEqual(dq1, dq2);
            util.assertEqual(dq1.empty(), true);
            util.assertEqual(dq1.size(), 0);
        }
        // 2
        {
            tstd::deque<int> dq1((tstd::allocator<int>()));
            std::deque<int> dq2((std::allocator<int>()));
            util.assertSequenceEqual(dq1, dq2);
            util.assertEqual(dq1.empty(), true);
            util.assertEqual(dq1.size(), 0);
        }
        // 3
        {
            tstd::deque<int> dq1(10, 99);
            std::deque<int> dq2(10, 99);
            util.assertSequenceEqual(dq1, dq2);
            util.assertEqual(dq1.empty(), false);
            util.assertEqual(dq1.size(), 10);
        }
        // 4
        {
            tstd::deque<int> dq1(10);
            std::deque<int> dq2(10);
            util.assertSequenceEqual(dq1, dq2);
        }
        // 5
        {
            tstd::deque<int> dq1(vec.begin(), vec.end());
            std::deque<int> dq2(vec.begin(), vec.end());
            util.assertSequenceEqual(dq1, dq2);
        }
        // 6, 7, 8, 9
        {
            tstd::deque<int> tmp1(vec.begin(), vec.end());
            std::deque<int> tmp2(vec.begin(), vec.end());
            {
                tstd::deque<int> dq1(tmp1);
                std::deque<int> dq2(tmp2);
                util.assertSequenceEqual(dq1, dq2);
            }
            {
                tstd::deque<int> dq1(tmp1, tstd::allocator<int>());
                std::deque<int> dq2(tmp2, std::allocator<int>());
                util.assertSequenceEqual(dq1, dq2);
            }
            {
                tstd::deque<int> dq1(std::move(tmp1));
                std::deque<int> dq2(std::move(tmp2));
                util.assertSequenceEqual(dq1, dq2);
                util.assertSequenceEqual(tmp1, tmp2);
                tstd::deque<int> dqm1(std::move(dq1), tstd::allocator<int>());
                std::deque<int> dqm2(std::move(dq2), std::allocator<int>());
                util.assertSequenceEqual(dqm1, dqm2);
                util.assertSequenceEqual(dq1, dq2);
            }
        }
        // 10
        {
            tstd::deque<int> dq1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            std::deque<int> dq2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            util.assertSequenceEqual(dq1, dq2);
        }
    }
    {
        // assignment
        tstd::deque<int> tmp1(vec.begin(), vec.end());
        std::deque<int> tmp2(vec.begin(), vec.end());
        tstd::deque<int> dq1;
        std::deque<int> dq2;
        // operator=
        // 1
        dq1 = tmp1;
        dq2 = tmp2;
        util.assertSequenceEqual(dq1, tmp1);
        util.assertSequenceEqual(dq1, dq2);
        // 2
        dq1 = std::move(tmp1);
        dq2 = std::move(tmp2);
        util.assertSequenceEqual(dq1, dq2);
        util.assertSequenceEqual(tmp1, tmp2);
        // 3
        dq1 = {1, 2, 3, 4, 5};
        dq2 = {1, 2, 3, 4, 5};
        util.assertSequenceEqual(dq1, dq2);
        // assign
        // 1
        dq1.assign(10, 99);
        dq2.assign(10, 99);
        util.assertSequenceEqual(dq1, dq2);
        // 2
        dq1.assign(vec.begin(), vec.end());
        dq2.assign(vec.begin(), vec.end());
        util.assertSequenceEqual(dq1, dq2);
        // 3
        dq1.assign({1, 2, 3, 4, 5});
        dq2.assign({1, 2, 3, 4, 5});
        util.assertSequenceEqual(dq1, dq2);
    }
    {
        // allocator
        tstd::deque<int> dq1;
        util.assertEqual(dq1.get_allocator() == tstd::allocator<int>(), true);
    }
    {
        // element access
        tstd::deque<int> dq1(vec.begin(), vec.end());
        std::deque<int> dq2(vec.begin(), vec.end());
        std::vector<int> vec1, vec2;
        // at, operator[]
        for (std::size_t i = 0; i < dq1.size(); ++i)
        {
            vec1.push_back(dq1.at(i));
            vec1.push_back(dq1[i]);
            vec2.push_back(dq2.at(i));
            vec2.push_back(dq2[i]);
        }
        util.assertSequenceEqual(vec1, vec2);
        // front, back
        util.assertEqual(dq1.front(), dq2.front());
        util.assertEqual(dq1.back(), dq2.back());
        dq1.front() = dq1.back() = 9999;
        dq2.front() = dq2.back() = 9999;
        util.assertSequenceEqual(dq1, dq2);
    }
    {
        // iterators
        tstd::deque<int> dq1(vec.begin(), vec.end());
        std::deque<int> dq2(vec.begin(), vec.end());
        util.assertEqual(*dq2.begin(), *dq1.begin());
        util.assertEqual(*dq2.cbegin(), *dq1.cbegin());
        util.assertEqual(*tstd::prev(dq1.end()), *std::prev(dq2.end()));
        util.assertEqual(*tstd::prev(dq1.cend()), *std::prev(dq2.cend()));
        util.assertEqual(*dq2.rbegin(), *dq1.rbegin());
        util.assertEqual(*dq2.crbegin(), *dq1.crbegin());
        util.assertEqual(*tstd::prev(dq1.rend()), *std::prev(dq2.rend()));
        util.assertEqual(*tstd::prev(dq1.crend()), *std::prev(dq2.crend()));
        // random access iterator test
        auto iter = dq1.begin();
        util.assertEqual(++iter == dq1.begin() + 1, true);
        util.assertEqual(--iter == dq1.begin(), true);
        util.assertEqual(iter++ == dq1.begin(), true);
        util.assertEqual(iter == dq1.begin() + 1, true);
        util.assertEqual(iter-- == dq1.begin() + 1, true);
        util.assertEqual(iter == dq1.begin(), true);
        util.assertEqual(dq1.end() - dq1.begin(), dq1.size());
        util.assertEqual((iter += 3) == dq1.begin() + 3, true);
        util.assertEqual((iter -= 3) != dq1.begin(), false);
        util.assertEqual(dq1.begin() < dq1.end(), true);
        util.assertEqual(dq1.begin() > dq1.end(), false);
        util.assertEqual(dq1.begin() <= dq1.end(), true);
        util.assertEqual(dq1.begin() >= dq1.end(), false);
    }
    {
        // size and capacity
        tstd::deque<int> dq1(vec.begin(), vec.end());
        std::deque<int> dq2(vec.begin(), vec.end());
        util.assertEqual(dq1.size(), dq2.size());
        util.assertEqual(dq1.empty(), dq2.empty());
        // modififers
        // clear
        dq1.clear();
        dq2.clear();
        util.assertSequenceEqual(dq1, dq2);
    }
    {
        // modifiers
        // insert
        tstd::deque<int> dq1(vec.begin(), vec.end());
        std::deque<int> dq2(vec.begin(), vec.end());
        // 1
        int a = 100;
        dq1.insert(dq1.begin(), a);
        dq2.insert(dq2.begin(), a);
        // 2
        dq1.insert(dq1.begin(), 10);
        dq2.insert(dq2.begin(), 10);
        util.assertSequenceEqual(dq1, dq2);
        // 3
        dq1.insert(dq1.begin() + 20, 10, 1024);
        dq2.insert(dq2.begin() + 20, 10, 1024);
        util.assertSequenceEqual(dq1, dq2);
        // 4
        dq1.insert(dq1.begin() + 50, vec.begin(), vec.end());
        dq2.insert(dq2.begin() + 50, vec.begin(), vec.end());
        util.assertSequenceEqual(dq1, dq2);
        // 5
        dq1.insert(dq1.end() - 10, {1, 2, 3, 4, 5});
        dq2.insert(dq2.end() - 10, {1, 2, 3, 4, 5});
        util.assertSequenceEqual(dq1, dq2);
        // emplace
        dq1.emplace(dq1.end() - 20, 10);
        dq2.emplace(dq2.end() - 20, 10);
        util.assertSequenceEqual(dq1, dq2);
        // erase
        // 1
        dq1.erase(dq1.begin());
        dq2.erase(dq2.begin());
        dq1.erase(dq1.begin() + 50);
        dq2.erase(dq2.begin() + 50);
        util.assertSequenceEqual(dq1, dq2);
        // 2
        dq1.erase(dq1.begin() + 10, dq1.end() - 30);
        dq2.erase(dq2.begin() + 10, dq2.end() - 30);
        util.assertSequenceEqual(dq1, dq2);
        dq1.erase(dq1.begin(), dq1.end());
        dq2.erase(dq2.begin(), dq2.end());
        util.assertSequenceEqual(dq1, dq2);
        // push_back, emplace_back
        for (int i = 0; i < 10000; ++i)
        {
            dq1.push_back(i);
            dq1.push_back(std::move(i));
            dq1.emplace_back(i);
            dq2.push_back(i);
            dq2.push_back(std::move(i));
            dq2.emplace_back(i);
        }
        util.assertSequenceEqual(dq1, dq2);
        // push_front, emplace_front
        for (int i = 0; i < 10000; ++i)
        {
            dq1.push_front(i);
            dq2.push_front(i);
            dq1.push_front(std::move(i));
            dq2.push_front(std::move(i));
            dq1.emplace_front(i);
            dq2.emplace_front(i);
        }
        util.assertSequenceEqual(dq1, dq2);
        // pop_front, pop_back
        for (int i = 0; i < 10000; ++i)
        {
            dq1.pop_front();
            dq2.pop_front();
            dq1.pop_back();
            dq2.pop_back();
        }
        util.assertSequenceEqual(dq1, dq2);
        // resize
        // 1
        dq1.resize(100000);
        dq2.resize(100000);
        util.assertSequenceEqual(dq1, dq2);
        dq1.resize(100);
        dq2.resize(100);
        util.assertSequenceEqual(dq1, dq2);
        // 2
        dq1.resize(1000, 9999);
        dq2.resize(1000, 9999);
        util.assertSequenceEqual(dq1, dq2);
        dq1.resize(100, 200);
        dq2.resize(100, 200);
        util.assertSequenceEqual(dq1, dq2);
        // swap
        {
            tstd::deque<int> tmp1(vec.begin(), vec.end());
            std::deque<int> tmp2(vec.begin(), vec.end());
            tmp1.swap(dq1);
            tmp2.swap(dq2);
            util.assertSequenceEqual(dq1, dq2);
            util.assertSequenceEqual(tmp1, tmp2);
            swap(dq1, tmp1);
            swap(dq2, tmp2);
            util.assertSequenceEqual(dq1, dq2);
            util.assertSequenceEqual(tmp1, tmp2);
            tstd::swap(dq1, tmp1);
            tstd::swap(dq2, tmp2);
            util.assertSequenceEqual(dq1, dq2);
            util.assertSequenceEqual(tmp1, tmp2);
        }
    }
    // non-member operations
    {
        // comparisons
        tstd::deque<int> dq1(vec.begin(), vec.end());
        tstd::deque<int> dq2(vec.begin(), vec.end());
        util.assertEqual(dq1 == dq2, true);
        util.assertEqual(dq1 != dq2, false);
        dq1[0] = -1;
        util.assertEqual(dq1 < dq2, true);
        util.assertEqual(dq1 <= dq2, true);
        util.assertEqual(dq1 > dq2, false);
        util.assertEqual(dq1 >= dq2, false);
    }
    util.showFinalResult();
}
