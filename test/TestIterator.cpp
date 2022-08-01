#include <iostream>
#include <titerator.hpp>
#include <tvector.hpp>
#include "TestUtil.hpp"

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
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testRangeAccess(showDetails);
    return 0;
}
