#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <random>
#include <talgorithm.hpp>
#include <tnumeric.hpp>
#include "TestUtil.hpp"

void testNumericAlgorithms(bool showDetails);
void testNonModifyingSequenceAlgorithms(bool showDetails);
void testModifyingSequenceAlgorithms(bool showDetails);
void testPartitioningAlgorithms(bool showDetails);
void testSortingAlgorithms(bool showDetails);
void testBinarySearchAlgorihms(bool showDetails);
void testSortedRangeAlgorithms(bool showDetails);
void testSetAlgorithms(bool showDetails);
void testHeapAlgorithms(bool showDetails);
void testMinimumMaximumAlgorithms(bool showDetails);
void testComparisonAlgorithms(bool showDetails);
void testPermutationAlgorithms(bool showDetails);

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    // <tnumeric.hpp>
    testNumericAlgorithms(showDetails);
    // <talgorithm.hpp>
    testNonModifyingSequenceAlgorithms(showDetails);
    testModifyingSequenceAlgorithms(showDetails);
    testPartitioningAlgorithms(showDetails);
    testSortingAlgorithms(showDetails);
    testBinarySearchAlgorihms(showDetails);
    testSortedRangeAlgorithms(showDetails);
    testSetAlgorithms(showDetails);
    testHeapAlgorithms(showDetails);
    testMinimumMaximumAlgorithms(showDetails);
    testComparisonAlgorithms(showDetails);
    testPermutationAlgorithms(showDetails);
    std::cout << std::endl;
    return 0;
}

void testNumericAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "numeric algorithms");
    
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());
    // iota
    {
        std::vector<int> vec1(100), vec2(100);
        std::iota(vec1.begin(), vec1.end(), 1);
        tstd::iota(vec2.begin(), vec2.end(), 1);
        util.assertSequenceEqual(vec1, vec2);
    }
    // accumulate
    {
        // 1
        auto res1 = std::accumulate(vec.begin(), vec.end(), 0);
        auto res2 = tstd::accumulate(vec.begin(), vec.end(), 0);
        util.assertEqual(res1, res2);
        // 2
        auto op = [](int a, int b) -> int { return a + b - 2; };
        res1 = std::accumulate(vec.begin(), vec.end(), 0, op);
        res2 = tstd::accumulate(vec.begin(), vec.end(), 0, op);
        util.assertEqual(res1, res2);
    }
    // inner_product
    {
        std::vector<int> tmp(100);
        std::iota(tmp.begin(), tmp.end(), 1);
        // 1
        auto res1 = std::inner_product(vec.begin(), vec.end(), tmp.begin(), 0);
        auto res2 = tstd::inner_product(vec.begin(), vec.end(), tmp.begin(), 0);
        util.assertEqual(res1, res2);
        // 2
        auto op1 = std::plus<int>();
        auto op2 = [](int a, int b) -> int { return a * b - 2; };
        res1 = std::inner_product(vec.begin(), vec.end(), tmp.begin(), 0, op1, op2);
        res2 = tstd::inner_product(vec.begin(), vec.end(), tmp.begin(), 0, op1, op2);
        util.assertEqual(res1, res2);
    }
    // ajacent_difference
    {
        // 1
        std::vector<int> res1(100), res2(100);
        auto iter1 = std::adjacent_difference(vec.begin(), vec.end(), res1.begin());
        auto iter2 = tstd::adjacent_difference(vec.begin(), vec.end(), res2.begin());
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
        // 2
        auto op = [](int a, int b) -> int { return a + b - 2; };
        iter1 = std::adjacent_difference(vec.begin(), vec.end(), res1.begin(), op);
        iter2 = tstd::adjacent_difference(vec.begin(), vec.end(), res2.begin(), op);
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
    }
    // partial_sum
    {
        // 1
        std::vector<int> res1(100), res2(100);
        auto iter1 = std::partial_sum(vec.begin(), vec.end(), res1.begin());
        auto iter2 = tstd::partial_sum(vec.begin(), vec.end(), res2.begin());
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
        // 2
        auto op = [](int a, int b) -> int { return a + b - 2; };
        iter1 = std::partial_sum(vec.begin(), vec.end(), res1.begin(), op);
        iter2 = tstd::partial_sum(vec.begin(), vec.end(), res2.begin(), op);
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
    }
    // reduce
    {
        // 1
        auto res1 = std::reduce(vec.begin(), vec.end());
        auto res2 = tstd::reduce(vec.begin(), vec.end());
        util.assertEqual(res1, res2);
        // 2
        res1 = std::reduce(vec.begin(), vec.end(), 99);
        res2 = tstd::reduce(vec.begin(), vec.end(), 99);
        util.assertEqual(res1, res2);
        // 3
        auto op = [](int a, int b) -> int { return a + b - 2; };
        res1 = std::reduce(vec.begin(), vec.end(), 99, op);
        res2 = tstd::reduce(vec.begin(), vec.end(), 99, op);
        util.assertEqual(res1, res2);
    }
    // exclusive_scan
    {
        std::vector<int> res1(100), res2(100);
        // 1
        auto iter1 = std::exclusive_scan(vec.begin(), vec.end(), res1.begin(), 99);
        auto iter2 = tstd::exclusive_scan(vec.begin(), vec.end(), res2.begin(), 99);
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
        // 2
        auto op = [](int a, int b) -> int { return a + b - 2; };
        iter1 = std::exclusive_scan(vec.begin(), vec.end(), res1.begin(), 99, op);
        iter2 = tstd::exclusive_scan(vec.begin(), vec.end(), res2.begin(), 99, op);
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
    }
    // inclusive_scan
    {
        std::vector<int> res1(100), res2(100);
        // 1
        auto iter1 = std::inclusive_scan(vec.begin(), vec.end(), res1.begin());
        auto iter2 = tstd::inclusive_scan(vec.begin(), vec.end(), res2.begin());
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
        // 2
        auto op = [](int a, int b) -> int { return a + b - 2; };
        iter1 = std::inclusive_scan(vec.begin(), vec.end(), res1.begin(), op);
        iter2 = tstd::inclusive_scan(vec.begin(), vec.end(), res2.begin(), op);
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
        // 3
        iter1 = std::inclusive_scan(vec.begin(), vec.end(), res1.begin(), op, 99);
        iter2 = tstd::inclusive_scan(vec.begin(), vec.end(), res2.begin(), op, 99);
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
    }
    // transform_reduce
    {
        std::vector<int> tmp(100);
        std::iota(tmp.begin(), tmp.end(), 1);
        // 1
        auto res1 = std::transform_reduce(vec.begin(), vec.end(), tmp.begin(), 10);
        auto res2 = tstd::transform_reduce(vec.begin(), vec.end(), tmp.begin(), 10);
        util.assertEqual(res1, res2);
        // 2
        // careful: op1 should be combined and swappable
        auto op1 = [](int a, int b) -> int { return a + b - 1; };
        auto op2 = [](int a, int b) -> int { return a + b - 2; };
        res1 = std::transform_reduce(vec.begin(), vec.end(), tmp.begin(), 10, op1, op2);
        res2 = tstd::transform_reduce(vec.begin(), vec.end(), tmp.begin(), 10, op1, op2);
        util.assertEqual(res1, res2);
        // 3
        auto uop = [](int a) -> int { return 2 * a - 1; };
        res1 = std::transform_reduce(vec.begin(), vec.end(), 10, op1, uop);
        res2 = tstd::transform_reduce(vec.begin(), vec.end(), 10, op1, uop);
        util.assertEqual(res1, res2);
    }
    // transform_exclusive_scan
    {
        std::vector<int> res1(100), res2(100);
        auto uop = [](int a) -> int { return 2 * a - 1; };
        auto bop = [](int a, int b) -> int { return a + b - 1; };
        auto iter1 = std::transform_exclusive_scan(vec.begin(), vec.end(), res1.begin(), 10, bop, uop);
        auto iter2 = tstd::transform_exclusive_scan(vec.begin(), vec.end(), res2.begin(), 10, bop, uop);
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
    }
    // transform_inclusive_scan
    {
        std::vector<int> res1(100), res2(100);
        auto uop = [](int a) -> int { return 2 * a - 1; };
        auto bop = [](int a, int b) -> int { return a + b - 1; };
        // 1
        auto iter1 = std::transform_inclusive_scan(vec.begin(), vec.end(), res1.begin(), bop, uop);
        auto iter2 = tstd::transform_inclusive_scan(vec.begin(), vec.end(), res2.begin(), bop, uop);
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
        // 2
        iter1 = std::transform_inclusive_scan(vec.begin(), vec.end(), res1.begin(), bop, uop, 10);
        iter2 = tstd::transform_inclusive_scan(vec.begin(), vec.end(), res2.begin(), bop, uop, 10);
        util.assertEqual(iter1 == res1.end(), iter2 == res2.end());
        util.assertSequenceEqual(res1, res2);
    }
    util.showFinalResult();
}

void testNonModifyingSequenceAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "non-modifying sequence algorithms");
    {

    }
    util.showFinalResult();
}

void testModifyingSequenceAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "modifying sequence algorithms");
    {

    }
    util.showFinalResult();
}

void testPartitioningAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "partitioning algorithms");
    {

    }
    util.showFinalResult();
}

void testSortingAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "sorting algorithms");
    {

    }
    util.showFinalResult();
}

void testBinarySearchAlgorihms(bool showDetails)
{
    TestUtil util(showDetails, "binary search algorithms");
    {

    }
    util.showFinalResult();
}

void testSortedRangeAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "sorted range algorithms");
    {

    }
    util.showFinalResult();
}

void testSetAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "set algorithms");
    {

    }
    util.showFinalResult();
}


void testHeapAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "heap algorithms");

    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());
    {
        // is_heap_until
        std::vector<int> v(vec);
        auto res1 = tstd::is_heap_until(v.begin(), v.end());
        auto res2 = std::is_heap_until(v.begin(), v.end());
        util.assertEqual(res1 == res2, true);
        res1 = tstd::is_heap_until(v.begin(), v.end(), std::greater<int>());
        res2 = std::is_heap_until(v.begin(), v.end(), std::greater<int>());
        util.assertEqual(res1 == res2, true);
    }
    {
        // is_heap
        std::vector<int> v(vec);
        util.assertEqual(tstd::is_heap(vec.begin(), vec.end()), std::is_heap(vec.begin(), vec.end()));
        util.assertEqual(tstd::is_heap(vec.begin(), vec.end(), std::greater<int>()), std::is_heap(vec.begin(), vec.end(), std::greater<int>()));
        std::sort(vec.begin(), vec.end());
        util.assertEqual(tstd::is_heap(vec.begin(), vec.end()), std::is_heap(vec.begin(), vec.end()));
        util.assertEqual(tstd::is_heap(vec.begin(), vec.end(), std::greater<int>()), std::is_heap(vec.begin(), vec.end(), std::greater<int>()));
        std::sort(vec.begin(), vec.end(), std::greater<int>());
        util.assertEqual(tstd::is_heap(vec.begin(), vec.end()), std::is_heap(vec.begin(), vec.end()));
        util.assertEqual(tstd::is_heap(vec.begin(), vec.end(), std::greater<int>()), std::is_heap(vec.begin(), vec.end(), std::greater<int>()));
    }
    {
        // push_heap, pop_heap
        std::vector<int> v1(vec);
        std::vector<int> v2(vec);
        for (auto iter = v1.begin(); iter <= v1.end(); ++iter)
        {
            tstd::push_heap(v1.begin(), iter);
        }
        for (auto iter = v2.begin(); iter <= v2.end(); ++iter)
        {
            std::push_heap(v2.begin(), iter);
        }
        util.assertSequenceEqual(v1, v2);
        for (auto iter = v1.end(); iter != v1.begin() + v1.size() / 2; --iter)
        {
            tstd::pop_heap(v1.begin(), iter);
        }
        for (auto iter = v2.end(); iter != v2.begin() + v2.size() / 2; --iter)
        {
            std::pop_heap(v2.begin(), iter);
        }
        util.assertSequenceEqual(v1, v2);
        // compare
        for (auto iter = v1.begin(); iter <= v1.end(); ++iter)
        {
            tstd::push_heap(v1.begin(), iter, std::greater<int>());
        }
        for (auto iter = v2.begin(); iter <= v2.end(); ++iter)
        {
            std::push_heap(v2.begin(), iter, std::greater<int>());
        }
        util.assertSequenceEqual(v1, v2);
        for (auto iter = v1.end(); iter != v1.begin() + v1.size() / 2; --iter)
        {
            tstd::pop_heap(v1.begin(), iter, std::greater<int>());
        }
        for (auto iter = v2.end(); iter != v2.begin() + v2.size() / 2; --iter)
        {
            std::pop_heap(v2.begin(), iter, std::greater<int>());
        }
        util.assertSequenceEqual(v1, v2);
    }
    {
        // make_heap, sort_heap
        std::vector<int> v1(vec);
        std::vector<int> v2(vec);
        tstd::make_heap(v1.begin(), v1.end());
        std::make_heap(v2.begin(), v2.end());
        util.assertSequenceEqual(v1, v2);
        tstd::sort_heap(v1.begin(), v1.end());
        std::sort_heap(v2.begin(), v2.end());
        util.assertSequenceEqual(v1, v2);

        tstd::make_heap(v1.begin(), v1.end(), std::greater<int>());
        std::make_heap(v2.begin(), v2.end(), std::greater<int>());
        util.assertSequenceEqual(v1, v2);
        tstd::sort_heap(v1.begin(), v1.end(), std::greater<int>());
        std::sort_heap(v2.begin(), v2.end(), std::greater<int>());
        util.assertSequenceEqual(v1, v2);
    }
    util.showFinalResult();
}

void testMinimumMaximumAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "minimum/maximum algorithms");
    {

    }
    util.showFinalResult();
}

void testComparisonAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "comparison algorithms");
    {

    }
    util.showFinalResult();
}

void testPermutationAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "permutation algorithms");
    {

    }
    util.showFinalResult();
}