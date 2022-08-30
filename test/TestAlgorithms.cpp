#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <forward_list>
#include <numeric>
#include <random>
#include <iterator>
#include <string>
#include <talgorithm.hpp>
#include <tnumeric.hpp>
#include <tvector.hpp>
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

    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());
    // all_of
    {
        auto res1 = std::all_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 10; });
        auto res2 = tstd::all_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 10; });
        util.assertEqual(res1, res2);
        res1 = std::all_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 0; });
        res2 = tstd::all_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 0; });
        util.assertEqual(res1, res2);
        res1 = std::all_of(vec.begin(), vec.end(), [](int a) -> bool { return a <= 100; });
        res2 = tstd::all_of(vec.begin(), vec.end(), [](int a) -> bool { return a <= 100; });
        util.assertEqual(res1, res2);
    }
    // any_of
    {
        auto res1 = std::any_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 10; });
        auto res2 = tstd::any_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 10; });
        util.assertEqual(res1, res2);
        res1 = std::any_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 0; });
        res2 = tstd::any_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 0; });
        util.assertEqual(res1, res2);
        res1 = std::any_of(vec.begin(), vec.end(), [](int a) -> bool { return a <= 0; });
        res2 = tstd::any_of(vec.begin(), vec.end(), [](int a) -> bool { return a <= 0; });
        util.assertEqual(res1, res2);
    }
    // none_of
    {
        auto res1 = std::none_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 10; });
        auto res2 = tstd::none_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 10; });
        util.assertEqual(res1, res2);
        res1 = std::none_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 0; });
        res2 = tstd::none_of(vec.begin(), vec.end(), [](int a) -> bool { return a > 0; });
        util.assertEqual(res1, res2);
        res1 = std::none_of(vec.begin(), vec.end(), [](int a) -> bool { return a <= 0; });
        res2 = tstd::none_of(vec.begin(), vec.end(), [](int a) -> bool { return a <= 0; });
        util.assertEqual(res1, res2);
    }
    // for_each
    {
        int sum1 = 1;
        int sum2 = 1;
        std::for_each(vec.begin(), vec.end(), [&sum1](int a) { sum1 += a; });
        tstd::for_each(vec.begin(), vec.end(), [&sum2](int a) { sum2 += a; });
        util.assertEqual(sum1, sum2);
    }
    // for_each_n
    {
        int sum1 = 1;
        int sum2 = 1;
        auto iter1 = std::for_each_n(vec.begin(), 10, [&sum1](int a) { sum1 += a; });
        auto iter2 = tstd::for_each_n(vec.begin(), 10, [&sum2](int a) { sum2 += a; });
        util.assertEqual(sum1, sum2);
        util.assertEqual(iter1 == iter2, true);
    }
    // count
    {
        auto res1 = std::count(vec.begin(), vec.end(), 0);
        auto res2 = tstd::count(vec.begin(), vec.end(), 0);
        util.assertEqual(res1, res2);
        res1 = std::count(vec.begin(), vec.end(), 10);
        res2 = tstd::count(vec.begin(), vec.end(), 10);
        util.assertEqual(res1, res2);
    }
    // count_if
    {
        auto res1 = std::count_if(vec.begin(), vec.end(), [](int a) -> bool { return a > 20; });
        auto res2 = tstd::count_if(vec.begin(), vec.end(), [](int a) -> bool { return a > 20; });
        util.assertEqual(res1, res2);
    }
    // mismatch
    {
        std::vector<int> tmp(vec);
        tmp[30] = -1;
        // 1
        auto p1 = std::mismatch(vec.begin(), vec.end(), tmp.begin());
        auto p2 = tstd::mismatch(vec.begin(), vec.end(), tmp.begin());
        util.assertEqual(p1 == p2, true);
        // 2
        p1 = std::mismatch(vec.begin(), vec.end(), tmp.begin(), std::equal_to<int>());
        p2 = tstd::mismatch(vec.begin(), vec.end(), tmp.begin(), std::equal_to<int>());
        util.assertEqual(p1 == p2, true);
        // 3
        p1 = std::mismatch(vec.begin(), vec.end(), tmp.begin(), tmp.end());
        p2 = tstd::mismatch(vec.begin(), vec.end(), tmp.begin(), tmp.end());
        util.assertEqual(p1 == p2, true);
        // 4
        p1 = std::mismatch(vec.begin(), vec.end(), tmp.begin(), tmp.end(), std::equal_to<int>());
        p2 = tstd::mismatch(vec.begin(), vec.end(), tmp.begin(), tmp.end(), std::equal_to<int>());
        util.assertEqual(p1 == p2, true);
    }
    // find
    {
        auto iter1 = std::find(vec.begin(), vec.end(), 10);
        auto iter2 = tstd::find(vec.begin(), vec.end(), 10);
        util.assertEqual(iter1 == iter2, true);
    }
    // find_if
    {
        auto iter1 = std::find_if(vec.begin(), vec.end(), [](int a) -> bool { return a > 40; });
        auto iter2 = tstd::find_if(vec.begin(), vec.end(), [](int a) -> bool { return a > 40; });
        util.assertEqual(iter1 == iter2, true);
    }
    // find_if_not
    {
        auto iter1 = std::find_if_not(vec.begin(), vec.end(), [](int a) -> bool { return a > 40; });
        auto iter2 = tstd::find_if_not(vec.begin(), vec.end(), [](int a) -> bool { return a > 40; });
        util.assertEqual(iter1 == iter2, true);
    }
    // find_end
    {
        std::vector<int> tmp(100, 1);
        tmp.resize(200, 2);
        tmp.resize(300, 3);
        std::shuffle(tmp.begin(), tmp.end(), std::mt19937());
        std::vector<int> seq{1, 2, 3};
        // 1
        auto iter1 = std::find_end(tmp.begin(), tmp.end(), seq.begin(), seq.end());
        auto iter2 = tstd::find_end(tmp.begin(), tmp.end(), seq.begin(), seq.end());
        util.assertEqual(iter1 - tmp.begin(), iter2 - tmp.begin());
        util.assertEqual(iter1 == iter2, true);
        // 2
        iter1 = std::find_end(tmp.begin(), tmp.end(), seq.begin(), seq.end(), std::equal_to<int>());
        iter2 = tstd::find_end(tmp.begin(), tmp.end(), seq.begin(), seq.end(), std::equal_to<int>());
        util.assertEqual(iter1 - tmp.begin(), iter2 - tmp.begin());
        util.assertEqual(iter1 == iter2, true);
    }
    // find_first_of
    {
        // 1
        std::vector<int> tmp{1, 2, 3, 4, 5};
        auto iter1 = std::find_first_of(vec.begin(), vec.end(), tmp.begin(), tmp.end());
        auto iter2 = tstd::find_first_of(vec.begin(), vec.end(), tmp.begin(), tmp.end());
        util.assertEqual(iter1 - vec.begin(), iter2 - vec.begin());
        util.assertEqual(iter1 == iter2, true);
        // 2
        iter1 = std::find_first_of(vec.begin(), vec.end(), tmp.begin(), tmp.end(), std::equal_to<int>());
        iter2 = tstd::find_first_of(vec.begin(), vec.end(), tmp.begin(), tmp.end(), std::equal_to<int>());
        util.assertEqual(iter1 - vec.begin(), iter2 - vec.begin());
        util.assertEqual(iter1 == iter2, true);
    }
    // adjacent_find
    {
        std::vector<int> tmp(vec);
        tmp.resize(120, 1);
        std::shuffle(tmp.begin(), tmp.end(), std::mt19937());
        // 1
        auto iter1 = std::adjacent_find(tmp.begin(), tmp.end());
        auto iter2 = tstd::adjacent_find(tmp.begin(), tmp.end());
        util.assertEqual(iter1 - tmp.begin(), iter2 - tmp.begin());
        util.assertEqual(iter1 == iter2, true);
        // 2
        iter1 = std::adjacent_find(tmp.begin(), tmp.end(), std::equal_to<int>());
        iter2 = tstd::adjacent_find(tmp.begin(), tmp.end(), std::equal_to<int>());
        util.assertEqual(iter1 - tmp.begin(), iter2 - tmp.begin());
        util.assertEqual(iter1 == iter2, true);
    }
    // search
    {
        std::vector<int> tmp(100, 1);
        tmp.resize(200, 2);
        tmp.resize(300, 3);
        std::shuffle(tmp.begin(), tmp.end(), std::mt19937());
        std::vector<int> seq{1, 2, 3};
        // 1
        auto iter1 = std::search(tmp.begin(), tmp.end(), seq.begin(), seq.end());
        auto iter2 = tstd::search(tmp.begin(), tmp.end(), seq.begin(), seq.end());
        util.assertEqual(iter1 - tmp.begin(), iter2 - tmp.begin());
        util.assertEqual(iter1 == iter2, true);
        // 2
        iter1 = std::search(tmp.begin(), tmp.end(), seq.begin(), seq.end(), std::equal_to<int>());
        iter2 = tstd::search(tmp.begin(), tmp.end(), seq.begin(), seq.end(), std::equal_to<int>());
        util.assertEqual(iter1 - tmp.begin(), iter2 - tmp.begin());
        util.assertEqual(iter1 == iter2, true);
        // 3: not implemented!
    }
    // search_n
    {
        std::vector<int> tmp(vec);
        tmp.resize(120, 1);
        std::shuffle(tmp.begin(), tmp.end(), std::mt19937());
        // 1
        auto iter1 = std::search_n(tmp.begin(), tmp.end(), 2, 1);
        auto iter2 = tstd::search_n(tmp.begin(), tmp.end(), 2, 1);
        util.assertEqual(iter1 - tmp.begin(), iter2 - tmp.begin());
        util.assertEqual(iter1 == iter2, true);
        // 2
        iter1 = std::search_n(tmp.begin(), tmp.end(), 2, 1, std::equal_to<int>());
        iter2 = tstd::search_n(tmp.begin(), tmp.end(), 2, 1, std::equal_to<int>());
        util.assertEqual(iter1 - tmp.begin(), iter2 - tmp.begin());
        util.assertEqual(iter1 == iter2, true);
    }
    util.showFinalResult();
}

void testModifyingSequenceAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "modifying sequence algorithms");

    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());
    // copy
    {
        std::vector<int> tmp1(100);
        std::vector<int> tmp2(100);
        std::copy(vec.begin(), vec.end(), tmp1.begin());
        tstd::copy(vec.begin(), vec.end(), tmp2.begin());
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // copy_if
    {
        std::vector<int> tmp1(50);
        std::vector<int> tmp2(50);
        std::copy_if(vec.begin(), vec.end(), tmp1.begin(), [](int a)->bool { return a <= 50; });
        tstd::copy_if(vec.begin(), vec.end(), tmp2.begin(), [](int a)->bool { return a <= 50; });
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // copy_n
    {
        std::vector<int> tmp1(50);
        std::vector<int> tmp2(50);
        std::copy_n(vec.begin(), 50, tmp1.begin());
        tstd::copy_n(vec.begin(), 50, tmp2.begin());
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // copy_backward
    {
        std::vector<int> tmp1(100);
        std::vector<int> tmp2(100);
        std::copy_backward(vec.begin(), vec.end(), tmp1.end());
        tstd::copy_backward(vec.begin(), vec.end(), tmp2.end());
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // move
    {
        std::vector<int> tmp1(100);
        std::vector<int> tmp2(100);
        std::move(vec.begin(), vec.end(), tmp1.begin());
        tstd::move(vec.begin(), vec.end(), tmp2.begin());
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // move_backward
    {
        std::vector<int> tmp1(100);
        std::vector<int> tmp2(100);
        std::move_backward(vec.begin(), vec.end(), tmp1.end());
        tstd::move_backward(vec.begin(), vec.end(), tmp2.end());
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // fill
    {
        std::vector<int> tmp1(100);
        std::vector<int> tmp2(100);
        std::fill(tmp1.begin(), tmp1.end(), 999);
        tstd::fill(tmp2.begin(), tmp2.end(), 999);
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // fill_n
    {
        std::vector<int> tmp1(50);
        std::vector<int> tmp2(50);
        std::fill_n(tmp1.begin(), 50, 999);
        tstd::fill_n(tmp2.begin(), 50, 999);
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // transform
    {
        std::vector<int> tmp1(100);
        std::vector<int> tmp2(100);
        std::transform(vec.begin(), vec.end(), tmp1.begin(), [](int a)->int { return a + 1; });
        tstd::transform(vec.begin(), vec.end(), tmp2.begin(), [](int a)->int { return a + 1; });
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // transform
    {
        std::vector<int> vec2(100);
        std::iota(vec2.begin(), vec2.end(), 1);
        std::vector<int> tmp1(100);
        std::vector<int> tmp2(100);
        std::transform(vec.begin(), vec.end(), vec2.begin(), tmp1.begin(), [](int a, int b)->int { return a * b + 1; });
        tstd::transform(vec.begin(), vec.end(), vec2.begin(), tmp2.begin(), [](int a, int b)->int { return a * b + 1; });
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // generate
    {
        std::vector<int> tmp1(100);
        std::vector<int> tmp2(100);
        int init = 100;
        auto g1 = [&init]() -> int { return init += 2; };
        std::generate(tmp1.begin(), tmp1.end(), g1);
        init = 100;
        auto g2 = [&init]() -> int { return init += 2; };
        tstd::generate(tmp2.begin(), tmp2.end(), g2);
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // generate_n
    {
        std::vector<int> tmp1(100);
        std::vector<int> tmp2(100);
        int init = 100;
        auto g1 = [&init]() -> int { return init += 2; };
        std::generate_n(tmp1.begin(), 100, g1);
        init = 100;
        auto g2 = [&init]() -> int { return init += 2; };
        tstd::generate_n(tmp2.begin(), 100, g2);
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // remove
    {
        std::vector<int> tmp1(vec);
        std::vector<int> tmp2(vec);
        auto iter1 = std::remove(tmp1.begin(), tmp1.end(), 10);
        auto iter2 = tstd::remove(tmp2.begin(), tmp2.end(), 10);
        util.assertEqual(iter1 - tmp1.begin(), iter2 - tmp2.begin());
        util.assertRangeEqual(tmp1.begin(), iter1, tmp2.begin());
    }
    // remove_if
    {
        std::vector<int> tmp1(vec);
        std::vector<int> tmp2(vec);
        auto p = [](int a) -> bool { return a < 50; };
        auto iter1 = std::remove_if(tmp1.begin(), tmp1.end(), p);
        auto iter2 = tstd::remove_if(tmp2.begin(), tmp2.end(), p);
        util.assertEqual(iter1 - tmp1.begin(), iter2 - tmp2.begin());
        util.assertRangeEqual(tmp1.begin(), iter1, tmp2.begin());
    }
    // remove_copy
    {
        std::vector<int> tmp1(vec), tmp1d(100);
        std::vector<int> tmp2(vec), tmp2d(100);
        auto iter1 = std::remove_copy(tmp1.begin(), tmp1.end(), tmp1d.begin(), 10);
        auto iter2 = tstd::remove_copy(tmp2.begin(), tmp2.end(), tmp2d.begin(), 10);
        util.assertEqual(iter1 - tmp1d.begin(), iter2 - tmp2d.begin());
        util.assertRangeEqual(tmp1d.begin(), iter1, tmp2d.begin());
    }
    // remove_copy_if
    {
        std::vector<int> tmp1(vec), tmp1d(100);
        std::vector<int> tmp2(vec), tmp2d(100);
        auto p = [](int a) -> bool { return a < 50; };
        auto iter1 = std::remove_copy_if(tmp1.begin(), tmp1.end(), tmp1d.begin(), p);
        auto iter2 = tstd::remove_copy_if(tmp2.begin(), tmp2.end(), tmp2d.begin(), p);
        util.assertEqual(iter1 - tmp1d.begin(), iter2 - tmp2d.begin());
        util.assertRangeEqual(tmp1d.begin(), iter1, tmp2d.begin());
    }
    // replace
    {
        std::vector<int> tmp1(vec);
        std::vector<int> tmp2(vec);
        std::replace(tmp1.begin(), tmp1.end(), 10, 110);
        tstd::replace(tmp2.begin(), tmp2.end(), 10, 110);
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // replace_if
    {
        std::vector<int> tmp1(vec);
        std::vector<int> tmp2(vec);
        auto p = [](int a) -> bool { return a < 50; };
        std::replace_if(tmp1.begin(), tmp1.end(), p, 110);
        tstd::replace_if(tmp2.begin(), tmp2.end(), p, 110);
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // replace_copy
    {
        std::vector<int> tmp1(vec), tmp1d(100);
        std::vector<int> tmp2(vec), tmp2d(100);
        std::replace_copy(tmp1.begin(), tmp1.end(), tmp1d.begin(), 10, 110);
        tstd::replace_copy(tmp2.begin(), tmp2.end(), tmp2d.begin(), 10, 110);
        util.assertSequenceEqual(tmp1d, tmp2d);
    }
    // replace_copy_if
    {
        std::vector<int> tmp1(vec), tmp1d(100);
        std::vector<int> tmp2(vec), tmp2d(100);
        auto p = [](int a) -> bool { return a < 50; };
        std::replace_copy_if(tmp1.begin(), tmp1.end(), tmp1d.begin(), p, 110);
        tstd::replace_copy_if(tmp2.begin(), tmp2.end(), tmp2d.begin(), p, 110);
        util.assertSequenceEqual(tmp1d, tmp2d);
    }
    // swap
    {
        int a1 = 1, b1 = 2;
        int a2 = 1, b2 = 2;
        std::swap(a1, b1);
        tstd::swap(a2, b2);
        util.assertEqual(a1, a2);
        util.assertEqual(b1, b2);
        {
            // for array
            int a1[3] = {1, 2, 3}, b1[3] = {4, 5, 6};
            int a2[3] = {1, 2, 3}, b2[3] = {4, 5, 6};
            std::swap(a1, b1);
            tstd::swap(a2, b2);
            util.assertArrayEqual(a1, a2, 3);
            util.assertArrayEqual(b1, b2, 3);
        }
    }
    // swap_ranges
    {
        std::vector<int> tmp1(vec), tmp1c(100);
        std::vector<int> tmp2(vec), tmp2c(100);
        std::swap_ranges(tmp1.begin(), tmp1.end(), tmp1c.begin());
        tstd::swap_ranges(tmp2.begin(), tmp2.end(), tmp2c.begin());
        util.assertSequenceEqual(tmp1, tmp2);
        util.assertSequenceEqual(tmp1c, tmp2c);
    }
    // iter_swap
    {
        std::vector<int> tmp1(vec);
        std::vector<int> tmp2(vec);
        std::iter_swap(tmp1.begin(), tmp1.end() - 1);
        tstd::iter_swap(tmp2.begin(), tmp2.end() - 1);
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // reverse
    {
        // random access iteraor
        std::vector<int> tmp1(vec);
        std::vector<int> tmp2(vec);
        std::reverse(tmp1.begin(), tmp1.end());
        tstd::reverse(tmp2.begin(), tmp2.end());
        util.assertSequenceEqual(tmp1, tmp2);
        // bidirectional iterator
        std::list<int> tmpl1(vec.begin(), vec.end());
        std::list<int> tmpl2(vec.begin(), vec.end());
        std::reverse(tmpl1.begin(), tmpl1.end());
        tstd::reverse(tmpl2.begin(), tmpl2.end());
        util.assertSequenceEqual(tmpl1, tmpl2);
    }
    // reverse_copy
    {
        std::list<int> tmpl1(100);
        std::list<int> tmpl2(100);
        std::reverse_copy(vec.begin(), vec.end(), tmpl1.begin());
        tstd::reverse_copy(vec.begin(), vec.end(), tmpl2.begin());
        util.assertSequenceEqual(tmpl1, tmpl2);
    }
    // rotate
    {
        std::forward_list<int> tmp1(vec.begin(), vec.end());
        std::forward_list<int> tmp2(vec.begin(), vec.end());
        auto iter1 = std::rotate(tmp1.begin(), std::next(tmp1.begin(), 50), tmp1.end());
        auto iter2 = tstd::rotate(tmp2.begin(), std::next(tmp2.begin(), 50), tmp2.end());
        util.assertSequenceEqual(tmp1, tmp2);
        util.assertEqual(std::distance(tmp1.begin(), iter1), std::distance(tmp2.begin(), iter2));
    }
    // rotate_copy
    {
        std::forward_list<int> tmp1(100);
        std::forward_list<int> tmp2(100);
        auto iter1 = std::rotate_copy(vec.begin(), vec.begin() + 50, vec.end(), tmp1.begin());
        auto iter2 = tstd::rotate_copy(vec.begin(), vec.begin() + 50, vec.end(), tmp2.begin());
        util.assertSequenceEqual(tmp1, tmp2);
        util.assertEqual(std::distance(tmp1.begin(), iter1), std::distance(tmp2.begin(), iter2));
    }
    // shift_left
    {
        std::forward_list<int> tmp1(vec.begin(), vec.end());
        std::forward_list<int> tmp2(vec.begin(), vec.end());
        auto iter1 = std::shift_left(tmp1.begin(), tmp1.end(), 50);
        auto iter2 = tstd::shift_left(tmp2.begin(), tmp2.end(), 50);
        util.assertSequenceEqual(tmp1, tmp2);
        util.assertEqual(std::distance(tmp1.begin(), iter1), std::distance(tmp2.begin(), iter2));
    }
    // shift_right
    {
        // bidirectional iterator
        {
            std::list<int> tmp1(vec.begin(), vec.end());
            std::list<int> tmp2(vec.begin(), vec.end());
            auto iter1 = std::shift_right(tmp1.begin(), tmp1.end(), 50);
            auto iter2 = tstd::shift_right(tmp2.begin(), tmp2.end(), 50);
            util.assertEqual(std::distance(tmp1.begin(), iter1), std::distance(tmp2.begin(), iter2));
            util.assertSequenceEqual(tmp1, tmp2);
        }
        // forward iterator
        {
            std::forward_list<int> tmp1(vec.begin(), vec.end());
            std::forward_list<int> tmp2(vec.begin(), vec.end());
            auto iter1 = std::shift_right(tmp1.begin(), tmp1.end(), 50);
            auto iter2 = tstd::shift_right(tmp2.begin(), tmp2.end(), 50);
            util.assertEqual(std::distance(tmp1.begin(), iter1), std::distance(tmp2.begin(), iter2));
            util.assertSequenceEqual(tmp1, tmp2);
        }
    }
    // random_shuffle
    {
        std::vector<int> tmp1(100);
        std::vector<int> tmp2(100);
        // 1
        std::iota(tmp1.begin(), tmp1.end(), 1);
        std::iota(tmp2.begin(), tmp2.end(), 1);
        std::random_shuffle(tmp1.begin(), tmp1.end());
        tstd::random_shuffle(tmp2.begin(), tmp2.end());
        if (showDetails)
        {
            std::cout << "random_shuffle test 1: " << std::endl;
            std::cout << "\tstd::random_shuffle: " << printContainerElememts(tmp1, 100) << std::endl;
            std::cout << "\ttstd::random_shuffle: " << printContainerElememts(tmp2, 100) << std::endl;
        }
        util.assertSetEqual(tmp1, tmp2);
        // 2
        auto gen = [](int a) -> int { return std::rand() % a; };
        std::iota(tmp1.begin(), tmp1.end(), 1);
        std::iota(tmp2.begin(), tmp2.end(), 1);
        std::random_shuffle(tmp1.begin(), tmp1.end(), gen);
        tstd::random_shuffle(tmp2.begin(), tmp2.end(), gen);
        if (showDetails)
        {
            std::cout << "random_shuffle test 2: " << std::endl;
            std::cout << "\tstd::random_shuffle: " << printContainerElememts(tmp1, 100) << std::endl;
            std::cout << "\ttstd::random_shuffle: " << printContainerElememts(tmp2, 100) << std::endl;
        }
        util.assertSetEqual(tmp1, tmp2);
    }
    // shuffle
    {
        std::vector<int> tmp1(100);
        std::vector<int> tmp2(100);
        std::iota(tmp1.begin(), tmp1.end(), 1);
        std::iota(tmp2.begin(), tmp2.end(), 1);
        std::shuffle(tmp1.begin(), tmp1.end(), std::mt19937());
        tstd::shuffle(tmp2.begin(), tmp2.end(), std::mt19937());
        if (showDetails)
        {
            std::cout << "shuffle test: " << std::endl;
            std::cout << "\tstd::shuffle: " << printContainerElememts(tmp1, 100) << std::endl;
            std::cout << "\ttstd::shuffle: " << printContainerElememts(tmp2, 100) << std::endl;
        }
        util.assertSetEqual(tmp1, tmp2);
    }
    // sample
    {
        std::vector<int> tmp1(50);
        std::vector<int> tmp2(50);
        std::sample(vec.begin(), vec.end(), tmp1.begin(), 50, std::mt19937());
        tstd::sample(vec.begin(), vec.end(), tmp2.begin(), 50, std::mt19937());
        if (showDetails)
        {
            std::cout << "sample test: " << std::endl;
            std::cout << "\tstd::sample: " << printContainerElememts(tmp1, 50) << std::endl;
            std::cout << "\ttstd::sample: " << printContainerElememts(tmp2, 50) << std::endl;
        }
        // can not assert here!
    }
    // unique
    {
        // 1
        {
            std::vector<int> tmp1{1, 2, 2, 1, 2, 2, 1, 1, 1, 3, 3, 3, 2, 5};
            std::vector<int> tmp2{1, 2, 2, 1, 2, 2, 1, 1, 1, 3, 3, 3, 2, 5};
            auto iter1 = std::unique(tmp1.begin(), tmp1.end());
            auto iter2 = tstd::unique(tmp2.begin(), tmp2.end());
            util.assertEqual(std::distance(tmp1.begin(), iter1), std::distance(tmp2.begin(), iter2));
            util.assertSequenceEqual(tmp1, tmp2);
        }
        // 2
        {
            std::vector<int> tmp1{1, 2, 2, 1, 2, 2, 1, 1, 1, 3, 3, 3, 2, 5};
            std::vector<int> tmp2{1, 2, 2, 1, 2, 2, 1, 1, 1, 3, 3, 3, 2, 5};
            auto p = [](int a, int b) -> bool { return a == b; };
            auto iter1 = std::unique(tmp1.begin(), tmp1.end(), p);
            auto iter2 = tstd::unique(tmp2.begin(), tmp2.end(), p);
            util.assertEqual(std::distance(tmp1.begin(), iter1), std::distance(tmp2.begin(), iter2));
            util.assertSequenceEqual(tmp1, tmp2);
        }
    }
    // unique_copy
    {
        // 1
        {
            std::vector<int> tmp1{1, 2, 2, 1, 2, 2, 1, 1, 1, 3, 3, 3, 2, 5};
            std::vector<int> tmp2{1, 2, 2, 1, 2, 2, 1, 1, 1, 3, 3, 3, 2, 5};
            std::vector<int> tmp1c(tmp1.size());
            std::vector<int> tmp2c(tmp2.size());
            auto iter1 = std::unique_copy(tmp1.begin(), tmp1.end(), tmp1c.begin());
            auto iter2 = tstd::unique_copy(tmp2.begin(), tmp2.end(), tmp2c.begin());
            util.assertEqual(std::distance(tmp1c.begin(), iter1), std::distance(tmp2c.begin(), iter2));
            util.assertSequenceEqual(tmp1c, tmp2c);
        }
        // 2
        {
            std::vector<int> tmp1{1, 2, 2, 1, 2, 2, 1, 1, 1, 3, 3, 3, 2, 5};
            std::vector<int> tmp2{1, 2, 2, 1, 2, 2, 1, 1, 1, 3, 3, 3, 2, 5};
            std::vector<int> tmp1c(tmp1.size());
            std::vector<int> tmp2c(tmp2.size());
            auto p = [](int a, int b) -> bool { return a == b; };
            auto iter1 = std::unique_copy(tmp1.begin(), tmp1.end(), tmp1c.begin(), p);
            auto iter2 = tstd::unique_copy(tmp2.begin(), tmp2.end(), tmp2c.begin(), p);
            util.assertEqual(std::distance(tmp1c.begin(), iter1), std::distance(tmp2c.begin(), iter2));
            util.assertSequenceEqual(tmp1c, tmp2c);
        }
    }
    util.showFinalResult();
}

void testPartitioningAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "partitioning algorithms");

    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());
    // is_partitioned
    {
        auto res1 = std::is_partitioned(vec.begin(), vec.end(), [](int a) { return a > 0; });
        auto res2 = tstd::is_partitioned(vec.begin(), vec.end(), [](int a) { return a > 0; });
        util.assertEqual(res1, res2);
        res1 = std::is_partitioned(vec.begin(), vec.end(), [](int a) { return a < 0; });
        res2 = tstd::is_partitioned(vec.begin(), vec.end(), [](int a) { return a < 0; });
        util.assertEqual(res1, res2);
        res1 = std::is_partitioned(vec.begin(), vec.end(), [](int a) { return a < 50; });
        res2 = tstd::is_partitioned(vec.begin(), vec.end(), [](int a) { return a < 50; });
        util.assertEqual(res1, res2);
        std::vector<int> tmp{1, 1, 2, 2, 3, 3, 4};
        res1 = std::is_partitioned(tmp.begin(), tmp.end(), [](int a) { return a < 3; });
        res2 = tstd::is_partitioned(tmp.begin(), tmp.end(), [](int a) { return a < 3; });
        util.assertEqual(res1, res2);
        res1 = std::is_partitioned(tmp.begin(), tmp.end(), [](int a) { return a > 4; });
        res2 = tstd::is_partitioned(tmp.begin(), tmp.end(), [](int a) { return a > 4; });
        util.assertEqual(res1, res2);
    }
    // partition
    {
        std::vector<int> tmp1(vec);
        std::vector<int> tmp2(vec);
        auto p = [](int a) -> bool { return a > 50; };
        auto iter1 = std::partition(tmp1.begin(), tmp1.end(), p);
        auto iter2 = tstd::partition(tmp2.begin(), tmp2.end(), p);
        util.assertEqual(std::distance(tmp1.begin(), iter1), std::distance(tmp2.begin(), iter2));
        util.assertSetEqual(tmp1, tmp2); // not guarantee sequence equality
        util.assertEqual(std::is_partitioned(tmp1.begin(), tmp1.end(), p), tstd::is_partitioned(tmp2.begin(), tmp2.end(), p));
        {
            auto p = [](int a) -> bool { return a < 50; };
            auto iter1 = std::partition(tmp1.begin(), tmp1.end(), p);
            auto iter2 = tstd::partition(tmp2.begin(), tmp2.end(), p);
            util.assertEqual(std::distance(tmp1.begin(), iter1), std::distance(tmp2.begin(), iter2));
            util.assertSetEqual(tmp1, tmp2); // not guarantee sequence equality
            util.assertEqual(std::is_partitioned(tmp1.begin(), tmp1.end(), p), tstd::is_partitioned(tmp2.begin(), tmp2.end(), p));
        }
    }
    // partition_copy
    {
        std::vector<int> tmp1t, tmp1f;
        std::vector<int> tmp2t, tmp2f;
        auto pred = [](int a) -> bool { return a > 50; };
        auto p1 = std::partition_copy(vec.begin(), vec.end(), std::back_inserter(tmp1t), std::back_inserter(tmp1f), pred);
        auto p2 = std::partition_copy(vec.begin(), vec.end(), std::back_inserter(tmp2t), std::back_inserter(tmp2f), pred);
        util.assertEqual(tmp1t.size(), tmp2t.size());
        util.assertEqual(tmp1f.size(), tmp2f.size());
        util.assertSequenceEqual(tmp1t, tmp2t);
        util.assertSequenceEqual(tmp1f, tmp2f);
    }
    // stable_partition
    {
        std::vector<int> tmp1(vec);
        std::vector<int> tmp2(vec);
        auto p = [](int a) -> bool { return a > 50; };
        auto iter1 = std::stable_partition(tmp1.begin(), tmp1.end(), p);
        auto iter2 = tstd::stable_partition(tmp2.begin(), tmp2.end(), p);
        util.assertEqual(std::distance(tmp1.begin(), iter1), std::distance(tmp2.begin(), iter2));
        util.assertSequenceEqual(tmp1, tmp2); // should guarantee sequence equality
        util.assertEqual(std::is_partitioned(tmp1.begin(), tmp1.end(), p), tstd::is_partitioned(tmp2.begin(), tmp2.end(), p));
        {
            auto p = [](int a) -> bool { return a < 50; };
            auto iter1 = std::stable_partition(tmp1.begin(), tmp1.end(), p);
            auto iter2 = tstd::stable_partition(tmp2.begin(), tmp2.end(), p);
            util.assertEqual(std::distance(tmp1.begin(), iter1), std::distance(tmp2.begin(), iter2));
            util.assertSequenceEqual(tmp1, tmp2);  // not guarantee sequence equality
            util.assertEqual(std::is_partitioned(tmp1.begin(), tmp1.end(), p), tstd::is_partitioned(tmp2.begin(), tmp2.end(), p));
        }
    }
    // partition_point
    {
        std::vector<int> tmp(vec);
        auto p = [](int a) -> bool { return a > 50; };
        auto iter = std::partition(tmp.begin(), tmp.end(), p);
        auto iter1 = std::partition_point(tmp.begin(), tmp.end(), p);
        auto iter2 = tstd::partition_point(tmp.begin(), tmp.end(), p);
        util.assertEqual(iter == iter1, true);
        util.assertEqual(iter1 == iter2, true);
    }
    util.showFinalResult();
}

void testSortingAlgorithms(bool showDetails)
{
    TestUtil util(showDetails, "sorting algorithms");
    
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());
    // is_sorted
    {
        std::vector<int> tmp(vec);
        std::sort(tmp.begin(), tmp.end());
        // 1
        auto res1 = std::is_sorted(tmp.begin(), tmp.end());
        auto res2 = tstd::is_sorted(tmp.begin(), tmp.end());
        util.assertEqual(res1, res2);
        res1 = std::is_sorted(vec.begin(), vec.end());
        res2 = tstd::is_sorted(vec.begin(), vec.end());
        util.assertEqual(res1, res2);
        // 2
        std::sort(tmp.begin(), tmp.end(), std::greater<int>());
        res1 = std::is_sorted(tmp.begin(), tmp.end());
        res2 = tstd::is_sorted(tmp.begin(), tmp.end());
        util.assertEqual(res1, res2);
        res1 = std::is_sorted(tmp.begin(), tmp.end(), std::greater<int>());
        res2 = tstd::is_sorted(tmp.begin(), tmp.end(), std::greater<int>());
        util.assertEqual(res1, res2);
    }
    // is_sorted_until
    {
        std::vector<int> tmp(vec);
        std::sort(tmp.begin(), tmp.begin() + 50);
        // 1
        auto iter1 = std::is_sorted_until(tmp.begin(), tmp.end());
        auto iter2 = tstd::is_sorted_until(tmp.begin(), tmp.end());
        util.assertEqual(iter1 == iter2, true);
        // 2
        std::sort(tmp.begin(), tmp.begin() + 50, std::greater<int>());
        iter1 = std::is_sorted_until(tmp.begin(), tmp.end(), std::greater<int>());
        iter2 = tstd::is_sorted_until(tmp.begin(), tmp.end(), std::greater<int>());
        util.assertEqual(iter1 == iter2, true);
    }
    // sort
    {
        std::vector<int> tmp1(vec);
        std::vector<int> tmp2(vec);
        // 1
        std::sort(tmp1.begin(), tmp1.end());
        tstd::sort(tmp2.begin(), tmp2.end());
        util.assertSequenceEqual(tmp1, tmp2);
        // 2
        std::sort(tmp1.begin(), tmp1.end(), std::greater<int>());
        tstd::sort(tmp2.begin(), tmp2.end(), std::greater<int>());
        util.assertSequenceEqual(tmp1, tmp2);
    }
    // partial_sort
    {
        std::vector<int> tmp1(vec);
        std::vector<int> tmp2(vec);
        // 1
        std::partial_sort(tmp1.begin(), tmp1.begin() + 47, tmp1.end());
        tstd::partial_sort(tmp2.begin(), tmp2.begin() + 47, tmp2.end());
        util.assertRangeEqual(tmp1.begin(), tmp1.begin() + 47, tmp2.begin());
        util.assertSetEqual(tmp1, tmp2);
        // 2
        std::partial_sort(tmp1.begin(), tmp1.begin() + 47, tmp1.end(), std::greater<int>());
        tstd::partial_sort(tmp2.begin(), tmp2.begin() + 47, tmp2.end(), std::greater<int>());
        util.assertRangeEqual(tmp1.begin(), tmp1.begin() + 47, tmp2.begin());
        util.assertSetEqual(tmp1, tmp2);
    }
    // partial_sort_copy
    {
        std::vector<int> tmp1(vec);
        std::vector<int> tmp2(vec);
        {
            std::vector<int> tmp1d(50);
            std::vector<int> tmp2d(50);
            // 1
            auto iter1 = std::partial_sort_copy(tmp1.begin(), tmp1.end(), tmp1d.begin(), tmp1d.end());
            auto iter2 = tstd::partial_sort_copy(tmp2.begin(), tmp2.end(), tmp2d.begin(), tmp2d.end());
            util.assertEqual(iter1 - tmp1d.begin(), iter2 - tmp2d.begin());
            util.assertSequenceEqual(tmp1d, tmp2d);
            // 2
            iter1 = std::partial_sort_copy(tmp1.begin(), tmp1.end(), tmp1d.begin(), tmp1d.end(), std::greater<int>());
            iter2 = tstd::partial_sort_copy(tmp2.begin(), tmp2.end(), tmp2d.begin(), tmp2d.end(), std::greater<int>());
            util.assertEqual(iter1 - tmp1d.begin(), iter2 - tmp2d.begin());
            util.assertSequenceEqual(tmp1d, tmp2d);
        }
        {
            std::vector<int> tmp1d(150);
            std::vector<int> tmp2d(150);
            // 1
            auto iter1 = std::partial_sort_copy(tmp1.begin(), tmp1.end(), tmp1d.begin(), tmp1d.end());
            auto iter2 = tstd::partial_sort_copy(tmp2.begin(), tmp2.end(), tmp2d.begin(), tmp2d.end());
            util.assertEqual(iter1 - tmp1d.begin(), iter2 - tmp2d.begin());
            util.assertRangeEqual(tmp1d.begin(), iter1, tmp2d.begin(), iter2);
            // 2
            iter1 = std::partial_sort_copy(tmp1.begin(), tmp1.end(), tmp1d.begin(), tmp1d.end(), std::greater<int>());
            iter2 = tstd::partial_sort_copy(tmp2.begin(), tmp2.end(), tmp2d.begin(), tmp2d.end(), std::greater<int>());
            util.assertEqual(iter1 - tmp1d.begin(), iter2 - tmp2d.begin());
            util.assertRangeEqual(tmp1d.begin(), tmp1d.begin() + 100, tmp2d.begin());
        }
    }
    // stable_sort
    {

        // 1
        {
            std::vector<int> tmp1{1, 2, 2, 3, 1, 1, 1, 10, 2};
            std::vector<int> tmp2{1, 2, 2, 3, 1, 1, 1, 10, 2};
            std:stable_sort(tmp1.begin(), tmp1.end());
            tstd:stable_sort(tmp2.begin(), tmp2.end());
            util.assertSequenceEqual(tmp1, tmp2);
        }
        // 2
        {
            std::vector<std::pair<int, std::string>> tmp1{{1, "1_x"}, {2, "2_v"}, {2, "2_c"}, {1, "1_d"}, {1, "1_e"}, {4, "4_f"}, {5, "5_g"}, {6, "6_h"}, {8, "8_i"}, {1, "1_j"}, {1, "1_k"}, {2, "2_l"}};
            std::vector<std::pair<int, std::string>> tmp2(tmp1);
            auto cmp = [](const std::pair<int, std::string>& p1, const std::pair<int, std::string>& p2) -> bool { return p1.first < p2.first; };
            std::stable_sort(tmp1.begin(), tmp1.end(), cmp);
            tstd::stable_sort(tmp2.begin(), tmp2.end(), cmp);
            util.assertSequenceEqual(tmp1, tmp2);
        }
        {
            std::vector<std::pair<int, std::string>> tmp1{{1, "1_x"}, {2, "2_v"}, {2, "2_c"}, {1, "1_d"}, {1, "1_e"}, {4, "4_f"}, {5, "5_g"}, {6, "6_h"}, {8, "8_i"}, {1, "1_j"}, {1, "1_k"}, {2, "2_l"}};
            std::vector<std::pair<int, std::string>> tmp2(tmp1);
            auto cmp = [](const std::pair<int, std::string>& p1, const std::pair<int, std::string>& p2) -> bool { return p1.first > p2.first; };
            std::stable_sort(tmp1.begin(), tmp1.end(), cmp);
            tstd::stable_sort(tmp2.begin(), tmp2.end(), cmp);
            util.assertSequenceEqual(tmp1, tmp2);
        }
    }
    // nth_element
    {
        // 1
        {
            std::vector<int> tmp1(vec);
            std::vector<int> tmp2(vec);
            std::nth_element(tmp1.begin(), tmp1.begin() + 50, tmp1.end());
            tstd::nth_element(tmp2.begin(), tmp2.begin() + 50, tmp2.end());
            util.assertSetEqual(tmp1, tmp2);
            util.assertEqual(*(tmp1.begin() + 50), *(tmp2.begin() + 50));
            util.assertSetEqual(tmp1.begin(), tmp1.begin() + 49, tmp2.begin(), tmp2.begin() + 49);
            util.assertSetEqual(tmp1.begin() + 50, tmp1.end(), tmp2.begin() + 50, tmp2.end());
        }
        // 2
        {
            std::vector<int> tmp1(vec);
            std::vector<int> tmp2(vec);
            std::nth_element(tmp1.begin(), tmp1.begin() + 50, tmp1.end(), std::greater<int>());
            tstd::nth_element(tmp2.begin(), tmp2.begin() + 50, tmp2.end(), std::greater<int>());
            util.assertSetEqual(tmp1, tmp2);
            util.assertEqual(*(tmp1.begin() + 50), *(tmp2.begin() + 50));
            util.assertSetEqual(tmp1.begin(), tmp1.begin() + 49, tmp2.begin(), tmp2.begin() + 49);
            util.assertSetEqual(tmp1.begin() + 50, tmp1.end(), tmp2.begin() + 50, tmp2.end());
        }
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
    // is_heap_until
    {
        std::vector<int> v(vec);
        auto res1 = tstd::is_heap_until(v.begin(), v.end());
        auto res2 = std::is_heap_until(v.begin(), v.end());
        util.assertEqual(res1 == res2, true);
        res1 = tstd::is_heap_until(v.begin(), v.end(), std::greater<int>());
        res2 = std::is_heap_until(v.begin(), v.end(), std::greater<int>());
        util.assertEqual(res1 == res2, true);
    }
    // is_heap
    {
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
    // push_heap, pop_heap
    {
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
    // make_heap, sort_heap
    {
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

    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());
    // equal
    {
        std::vector<int> tmp(vec);
        // 1
        auto res1 = std::equal(vec.begin(), vec.end(), tmp.begin());
        auto res2 = tstd::equal(vec.begin(), vec.end(), tmp.begin());
        util.assertEqual(res1, res2);
        // 2
        res1 = std::equal(vec.begin(), vec.end(), tmp.begin(), std::equal_to<int>());
        res2 = tstd::equal(vec.begin(), vec.end(), tmp.begin(), std::equal_to<int>());
        util.assertEqual(res1, res2);
        // 3
        res1 = std::equal(vec.begin(), vec.end(), tmp.begin(), tmp.end());
        res2 = tstd::equal(vec.begin(), vec.end(), tmp.begin(), tmp.end());
        util.assertEqual(res1, res2);
        // 4
        res1 = std::equal(vec.begin(), vec.end(), tmp.begin(), tmp.end(), std::equal_to<int>());
        res2 = tstd::equal(vec.begin(), vec.end(), tmp.begin(), tmp.end(), std::equal_to<int>());
        util.assertEqual(res1, res2);
        
        tmp[10] = -1;
        // 1
        res1 = std::equal(vec.begin(), vec.end(), tmp.begin());
        res2 = tstd::equal(vec.begin(), vec.end(), tmp.begin());
        util.assertEqual(res1, res2);
        // 2
        res1 = std::equal(vec.begin(), vec.end(), tmp.begin(), std::equal_to<int>());
        res2 = tstd::equal(vec.begin(), vec.end(), tmp.begin(), std::equal_to<int>());
        util.assertEqual(res1, res2);
        // 3
        res1 = std::equal(vec.begin(), vec.end(), tmp.begin(), tmp.end());
        res2 = tstd::equal(vec.begin(), vec.end(), tmp.begin(), tmp.end());
        util.assertEqual(res1, res2);
        // 4
        res1 = std::equal(vec.begin(), vec.end(), tmp.begin(), tmp.end(), std::equal_to<int>());
        res2 = tstd::equal(vec.begin(), vec.end(), tmp.begin(), tmp.end(), std::equal_to<int>());
        util.assertEqual(res1, res2);
    }
    // lexicographical_compare
    {
        std::vector<int> tmp(vec);
        tmp[50] = -1;
        // 1
        auto res1 = std::lexicographical_compare(vec.begin(), vec.end(), tmp.begin(), tmp.end());
        auto res2 = tstd::lexicographical_compare(vec.begin(), vec.end(), tmp.begin(), tmp.end());
        util.assertEqual(res1, res2);
        // 2
        res1 = std::lexicographical_compare(vec.begin(), vec.end(), tmp.begin(), tmp.end(), std::greater<int>());
        res2 = tstd::lexicographical_compare(vec.begin(), vec.end(), tmp.begin(), tmp.end(), std::greater<int>());
        util.assertEqual(res1, res2);
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