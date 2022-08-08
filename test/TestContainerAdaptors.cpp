#include <iostream>
#include <deque>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <tstack.hpp>
#include <tdeque.hpp>
#include <tqueue.hpp>
#include <tlist.hpp>
#include <tvector.hpp>
#include "TestUtil.hpp"

template<typename Stack>
std::vector<typename Stack::value_type> stackToVector(Stack s)
{
    std::vector<typename Stack::value_type> vec;
    while (!s.empty())
    {
        vec.push_back(s.top());
        s.pop();
    }
    return vec;
}

template<typename Queue>
std::vector<typename Queue::value_type> queueToVector(Queue q)
{
    std::vector<typename Queue::value_type> vec;
    while (!q.empty())
    {
        vec.push_back(q.front());
        q.pop();
    }
    return vec;
}

template<typename Container1, typename Container2>
void testStackImpl(bool showDetails, const std::string& underlyingContainer)
{
    using tstd_stack = tstd::stack<int, Container1>;
    using std_stack = std::stack<int, Container2>;
    TestUtil util(showDetails, "stack(underlying containers: " + underlyingContainer + ")");

    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::random_shuffle(vec.begin(), vec.end());
    {
        // constructors
        // 1
        {
            tstd_stack s1;
            std_stack s2;
            util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
            util.assertEqual(s1.size(), s2.size());
            util.assertEqual(s1.empty(), s2.empty());
        }
        // 2, 3, 4, 5
        {
            Container1 c1{1, 2, 3, 4, 5, 100};
            Container2 c2{1, 2, 3, 4, 5, 100};
            {
                tstd_stack s1(c1);
                std_stack s2(c2);
                util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
            }
            {
                tstd_stack s1(std::move(c1));
                std_stack s2(std::move(c2));
                util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
                util.assertSequenceEqual(c1, c2);
                tstd_stack sc1(s1);
                std_stack sc2(s2);
                util.assertSequenceEqual(stackToVector(sc1), stackToVector(sc2));
                tstd_stack sm1(std::move(s1));
                std_stack sm2(std::move(s2));
                util.assertSequenceEqual(stackToVector(sm1), stackToVector(sm2));
            }
        }
        // since C++23
        // // 6
        // {
        //     tstd_stack s1(vec.begin(), vec.end());
        //     std_stack s2(vec.begin(), vec.end());
        //     util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
        // }
        // 7
        {
            tstd_stack s1((typename Container1::allocator_type()));
            std_stack s2((typename Container2::allocator_type()));
            util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
            util.assertEqual(s1.size(), s2.size());
            util.assertEqual(s1.empty(), s2.empty());
        }
        // 8, 9, 10, 11
        {
            Container1 c1{1, 2, 3, 4, 5, 100};
            Container2 c2{1, 2, 3, 4, 5, 100};
            {
                tstd_stack s1(c1, typename Container1::allocator_type());
                std_stack s2(c2, typename Container2::allocator_type());
                util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
            }
            {
                tstd_stack s1(std::move(c1), typename Container1::allocator_type());
                std_stack s2(std::move(c2), typename Container2::allocator_type());
                util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
                util.assertSequenceEqual(c1, c2);
                tstd_stack sc1(s1, typename Container1::allocator_type());
                std_stack sc2(s2, typename Container2::allocator_type());
                util.assertSequenceEqual(stackToVector(sc1), stackToVector(sc2));
                tstd_stack sm1(std::move(s1), typename Container1::allocator_type());
                std_stack sm2(std::move(s2), typename Container2::allocator_type());
                util.assertSequenceEqual(stackToVector(sm1), stackToVector(sm2));
            }
        }
        // since C++23
        // // 12
        // {
        //     tstd_stack s1(vec.begin(), vec.end(), typename Container1::allocator_type());
        //     std_stack s2(vec.begin(), vec.end(), typename Container2::allocator_type();
        //     util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
        // }
    }
    {
        // assignment
        tstd_stack s1(Container1{1, 2, 3, 4, 5, 100});
        std_stack s2(Container2{1, 2, 3, 4, 5, 100});
        tstd_stack tmp1(Container1{1, 2, 3, 4, 5, 99});
        std_stack tmp2(Container2{1, 2, 3, 4, 5, 99});
        s1 = tmp1;
        s2 = tmp2;
        util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
        s1 = std::move(tmp1);
        s2 = std::move(tmp2);
        util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
        util.assertSequenceEqual(stackToVector(tmp1), stackToVector(tmp2));
    }
    {
        // element access, size and capacity
        tstd_stack s1(Container1{1, 2, 3, 4, 5, 100});
        std_stack s2(Container2{1, 2, 3, 4, 5, 100});
        util.assertEqual(s1.top(), s2.top());
        s1.top() = s2.top() = 99;
        util.assertEqual(s1.top(), s2.top());
        util.assertEqual(s1.empty(), s2.empty());
        util.assertEqual(s1.size(), s2.size());
    }
    {
        // modifiers
        // push, emplace
        tstd_stack s1;
        std_stack s2;
        for (int i = 0; i < 100; ++i)
        {
            s1.push(i);
            s2.push(i);
            s1.emplace(i);
            s2.emplace(i);
        }
        util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
        util.assertEqual(s1.top(), s2.top());
        // pop
        for (int i = 0; i < 100; ++i)
        {
            s1.pop();
            s2.pop();
        }
        util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
        // swap
        tstd_stack tmp1(Container1{1, 2, 3, 4, 5, 100});
        std_stack tmp2(Container2{1, 2, 3, 4, 5, 100});
        tmp1.swap(s1);
        tmp2.swap(s2);
        util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
        util.assertSequenceEqual(stackToVector(tmp1), stackToVector(tmp2));
        swap(s1, tmp1);
        swap(s2, tmp2);
        util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
        util.assertSequenceEqual(stackToVector(tmp1), stackToVector(tmp2));
        tstd::swap(s1, tmp1);
        std::swap(s2, tmp2);
        util.assertSequenceEqual(stackToVector(s1), stackToVector(s2));
        util.assertSequenceEqual(stackToVector(tmp1), stackToVector(tmp2));
    }
    {
        // comparisons
        tstd_stack s1(Container1{1, 2, 3, 4, 5, 100});
        tstd_stack s2(Container1{1, 2, 3, 4, 5, 100});
        util.assertEqual(s1 == s2, true);
        util.assertEqual(s1 != s2, false);
        s1.top() = 10;
        s2.top() = 20;
        util.assertEqual(s1 < s2, true);
        util.assertEqual(s1 <= s2, true);
        util.assertEqual(s1 > s2, false);
        util.assertEqual(s1 >= s2, false);
    }
    util.showFinalResult();
}

void testStack(bool showDetails)
{
    testStackImpl<tstd::vector<int>, std::vector<int>>(showDetails, "tstd::vector<int> vs std::vector<int>");
    testStackImpl<tstd::deque<int>, std::deque<int>>(showDetails, "tstd::deque<int> vs std::deque<int>");
    testStackImpl<tstd::list<int>, std::list<int>>(showDetails, "tstd::list<int> vs std::list<int>");
}

template<typename Container1, typename Container2>
void testQueueImpl(bool showDetails, const std::string& underlyingContainer)
{
    using tstd_queue = std::queue<int, Container1>;
    using std_queue = std::queue<int, Container2>;
    TestUtil util(showDetails, "queue(underlying containers: " + underlyingContainer + ")");

    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::random_shuffle(vec.begin(), vec.end());
    {
        // constructors
        // 1
        {
            tstd_queue q1;
            std_queue q2;
            util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
            util.assertEqual(q1.size(), q2.size());
            util.assertEqual(q1.empty(), q2.empty());
        }
        // 2, 3, 4, 5
        {
            Container1 c1{1, 2, 3, 4, 5, 100};
            Container2 c2{1, 2, 3, 4, 5, 100};
            {
                tstd_queue q1(c1);
                std_queue q2(c2);
                util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
            }
            {
                tstd_queue q1(std::move(c1));
                std_queue q2(std::move(c2));
                util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
                util.assertSequenceEqual(c1, c2);
                tstd_queue sc1(q1);
                std_queue sc2(q2);
                util.assertSequenceEqual(queueToVector(sc1), queueToVector(sc2));
                tstd_queue sm1(std::move(q1));
                std_queue sm2(std::move(q2));
                util.assertSequenceEqual(queueToVector(sm1), queueToVector(sm2));
            }
        }
        // since C++23
        // // 6
        // {
        //     tstd_queue q1(vec.begin(), vec.end());
        //     std_queue q2(vec.begin(), vec.end());
        //     util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
        // }
        // 7
        {
            tstd_queue q1((typename Container1::allocator_type()));
            std_queue q2((typename Container2::allocator_type()));
            util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
            util.assertEqual(q1.size(), q2.size());
            util.assertEqual(q1.empty(), q2.empty());
        }
        // 8, 9, 10, 11
        {
            Container1 c1{1, 2, 3, 4, 5, 100};
            Container2 c2{1, 2, 3, 4, 5, 100};
            {
                tstd_queue q1(c1, typename Container1::allocator_type());
                std_queue q2(c2, typename Container2::allocator_type());
                util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
            }
            {
                tstd_queue q1(std::move(c1), typename Container1::allocator_type());
                std_queue q2(std::move(c2), typename Container2::allocator_type());
                util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
                util.assertSequenceEqual(c1, c2);
                tstd_queue sc1(q1, typename Container1::allocator_type());
                std_queue sc2(q2, typename Container2::allocator_type());
                util.assertSequenceEqual(queueToVector(sc1), queueToVector(sc2));
                tstd_queue sm1(std::move(q1), typename Container1::allocator_type());
                std_queue sm2(std::move(q2), typename Container2::allocator_type());
                util.assertSequenceEqual(queueToVector(sm1), queueToVector(sm2));
            }
        }
        // since C++23
        // // 12
        // {
        //     tstd_queue q1(vec.begin(), vec.end(), typename Container1::allocator_type());
        //     std_queue q2(vec.begin(), vec.end(), typename Container2::allocator_type();
        //     util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
        // }
    }
    {
        // assignment
        tstd_queue q1(Container1{1, 2, 3, 4, 5, 100});
        std_queue q2(Container2{1, 2, 3, 4, 5, 100});
        tstd_queue tmp1(Container1{1, 2, 3, 4, 5, 99});
        std_queue tmp2(Container2{1, 2, 3, 4, 5, 99});
        q1 = tmp1;
        q2 = tmp2;
        util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
        q1 = std::move(tmp1);
        q2 = std::move(tmp2);
        util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
        util.assertSequenceEqual(queueToVector(tmp1), queueToVector(tmp2));
    }
    {
        // element access, size and capacity
        tstd_queue q1(Container1{1, 2, 3, 4, 5, 100});
        std_queue q2(Container2{1, 2, 3, 4, 5, 100});
        util.assertEqual(q1.front(), q2.front());
        q1.front() = q2.front() = 99;
        util.assertEqual(q1.front(), q2.front());
        q1.back() = q2.back() = 999;
        util.assertEqual(q1.back(), q2.back());
        util.assertEqual(q1.empty(), q2.empty());
        util.assertEqual(q1.size(), q2.size());
    }
    {
        // modifiers
        // push, emplace
        tstd_queue q1;
        std_queue q2;
        for (int i = 0; i < 100; ++i)
        {
            q1.push(i);
            q2.push(i);
            q1.emplace(i);
            q2.emplace(i);
        }
        util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
        util.assertEqual(q1.front(), q2.front());
        // pop
        for (int i = 0; i < 100; ++i)
        {
            q1.pop();
            q2.pop();
        }
        util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
        // swap
        tstd_queue tmp1(Container1{1, 2, 3, 4, 5, 100});
        std_queue tmp2(Container2{1, 2, 3, 4, 5, 100});
        tmp1.swap(q1);
        tmp2.swap(q2);
        util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
        util.assertSequenceEqual(queueToVector(tmp1), queueToVector(tmp2));
        swap(q1, tmp1);
        swap(q2, tmp2);
        util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
        util.assertSequenceEqual(queueToVector(tmp1), queueToVector(tmp2));
        tstd::swap(q1, tmp1);
        std::swap(q2, tmp2);
        util.assertSequenceEqual(queueToVector(q1), queueToVector(q2));
        util.assertSequenceEqual(queueToVector(tmp1), queueToVector(tmp2));
    }
    {
        // comparisons
        tstd_queue q1(Container1{1, 2, 3, 4, 5, 100});
        tstd_queue q2(Container1{1, 2, 3, 4, 5, 100});
        util.assertEqual(q1 == q2, true);
        util.assertEqual(q1 != q2, false);
        q1.front() = 10;
        q2.front() = 20;
        util.assertEqual(q1 < q2, true);
        util.assertEqual(q1 <= q2, true);
        util.assertEqual(q1 > q2, false);
        util.assertEqual(q1 >= q2, false);
    }
    util.showFinalResult();
}

void testQueue(bool showDetails)
{
    testQueueImpl<tstd::deque<int>, std::deque<int>>(showDetails, "tstd::deque<int> vs std::deque<int>");
    testQueueImpl<tstd::list<int>, std::list<int>>(showDetails, "tstd::list<int> vs std::list<int>");
}

int main(int argc, char const *argv[])
{
    bool showDetail = parseDetailFlag(argc, argv);
    testStack(showDetail);
    testQueue(showDetail);
    std::cout << std::endl;
    return 0;
}
