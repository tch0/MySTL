#include <iostream>
#include <set>
#include <random>
#include <vector>
#include <string>
#include <functional>
#include <iterator>
#include <tset.hpp>
// #include <tmap.hpp>
// #include <tmultiset.hpp>
// #include <tmultimap.hpp>
#include "TestUtil.hpp"

void testSet(bool showDetails);
void testMap(bool showDetails);
void testMultiset(bool showDetails);
void testMultimap(bool showDetails);

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testSet(showDetails);
    std::cout << std::endl;
    return 0;
}

class Foo
{
    friend std::ostream& operator<<(std::ostream& os, const Foo& foo)
    {
        os << "foo(" << foo.a << ")";
        return os;
    }
public:
    Foo(int _a = 0) : a(_a) {}
    Foo(const Foo& foo) = default;
    Foo& operator=(const Foo& foo) = default;
    bool operator==(const Foo& foo) const = default;
    bool operator<(const Foo& foo) const
    {
        return a < foo.a;
    }
private:
    int a;
};


void testSet(bool showDetails)
{
    TestUtil util(showDetails, "set");
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::random_shuffle(vec.begin(), vec.end());

    // constructors
    {
        // 1
        tstd::set<int> s1;
        std::set<int> s2;
        util.assertSequenceEqual(s1, s2);
    }
    {
        // 2
        tstd::set<int> s1((std::less<int>(), tstd::allocator<int>()));
        std::set<int> s2((std::less<int>(), std::allocator<int>()));
        util.assertSequenceEqual(s1, s2);
    }
    {
        // 3
        tstd::set<int> s1((tstd::allocator<int>()));
        std::set<int> s2((std::allocator<int>()));
        util.assertSequenceEqual(s1, s2);
    }
    {
        // 4
        tstd::set<int> s1(vec.begin(), vec.end());
        std::set<int> s2(vec.begin(), vec.end());
        util.assertSequenceEqual(s1, s2);
    }
    {
        // 5
        tstd::set<int> s1(vec.begin(), vec.end(), tstd::allocator<int>());
        std::set<int> s2(vec.begin(), vec.end(), std::allocator<int>());
        util.assertSequenceEqual(s1, s2);
        {
            // 6
            tstd::set<int> s1c(s1);
            std::set<int> s2c(s2);
            util.assertSequenceEqual(s1c, s2c);
        }
        {
            // 7
            tstd::set<int> s1c(s1, tstd::allocator<int>());
            std::set<int> s2c(s2, std::allocator<int>());
            util.assertSequenceEqual(s1c, s2c);
        }
        // 8
        tstd::set<int> s1m(std::move(s1));
        std::set<int> s2m(std::move(s2));
        util.assertSequenceEqual(s1, s2);
        util.assertSequenceEqual(s1m, s2m);
        // 9
        tstd::set<int> s1mm(std::move(s1m), tstd::allocator<int>());
        std::set<int> s2mm(std::move(s2m), std::allocator<int>());
        util.assertSequenceEqual(s1m, s2m);
        util.assertSequenceEqual(s1mm, s2mm);
    }
    {
        // 10
        tstd::set<int> s1{2, 1, 3, 4, 4, 10};
        std::set<int> s2{2, 1, 3, 4, 4, 10};
        util.assertSequenceEqual(s1, s2);
    }
    {
        // 11
        tstd::set<int> s1({2, 1, 3, 4, 4, 10}, tstd::allocator<int>());
        std::set<int> s2({2, 1, 3, 4, 4, 10}, std::allocator<int>());
        util.assertSequenceEqual(s1, s2);
    }
    // assignment
    {
        tstd::set<int> s1(vec.begin(), vec.end());
        std::set<int> s2(vec.begin(), vec.end());
        tstd::set<int> s1c;
        std::set<int> s2c;
        // 1
        s1c = s1;
        s2c = s2;
        util.assertSequenceEqual(s1c, s2c);
        // 2
        s1c = std::move(s1);
        s2c = std::move(s2);
        util.assertSequenceEqual(s1c, s2c);
        util.assertSequenceEqual(s1, s2);
        // 3
        s1 = {2, 1, 3, 4, 4, 10};
        s2 = {2, 1, 3, 4, 4, 10};
        util.assertSequenceEqual(s1, s2);
    }
    // allocator
    {
        tstd::set<int> s1;
        util.assertEqual(s1.get_allocator() == tstd::allocator<int>(), true);
    }
    // iterators
    {
        tstd::set<int> s1(vec.begin(), vec.end());
        std::set<int> s2(vec.begin(), vec.end());
        util.assertEqual(*s1.begin(), *s2.begin());
        util.assertRangeEqual(s1.begin(), s1.end(), s2.cbegin());
        util.assertRangeEqual(s1.cbegin(), s1.cend(), s2.begin());
        util.assertRangeEqual(s1.rbegin(), s1.rend(), s2.crbegin());
        util.assertRangeEqual(s1.crbegin(), s1.crend(), s2.rbegin());
        // const version
        {
            const tstd::set<int> s1(vec.begin(), vec.end());
            const std::set<int> s2(vec.begin(), vec.end());
            util.assertEqual(*s1.begin(), *s2.begin());
            util.assertRangeEqual(s1.begin(), s1.end(), s2.cbegin());
            util.assertRangeEqual(s1.cbegin(), s1.cend(), s2.begin());
            util.assertRangeEqual(s1.rbegin(), s1.rend(), s2.crbegin());
            util.assertRangeEqual(s1.crbegin(), s1.crend(), s2.rbegin());
        }
    }
    // size and capacity
    {
        tstd::set<int> s1(vec.begin(), vec.end());
        std::set<int> s2(vec.begin(), vec.end());
        util.assertEqual(s1.size(), s2.size());
        util.assertEqual(s1.empty(), s2.empty());
    }
    // modifiers
    {
        // clear
        tstd::set<int> s1(vec.begin(), vec.end());
        std::set<int> s2(vec.begin(), vec.end());
        s1.clear();
        s2.clear();
        util.assertSequenceEqual(s1, s2);
        // insert
        // 1
        int a = 88;
        auto p1 = s1.insert(a);
        auto p2 = s2.insert(a);
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        // 2
        p1 = s1.insert(100);
        p2 = s2.insert(100);
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        // 3
        auto iter1 = s1.insert(s1.begin(), 70);
        auto iter2 = s2.insert(s2.begin(), 70);
        util.assertEqual(*iter1, *iter2);
        // 4
        iter1 = s1.insert(s1.begin(), a);
        iter2 = s2.insert(s2.begin(), a);
        util.assertEqual(*iter1, *iter2);
        // 5
        s1.insert(vec.begin(), vec.end());
        s2.insert(vec.begin(), vec.end());
        util.assertSequenceEqual(s1, s2);
        // 6
        s1.insert({1, 2, 3, 3, 100, 1024, 100, 99, 30143});
        s2.insert({1, 2, 3, 3, 100, 1024, 100, 99, 30143});
        util.assertSequenceEqual(s1, s2);
        // emplace
        s1.emplace(-100);
        s2.emplace(-100);
        util.assertSequenceEqual(s1, s2);
        s1.emplace_hint(s1.begin(), -1001);
        s2.emplace_hint(s2.begin(), -1001);
        util.assertSequenceEqual(s1, s2);
        // erase
        // 1
        iter1 = s1.erase(s1.find(15));
        iter2 = s2.erase(s2.find(15));
        util.assertEqual(*iter1, *iter2);
        util.assertSequenceEqual(s1, s2);
        // 2
        iter1 = s1.erase(s1.find(10), s1.find(30));
        iter2 = s2.erase(s2.find(10), s2.find(30));
        util.assertEqual(*iter1, *iter2);
        util.assertSequenceEqual(s1, s2);
        // 3
        auto count1 = s1.erase(50);
        auto count2 = s2.erase(50);
        util.assertEqual(count1, count2);
        util.assertSequenceEqual(s1, s2);
        // swap
        tstd::set<int> tmp1(vec.begin(), vec.end());
        std::set<int> tmp2(vec.begin(), vec.end());
        s1.swap(tmp1);
        s2.swap(tmp2);
        util.assertSequenceEqual(s1, s2);
        util.assertSequenceEqual(tmp1, tmp2);
        swap(s1, tmp1);
        swap(s2, tmp2);
        util.assertSequenceEqual(s1, s2);
        util.assertSequenceEqual(tmp1, tmp2);
        tstd::swap(s1, tmp1);
        std::swap(s2, tmp2);
        util.assertSequenceEqual(s1, s2);
        util.assertSequenceEqual(tmp1, tmp2);
    }
    {
        // lookup
        tstd::set<int> s1(vec.begin(), vec.end());
        std::set<int> s2(vec.begin(), vec.end());
        // find
        util.assertEqual(*s1.find(10), *s2.find(10));
        util.assertEqual(s1.find(1000) == s1.end(), s2.find(1000) == s2.end());
        // contains
        util.assertEqual(s1.contains(10), s2.contains(10));
        util.assertEqual(s1.contains(999), s2.contains(999));
        // equal_range
        auto p1 = s1.equal_range(10);
        auto p2 = s2.equal_range(10);
        util.assertEqual(tstd::distance(p1.first, p1.second), tstd::distance(p2.first, p2.second));
        util.assertEqual(*p1.first, *p2.first);
        util.assertEqual(*p1.second, *p2.second);
        // lower_bound
        s1.erase(50);
        s2.erase(50);
        auto l1 = s1.lower_bound(50);
        auto l2 = s2.lower_bound(50);
        util.assertEqual(*l1, *l2);
        // upper_bound
        auto u1 = s1.upper_bound(50);
        auto u2 = s2.upper_bound(50);
        util.assertEqual(*u1, *u2);
        // const version
        {
            const tstd::set<int> s1(vec.begin(), vec.end());
            const std::set<int> s2(vec.begin(), vec.end());
            // find
            util.assertEqual(*s1.find(10), *s2.find(10));
            util.assertEqual(s1.find(1000) == s1.end(), s2.find(1000) == s2.end());
            // contains
            util.assertEqual(s1.contains(10), s2.contains(10));
            util.assertEqual(s1.contains(999), s2.contains(999));
            // equal_range
            auto p1 = s1.equal_range(10);
            auto p2 = s2.equal_range(10);
            util.assertEqual(tstd::distance(p1.first, p1.second), tstd::distance(p2.first, p2.second));
            util.assertEqual(*p1.first, *p2.first);
            util.assertEqual(*p1.second, *p2.second);
            // lower_bound
            auto l1 = s1.lower_bound(50);
            auto l2 = s2.lower_bound(50);
            util.assertEqual(*l1, *l2);
            // upper_bound
            auto u1 = s1.upper_bound(50);
            auto u2 = s2.upper_bound(50);
            util.assertEqual(*u1, *u2);
        }
    }
    // template version of erase/count/find/equal_range/lower_bound/upper_bound
    {
        std::vector<Foo> vecfoo(100);
        for (int i = 0; i < vecfoo.size(); ++i)
        {
            vecfoo[i] = Foo(i);
        }
        tstd::set<Foo> s1(vecfoo.begin(), vecfoo.end());
        std::set<Foo> s2(vecfoo.begin(), vecfoo.end());
        util.assertSequenceEqual(s1, s2);
        // erase
        auto size1 = s1.erase(10);
        // auto size2 = s2.erase(10); // this version is since C++ 23, so use another version for tstd::set
        auto size2 = s2.erase(Foo(10));
        util.assertEqual(size1, size2);
        util.assertSequenceEqual(s1, s2);
        // count
        auto count1 = s1.count(10);
        auto count2 = s2.count(10);
        util.assertEqual(count1, count2);
        count1 = s1.count(20);
        count2 = s2.count(20);
        util.assertEqual(count1, count2);
        // find
        auto iter1 = s1.find(10);
        auto iter2 = s2.find(10);
        util.assertEqual(iter1 == s1.end(), iter2 == s2.end());
        iter1 = s1.find(20);
        iter2 = s2.find(20);
        util.assertEqual(*iter1, *iter2);
        // equal_range
        auto p1 = s1.equal_range(50);
        auto p2 = s2.equal_range(50);
        util.assertEqual(tstd::distance(p1.first, p1.second), tstd::distance(p2.first, p2.second));
        util.assertEqual(*p1.first, *p2.first);
        util.assertEqual(*p1.second, *p2.second);
        // lower_bound
        auto l1 = s1.lower_bound(50);
        auto l2 = s2.lower_bound(50);
        util.assertEqual(*l1, *l2);
        // upper_bound
        auto u1 = s1.upper_bound(50);
        auto u2 = s2.upper_bound(50);
        util.assertEqual(*u1, *u2);
        // const version
        {
            const tstd::set<Foo> s1(vecfoo.begin(), vecfoo.end());
            const std::set<Foo> s2(vecfoo.begin(), vecfoo.end());
            util.assertSequenceEqual(s1, s2);
            // count
            auto count1 = s1.count(10);
            auto count2 = s2.count(10);
            util.assertEqual(count1, count2);
            count1 = s1.count(20);
            count2 = s2.count(20);
            util.assertEqual(count1, count2);
            // find
            auto iter1 = s1.find(10);
            auto iter2 = s2.find(10);
            util.assertEqual(iter1 == s1.end(), iter2 == s2.end());
            iter1 = s1.find(20);
            iter2 = s2.find(20);
            util.assertEqual(*iter1, *iter2);
            // equal_range
            auto p1 = s1.equal_range(50);
            auto p2 = s2.equal_range(50);
            util.assertEqual(tstd::distance(p1.first, p1.second), tstd::distance(p2.first, p2.second));
            util.assertEqual(*p1.first, *p2.first);
            util.assertEqual(*p1.second, *p2.second);
            // lower_bound
            auto l1 = s1.lower_bound(50);
            auto l2 = s2.lower_bound(50);
            util.assertEqual(*l1, *l2);
            // upper_bound
            auto u1 = s1.upper_bound(50);
            auto u2 = s2.upper_bound(50);
            util.assertEqual(*u1, *u2);
        }
    }
    // observers
    {
        tstd::set<int> s1(vec.begin(), vec.end());
        std::set<int> s2(vec.begin(), vec.end());
        util.assertEqual(typeid(s1.key_comp()) == typeid(std::less<int>), true);
        util.assertEqual(typeid(s2.key_comp()) == typeid(std::less<int>), true);
        util.assertEqual(typeid(s1.value_comp()) == typeid(std::less<int>), true);
        util.assertEqual(typeid(s2.value_comp()) == typeid(std::less<int>), true);
    }
    // comparisons
    {
        // comparisons
        tstd::set<int> s1(vec.begin(), vec.end());
        tstd::set<int> s2(vec.begin(), vec.end());
        util.assertEqual(s1 == s2, true);
        util.assertEqual(s1 != s2, false);
        s1.erase(10);
        util.assertEqual(s1 != s2, true);
        util.assertEqual(s1 > s2, true);
        util.assertEqual(s1 >= s2, true);
        util.assertEqual(s1 < s2, false);
        util.assertEqual(s1 <= s2, false);
    }
    util.showFinalResult();
}

void testMap(bool showDetails)
{
    TestUtil util(showDetails, "map");
    {

    }
    util.showFinalResult();
}
void testMultiset(bool showDetails)
{
    TestUtil util(showDetails, "multiset");
    {
        
    }
    util.showFinalResult();
}

void testMultimap(bool showDetails)
{
    TestUtil util(showDetails, "multimap");
    {
        
    }
    util.showFinalResult();
}