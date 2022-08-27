#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <forward_list>
#include <algorithm>
#include <random>
#include <titerator.hpp>
#include <tvector.hpp>
#include "TestUtil.hpp"

void testReverseIterator(bool showDetails);
void testMoveIterator(bool showDetails);
void testIteratorOperations(bool showDetails);
void testRangeAccess(bool showDetails);

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testReverseIterator(showDetails);
    testMoveIterator(showDetails);
    testIteratorOperations(showDetails);
    testRangeAccess(showDetails);
    std::cout << std::endl;
    return 0;
}

class Foo
{
public:
    Foo(int _a) : a(_a) {}
    int bar() { return a; }
    bool operator==(const Foo& foo) { return a == foo.a; }
private:
    int a;
};

void testReverseIterator(bool showDetails)
{
    TestUtil util(showDetails, "reverse iterator");
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());

    // reverse iterator
    {
        // constructors
        // 1
        {
            // default construction, no meaning, so do not test
            tstd::reverse_iterator<std::vector<int>::iterator> iter1;
            std::reverse_iterator<std::vector<int>::iterator> iter2;
        }
        {
            // 2
            tstd::reverse_iterator<std::vector<int>::iterator> iter1(vec.end());
            std::reverse_iterator<std::vector<int>::iterator> iter2(vec.end());
            util.assertEqual(*iter1, *iter2);
            // 3
            tstd::reverse_iterator<std::vector<int>::iterator> iter1c(iter1);
            std::reverse_iterator<std::vector<int>::iterator> iter2c(iter2);
            util.assertEqual(*iter1c, *iter2c);
        }
    }
    {
        // operator=
        tstd::reverse_iterator<std::vector<int>::iterator> iter1(vec.end());
        std::reverse_iterator<std::vector<int>::iterator> iter2(vec.end());
        tstd::reverse_iterator<std::vector<int>::iterator> iter1c;
        std::reverse_iterator<std::vector<int>::iterator> iter2c;
        iter1c = iter1;
        iter2c = iter2;
        util.assertEqual(*iter1c, *iter2c);
        // base
        util.assertEqual(iter1c.base() == iter2c.base(), true);
        util.assertEqual(*iter1c.base(), *iter2c.base());
    }
    {
        // operator*, operator->, operator[]
        std::vector<Foo> fvec{1, 2, 3, 4, 5};
        tstd::reverse_iterator<std::vector<Foo>::iterator> iter1(fvec.end());
        std::reverse_iterator<std::vector<Foo>::iterator> iter2(fvec.end());
        util.assertEqual(*iter1 == *iter2, true);
        util.assertEqual(iter1->bar(), iter2->bar());
        util.assertEqual(iter1[2] == iter2[2], true);
    }
    {
        // arithmetic operations
        // pre/post ++/--
        // +n/-n, +=n/-=n
        tstd::reverse_iterator<std::vector<int>::iterator> iter1(vec.end());
        std::reverse_iterator<std::vector<int>::iterator> iter2(vec.end());
        util.assertEqual(*(iter1++), *(iter2++));
        util.assertEqual(*iter1, *iter2);
        util.assertEqual(*(iter1--), *(iter2--));
        util.assertEqual(*iter1, *iter2);
        util.assertEqual(*(++iter1), *(++iter2));
        util.assertEqual(*iter1, *iter2);
        util.assertEqual(*(--iter1), *(--iter2));
        util.assertEqual(*iter1, *iter2);
        util.assertEqual(*(iter1+2), *(iter2+2));
        iter1 += 10;
        iter2 += 10;
        util.assertEqual(*(iter1 - 5), *(iter2 - 5));
        util.assertEqual(*iter1, *iter2);
        iter1 -= 2;
        iter2 -= 2;
        util.assertEqual(*iter1, *iter2);
    }
    {
        // make_reverse_iterator
        std::vector<int> vec1(vec);
        std::vector<int> vec2(vec);
        std::sort(tstd::make_reverse_iterator(vec1.end()), tstd::make_reverse_iterator(vec1.begin()));
        std::sort(std::make_reverse_iterator(vec2.end()), std::make_reverse_iterator(vec2.begin()));
        util.assertSequenceEqual(vec1, vec2);
    }
    {
        // comparisons
        tstd::reverse_iterator<std::vector<int>::iterator> iter1(vec.end());
        std::reverse_iterator<std::vector<int>::iterator> iter2(vec.end());
        auto iter3 = iter1 + 2;
        auto iter4 = iter2 + 2;
        util.assertEqual(iter1 == iter3, iter2 == iter4);
        util.assertEqual(iter1 != iter3, iter2 != iter4);
        util.assertEqual(iter1 < iter3, iter2 < iter4);
        util.assertEqual(iter1 <= iter3, iter2 <= iter4);
        util.assertEqual(iter1 > iter3, iter2 > iter4);
        util.assertEqual(iter1 >= iter3, iter2 >= iter4);
    }
    {
        // non-member arithemetic operations
        // n + iter, iter1 - iter2
        tstd::reverse_iterator<std::vector<int>::iterator> iter1(vec.end());
        std::reverse_iterator<std::vector<int>::iterator> iter2(vec.end());
        tstd::reverse_iterator<std::vector<int>::iterator> tmp1(vec.begin() + 30);
        std::reverse_iterator<std::vector<int>::iterator> tmp2(vec.begin() + 30);
        util.assertEqual(*(50 + iter1), *(50 + iter2));
        util.assertEqual(iter1 - tmp1, iter2 - tmp2);
        util.assertEqual(tmp1 - iter1, tmp2 - iter2);
    }
    util.showFinalResult();
}

void testMoveIterator(bool showDetails)
{
    TestUtil util(showDetails, "move iterator");
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::shuffle(vec.begin(), vec.end(), std::mt19937());

    // move iterator
    {
        // constructors
        // 1
        {
            // default construction, no meaning, so do not test
            tstd::move_iterator<std::vector<int>::iterator> iter1;
            std::move_iterator<std::vector<int>::iterator> iter2;
        }
        {
            // 2
            tstd::move_iterator<std::vector<int>::iterator> iter1(vec.begin());
            std::move_iterator<std::vector<int>::iterator> iter2(vec.begin());
            util.assertEqual(*iter1, *iter2);
            // 3
            tstd::move_iterator<std::vector<int>::iterator> iter1c(iter1);
            std::move_iterator<std::vector<int>::iterator> iter2c(iter2);
            util.assertEqual(*iter1c, *iter2c);
        }
    }
    {
        // operator=
        tstd::move_iterator<std::vector<int>::iterator> iter1(vec.begin());
        std::move_iterator<std::vector<int>::iterator> iter2(vec.begin());
        tstd::move_iterator<std::vector<int>::iterator> iter1c;
        std::move_iterator<std::vector<int>::iterator> iter2c;
        iter1c = iter1;
        iter2c = iter2;
        util.assertEqual(*iter1c, *iter2c);
        // base
        util.assertEqual(iter1c.base() == iter2c.base(), true);
        util.assertEqual(*iter1c.base(), *iter2c.base());
    }
    {
        // operator*, operator->, operator[]
        std::vector<Foo> fvec{1, 2, 3, 4, 5};
        tstd::move_iterator<std::vector<Foo>::iterator> iter1(fvec.begin());
        std::move_iterator<std::vector<Foo>::iterator> iter2(fvec.begin());
        util.assertEqual(*iter1 == *iter2, true);
        util.assertEqual(iter1->bar(), iter2->bar());
        util.assertEqual(iter1[2] == iter2[2], true);
    }
    {
        // arithmetic operations
        // pre/post ++/--
        // +n/-n, +=n/-=n
        tstd::move_iterator<std::vector<int>::iterator> iter1(vec.begin());
        std::move_iterator<std::vector<int>::iterator> iter2(vec.begin());
        util.assertEqual(*(iter1++), *(iter2++));
        util.assertEqual(*iter1, *iter2);
        util.assertEqual(*(iter1--), *(iter2--));
        util.assertEqual(*iter1, *iter2);
        util.assertEqual(*(++iter1), *(++iter2));
        util.assertEqual(*iter1, *iter2);
        util.assertEqual(*(--iter1), *(--iter2));
        util.assertEqual(*iter1, *iter2);
        util.assertEqual(*(iter1+2), *(iter2+2));
        iter1 += 10;
        iter2 += 10;
        util.assertEqual(*(iter1 - 5), *(iter2 - 5));
        util.assertEqual(*iter1, *iter2);
        iter1 -= 2;
        iter2 -= 2;
        util.assertEqual(*iter1, *iter2);
    }
    {
        // make_reverse_iterator
        std::vector<std::string> vec1{"hello", "world"};
        std::vector<std::string> vec2{"hello", "world"};
        std::vector<std::string> tmp1(2);
        std::vector<std::string> tmp2(2);
        std::copy(tstd::make_move_iterator(vec1.begin()), tstd::make_move_iterator(vec1.end()), tmp1.begin());
        std::copy(std::make_move_iterator(vec2.begin()), std::make_move_iterator(vec2.end()), tmp2.begin());
        util.assertSequenceEqual(vec1, vec2);
        util.assertSequenceEqual(tmp1, tmp2);
    }
    {
        // comparisons
        tstd::move_iterator<std::vector<int>::iterator> iter1(vec.begin());
        std::move_iterator<std::vector<int>::iterator> iter2(vec.begin());
        auto iter3 = iter1 + 2;
        auto iter4 = iter2 + 2;
        util.assertEqual(iter1 == iter3, iter2 == iter4);
        util.assertEqual(iter1 != iter3, iter2 != iter4);
        util.assertEqual(iter1 < iter3, iter2 < iter4);
        util.assertEqual(iter1 <= iter3, iter2 <= iter4);
        util.assertEqual(iter1 > iter3, iter2 > iter4);
        util.assertEqual(iter1 >= iter3, iter2 >= iter4);
    }
    {
        // non-member arithemetic operations
        // n + iter, iter1 - iter2
        tstd::move_iterator<std::vector<int>::iterator> iter1(vec.begin());
        std::move_iterator<std::vector<int>::iterator> iter2(vec.begin());
        tstd::move_iterator<std::vector<int>::iterator> tmp1(vec.begin() + 30);
        std::move_iterator<std::vector<int>::iterator> tmp2(vec.begin() + 30);
        util.assertEqual(*(50 + iter1), *(50 + iter2));
        util.assertEqual(iter1 - tmp1, iter2 - tmp2);
        util.assertEqual(tmp1 - iter1, tmp2 - iter2);
    }
    util.showFinalResult();
}

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

