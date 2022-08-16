#include <iostream>
#include <algorithm>
#include <talgorithm.hpp>
#include <vector>
#include <numeric>
#include "TestUtil.hpp"

void testHeapAlgorithms(bool showDetils);

int main(int argc, char const *argv[])
{
    bool showDetils = parseDetailFlag(argc, argv);
    testHeapAlgorithms(showDetils);
    std::cout << std::endl;
    return 0;
}


void testHeapAlgorithms(bool showDetials)
{
    TestUtil util(showDetials, "heap algorithms");

    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::random_shuffle(vec.begin(), vec.end());
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
