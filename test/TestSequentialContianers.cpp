#include <iostream>
#include <vector>
#include <array>
#include <iterator>
#include <tmemory.hpp>
#include <tvector.hpp>
#include <tarray.hpp>
#include <algorithm>
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
    std::cout << std::endl;
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
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testVector(showDetails);
    testArray(showDetails);
    return 0;
}
