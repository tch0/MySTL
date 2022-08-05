#include <iostream>
#include <titerator.hpp>
#include <tvector.hpp>
#include <vector>
#include <list>
#include <forward_list>
#include "TestUtil.hpp"

void testIteratorOperations(bool showDetails)
{
    TestUtil util(showDetails, "iterator operations");
    {
        // advance
        {
            // forward iterator
            std::forward_list<int> l{1, 2, 3, 4};
            auto it1 = l.begin();
            auto it2 = l.begin();
            tstd::advance(it1, 3);
            ++it2;
            ++it2;
            ++it2;
            util.assertEqual(it1 == it2, true);
        }
        {
            // random access iterator
            std::vector<int> vec{1, 2, 3, 4};
            auto it1 = vec.begin();
            auto it2 = vec.begin();
            tstd::advance(it1, 3);
            it2 += 3;
            util.assertEqual(it1 == it2, true);
        }
        // distance
        {
            // forward iterator
            std::forward_list<int> l{1, 2, 3, 4};
            auto it = l.begin();
            tstd::advance(it, 3);
            util.assertEqual(tstd::distance(l.begin(), it), 3);
        }
        {
            // random access iterator
            std::vector<int> vec{1, 2, 3, 4};
            auto it = vec.begin();
            tstd::advance(it, 3);
            util.assertEqual(tstd::distance(vec.begin(), it), 3);
        }
        // next
        {
            // forward iterator
            std::forward_list<int> l{1, 2, 3, 4};
            auto it1 = l.begin();
            auto it2 = l.begin();
            util.assertEqual(tstd::next(it1) == ++it2, true); // next do not change iterator
            util.assertEqual(tstd::next(l.begin()) == it2, true);
            util.assertEqual(it1 == l.begin(), true);
        }
        {
            // random access iterator
            std::vector<int> vec{1, 2, 3, 4};
            auto it1 = vec.begin();
            auto it2 = vec.begin();
            util.assertEqual(tstd::next(it1) == ++it2, true); // next do not change iterator
            util.assertEqual(tstd::next(vec.begin()) == it2, true);
            util.assertEqual(it1 == vec.begin(), true);
        }
        // prev
        {
            // bidirectional iterator
            std::list<int> l{1, 2, 3, 4};
            auto it = l.begin();
            tstd::advance(it, 3);
            util.assertEqual(tstd::prev(l.end()) == it, true);
        }
        {
            // random access iterator
            std::vector<int> vec{1, 2, 3, 4};
            auto it = vec.begin();
            tstd::advance(it, 3);
            util.assertEqual(tstd::prev(vec.end()) == it, true);
            util.assertEqual(tstd::prev(vec.end(), 2) == vec.begin() + 2, true);
        }
    }
    util.showFinalResult();
}

void testRangeAccess(bool showDetails)
{
    TestUtil util(showDetails, "range access in iterator");
    {
        // tstd::vector as example
        tstd::vector<int> vec{1, 2, 3};
        util.assertEqual(tstd::begin(vec) == vec.begin(), true);
        util.assertEqual(tstd::end(vec) == vec.end(), true);
        util.assertEqual(tstd::rbegin(vec) == vec.rbegin(), true);
        util.assertEqual(tstd::rend(vec) == vec.rend(), true);
        util.assertEqual(tstd::crbegin(vec) == vec.crbegin(), true);
        util.assertEqual(tstd::crend(vec) == vec.crend(), true);
        util.assertEqual(tstd::data(vec) == vec.data(), true);
        util.assertEqual(tstd::empty(vec) == vec.empty(), true);
    }
    {
        int A[10]{};
        util.assertEqual(tstd::begin(A) == A, true);
        util.assertEqual(tstd::end(A) == A + 10, true);
        util.assertEqual(tstd::rbegin(A) == tstd::reverse_iterator<int*>(A+10), true);
        util.assertEqual(tstd::rend(A) == tstd::reverse_iterator<int*>(A), true);
        util.assertEqual(tstd::crbegin(A) == tstd::reverse_iterator<const int*>(A+10), true);
        util.assertEqual(tstd::crend(A) == tstd::reverse_iterator<const int*>(A), true);
        util.assertEqual(tstd::data(A) == A, true);
        util.assertEqual(tstd::empty(A) == false, true);
    }
    util.showFinalResult();
}

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testIteratorOperations(showDetails);
    testRangeAccess(showDetails);
    std::cout << std::endl;
    return 0;
}
