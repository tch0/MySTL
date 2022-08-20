#include <iostream>
#include <set>
#include <map>
#include <random>
#include <vector>
#include <string>
#include <functional>
#include <iterator>
#include <tset.hpp>
#include <tmap.hpp>
#include "TestUtil.hpp"

void testSet(bool showDetails);
void testMultiset(bool showDetails);
void testMap(bool showDetails);
void testMultimap(bool showDetails);

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testSet(showDetails);
    testMultiset(showDetails);
    testMap(showDetails);
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
        auto iter1 = s1.insert(s1.begin(), a);
        auto iter2 = s2.insert(s2.begin(), a);
        util.assertEqual(*iter1, *iter2);
        // 4
        iter1 = s1.insert(s1.begin(), 70);
        iter2 = s2.insert(s2.begin(), 70);
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
        // emplace_hint
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
        // count
        auto sz1 = s1.count(10);
        auto sz2 = s2.count(10);
        util.assertEqual(sz1, sz2);
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
    // template version of erase/count/find/contains/equal_range/lower_bound/upper_bound
    {
        std::vector<Foo> vecfoo;
        for (int i = 0; i < 100; ++i)
        {
            vecfoo.emplace_back(i);
        }
        tstd::set<Foo> s1(vecfoo.begin(), vecfoo.end());
        std::set<Foo> s2(vecfoo.begin(), vecfoo.end());
        util.assertSequenceEqual(s1, s2);
        // erase
        auto size1 = s1.erase(10);
        // auto size2 = s2.erase(10); // this version is since C++ 23, so use another version for std::set
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
        // contains
        util.assertEqual(s1.contains(50), s2.contains(50));
        util.assertEqual(s1.contains(500), s2.contains(500));
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

void testMultiset(bool showDetails)
{
    TestUtil util(showDetails, "multiset");
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    vec.resize(300);
    std::copy(vec.begin(), vec.begin() + 100, vec.begin() + 100);
    std::copy(vec.begin(), vec.begin() + 100, vec.begin() + 200);
    std::random_shuffle(vec.begin(), vec.end());

    // constructors
    {
        // 1
        tstd::multiset<int> s1;
        std::multiset<int> s2;
        util.assertSequenceEqual(s1, s2);
    }
    {
        // 2
        tstd::multiset<int> s1((std::less<int>(), tstd::allocator<int>()));
        std::multiset<int> s2((std::less<int>(), std::allocator<int>()));
        util.assertSequenceEqual(s1, s2);
    }
    {
        // 3
        tstd::multiset<int> s1((tstd::allocator<int>()));
        std::multiset<int> s2((std::allocator<int>()));
        util.assertSequenceEqual(s1, s2);
    }
    {
        // 4
        tstd::multiset<int> s1(vec.begin(), vec.end());
        std::multiset<int> s2(vec.begin(), vec.end());
        util.assertSequenceEqual(s1, s2);
    }
    {
        // 5
        tstd::multiset<int> s1(vec.begin(), vec.end(), tstd::allocator<int>());
        std::multiset<int> s2(vec.begin(), vec.end(), std::allocator<int>());
        util.assertSequenceEqual(s1, s2);
        {
            // 6
            tstd::multiset<int> s1c(s1);
            std::multiset<int> s2c(s2);
            util.assertSequenceEqual(s1c, s2c);
        }
        {
            // 7
            tstd::multiset<int> s1c(s1, tstd::allocator<int>());
            std::multiset<int> s2c(s2, std::allocator<int>());
            util.assertSequenceEqual(s1c, s2c);
        }
        // 8
        tstd::multiset<int> s1m(std::move(s1));
        std::multiset<int> s2m(std::move(s2));
        util.assertSequenceEqual(s1, s2);
        util.assertSequenceEqual(s1m, s2m);
        // 9
        tstd::multiset<int> s1mm(std::move(s1m), tstd::allocator<int>());
        std::multiset<int> s2mm(std::move(s2m), std::allocator<int>());
        util.assertSequenceEqual(s1m, s2m);
        util.assertSequenceEqual(s1mm, s2mm);
    }
    {
        // 10
        tstd::multiset<int> s1{2, 1, 3, 4, 4, 10};
        std::multiset<int> s2{2, 1, 3, 4, 4, 10};
        util.assertSequenceEqual(s1, s2);
    }
    {
        // 11
        tstd::multiset<int> s1({2, 1, 3, 4, 4, 10}, tstd::allocator<int>());
        std::multiset<int> s2({2, 1, 3, 4, 4, 10}, std::allocator<int>());
        util.assertSequenceEqual(s1, s2);
    }
    // assignment
    {
        tstd::multiset<int> s1(vec.begin(), vec.end());
        std::multiset<int> s2(vec.begin(), vec.end());
        tstd::multiset<int> s1c;
        std::multiset<int> s2c;
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
        tstd::multiset<int> s1;
        util.assertEqual(s1.get_allocator() == tstd::allocator<int>(), true);
    }
    // iterators
    {
        tstd::multiset<int> s1(vec.begin(), vec.end());
        std::multiset<int> s2(vec.begin(), vec.end());
        util.assertEqual(*s1.begin(), *s2.begin());
        util.assertRangeEqual(s1.begin(), s1.end(), s2.cbegin());
        util.assertRangeEqual(s1.cbegin(), s1.cend(), s2.begin());
        util.assertRangeEqual(s1.rbegin(), s1.rend(), s2.crbegin());
        util.assertRangeEqual(s1.crbegin(), s1.crend(), s2.rbegin());
        // const version
        {
            const tstd::multiset<int> s1(vec.begin(), vec.end());
            const std::multiset<int> s2(vec.begin(), vec.end());
            util.assertEqual(*s1.begin(), *s2.begin());
            util.assertRangeEqual(s1.begin(), s1.end(), s2.cbegin());
            util.assertRangeEqual(s1.cbegin(), s1.cend(), s2.begin());
            util.assertRangeEqual(s1.rbegin(), s1.rend(), s2.crbegin());
            util.assertRangeEqual(s1.crbegin(), s1.crend(), s2.rbegin());
        }
    }
    // size and capacity
    {
        tstd::multiset<int> s1(vec.begin(), vec.end());
        std::multiset<int> s2(vec.begin(), vec.end());
        util.assertEqual(s1.size(), s2.size());
        util.assertEqual(s1.empty(), s2.empty());
    }
    // modifiers
    {
        // clear
        tstd::multiset<int> s1(vec.begin(), vec.end());
        std::multiset<int> s2(vec.begin(), vec.end());
        s1.clear();
        s2.clear();
        util.assertSequenceEqual(s1, s2);
        // insert
        // 1
        int a = 88;
        auto p1 = s1.insert(a);
        auto p2 = s2.insert(a);
        util.assertEqual(*p1, *p2);
        // 2
        p1 = s1.insert(100);
        p2 = s2.insert(100);
        util.assertEqual(*p1, *p2);
        // 3
        auto iter1 = s1.insert(s1.begin(), a);
        auto iter2 = s2.insert(s2.begin(), a);
        util.assertEqual(*iter1, *iter2);
        // 4
        iter1 = s1.insert(s1.begin(), 70);
        iter2 = s2.insert(s2.begin(), 70);
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
        // emplace_hint
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
        tstd::multiset<int> tmp1(vec.begin(), vec.end());
        std::multiset<int> tmp2(vec.begin(), vec.end());
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
        tstd::multiset<int> s1(vec.begin(), vec.end());
        std::multiset<int> s2(vec.begin(), vec.end());
        // count
        auto sz1 = s1.count(10);
        auto sz2 = s2.count(10);
        util.assertEqual(sz1, sz2);
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
            const tstd::multiset<int> s1(vec.begin(), vec.end());
            const std::multiset<int> s2(vec.begin(), vec.end());
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
    // template version of erase/count/find/contains/equal_range/lower_bound/upper_bound
    {
        std::vector<Foo> vecfoo;
        for (int i = 0; i < 100; ++i)
        {
            vecfoo.emplace_back(i);
        }
        tstd::multiset<Foo> s1(vecfoo.begin(), vecfoo.end());
        std::multiset<Foo> s2(vecfoo.begin(), vecfoo.end());
        util.assertSequenceEqual(s1, s2);
        // erase
        auto size1 = s1.erase(10);
        // auto size2 = s2.erase(10); // this version is since C++ 23, so use another version for std::multiset
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
        // contains
        util.assertEqual(s1.contains(50), s2.contains(50));
        util.assertEqual(s1.contains(500), s2.contains(500));
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
            const tstd::multiset<Foo> s1(vecfoo.begin(), vecfoo.end());
            const std::multiset<Foo> s2(vecfoo.begin(), vecfoo.end());
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
        tstd::multiset<int> s1(vec.begin(), vec.end());
        std::multiset<int> s2(vec.begin(), vec.end());
        util.assertEqual(typeid(s1.key_comp()) == typeid(std::less<int>), true);
        util.assertEqual(typeid(s2.key_comp()) == typeid(std::less<int>), true);
        util.assertEqual(typeid(s1.value_comp()) == typeid(std::less<int>), true);
        util.assertEqual(typeid(s2.value_comp()) == typeid(std::less<int>), true);
    }
    // comparisons
    {
        // comparisons
        tstd::multiset<int> s1(vec.begin(), vec.end());
        tstd::multiset<int> s2(vec.begin(), vec.end());
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
    std::vector<std::pair<const int, std::string>> vec;
    {
        std::vector<int> tmp(100);
        std::iota(tmp.begin(), tmp.end(), 1);
        std::random_shuffle(tmp.begin(), tmp.end());
        for (auto& elem : tmp)
        {
            vec.emplace_back(elem, std::to_string(elem));
        }
    }

    // constructors
    {
        // 1
        tstd::map<int, std::string> m1;
        std::map<int, std::string> m2;
        util.assertSequenceEqual(m1, m2);
    }
    {
        // 2
        tstd::map<int, std::string> m1((std::less<int>(), tstd::allocator<int>()));
        std::map<int, std::string> m2((std::less<int>(), std::allocator<int>()));
        util.assertSequenceEqual(m1, m2);
    }
    {
        // 3
        tstd::map<int, std::string> m1((tstd::allocator<int>()));
        std::map<int, std::string> m2((std::allocator<int>()));
        util.assertSequenceEqual(m1, m2);
    }
    {
        // 4
        tstd::map<int, std::string> m1(vec.begin(), vec.end());
        std::map<int, std::string> m2(vec.begin(), vec.end());
        util.assertSequenceEqual(m1, m2);
    }
    {
        // 5
        tstd::map<int, std::string> m1(vec.begin(), vec.end(), tstd::allocator<int>());
        std::map<int, std::string> m2(vec.begin(), vec.end(), std::allocator<int>());
        util.assertSequenceEqual(m1, m2);
        {
            // 6
            tstd::map<int, std::string> m1c(m1);
            std::map<int, std::string> m2c(m2);
            util.assertSequenceEqual(m1c, m2c);
        }
        {
            // 7
            tstd::map<int, std::string> m1c(m1, tstd::allocator<int>());
            std::map<int, std::string> m2c(m2, std::allocator<int>());
            util.assertSequenceEqual(m1c, m2c);
        }
        // 8
        tstd::map<int, std::string> m1m(std::move(m1));
        std::map<int, std::string> m2m(std::move(m2));
        util.assertSequenceEqual(m1, m2);
        util.assertSequenceEqual(m1m, m2m);
        // 9
        tstd::map<int, std::string> m1mm(std::move(m1m), tstd::allocator<int>());
        std::map<int, std::string> m2mm(std::move(m2m), std::allocator<int>());
        util.assertSequenceEqual(m1m, m2m);
        util.assertSequenceEqual(m1mm, m2mm);
    }
    {
        // 10
        tstd::map<int, std::string> m1{{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}, {10, "10"}};
        std::map<int, std::string> m2{{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}, {10, "10"}};
        util.assertSequenceEqual(m1, m2);
    }
    {
        // 11
        tstd::map<int, std::string> m1({{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}, {10, "10"}}, tstd::allocator<int>());
        std::map<int, std::string> m2({{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}, {10, "10"}}, std::allocator<int>());
        util.assertSequenceEqual(m1, m2);
    }
    // assignment
    {
        tstd::map<int, std::string> m1(vec.begin(), vec.end());
        std::map<int, std::string> m2(vec.begin(), vec.end());
        tstd::map<int, std::string> m1c;
        std::map<int, std::string> m2c;
        // 1
        m1c = m1;
        m2c = m2;
        util.assertSequenceEqual(m1c, m2c);
        // 2
        m1c = std::move(m1);
        m2c = std::move(m2);
        util.assertSequenceEqual(m1c, m2c);
        util.assertSequenceEqual(m1, m2);
        // 3
        m1 = {{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}, {10, "10"}};
        m2 = {{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}, {10, "10"}};
        util.assertSequenceEqual(m1, m2);
    }
    // allocator
    {
        tstd::map<int, std::string> m1;
        util.assertEqual(m1.get_allocator() == tstd::allocator<std::pair<const int, std::string>>(), true);
    }
    // iterators
    {
        tstd::map<int, std::string> m1(vec.begin(), vec.end());
        std::map<int, std::string> m2(vec.begin(), vec.end());
        util.assertEqual(*m1.begin(), *m2.begin());
        util.assertRangeEqual(m1.begin(), m1.end(), m2.cbegin());
        util.assertRangeEqual(m1.cbegin(), m1.cend(), m2.begin());
        util.assertRangeEqual(m1.rbegin(), m1.rend(), m2.crbegin());
        util.assertRangeEqual(m1.crbegin(), m1.crend(), m2.rbegin());
        // const version
        {
            const tstd::map<int, std::string> m1(vec.begin(), vec.end());
            const std::map<int, std::string> m2(vec.begin(), vec.end());
            util.assertEqual(*m1.begin(), *m2.begin());
            util.assertRangeEqual(m1.begin(), m1.end(), m2.cbegin());
            util.assertRangeEqual(m1.cbegin(), m1.cend(), m2.begin());
            util.assertRangeEqual(m1.rbegin(), m1.rend(), m2.crbegin());
            util.assertRangeEqual(m1.crbegin(), m1.crend(), m2.rbegin());
        }
    }
    // size and capacity
    {
        tstd::map<int, std::string> m1(vec.begin(), vec.end());
        std::map<int, std::string> m2(vec.begin(), vec.end());
        util.assertEqual(m1.size(), m2.size());
        util.assertEqual(m1.empty(), m2.empty());
    }
    // modifiers
    {
        // clear
        tstd::map<int, std::string> m1(vec.begin(), vec.end());
        std::map<int, std::string> m2(vec.begin(), vec.end());
        m1.clear();
        m2.clear();
        util.assertSequenceEqual(m1, m2);
        // insert
        const std::pair<const int, std::string> ca(88, "88");
        std::pair<const int, std::string> a(99, "99");
        // 1
        auto p1 = m1.insert(ca);
        auto p2 = m2.insert(ca);
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        // 2
        p1 = m1.insert(a);
        p2 = m2.insert(a);
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        // 3
        p1 = m1.insert(std::pair<const int, std::string>(100, "100"));
        p2 = m2.insert(std::pair<const int, std::string>(100, "100"));
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        // 4
        auto iter1 = m1.insert(m1.begin(), ca);
        auto iter2 = m2.insert(m2.begin(), ca);
        util.assertEqual(*iter1, *iter2);
        // 5
        iter1 = m1.insert(m1.begin(), a);
        iter2 = m2.insert(m2.begin(), a);
        util.assertEqual(*iter1, *iter2);
        // 6
        iter1 = m1.insert(m1.begin(), std::pair<const int, std::string>(70, "70"));
        iter2 = m2.insert(m2.begin(), std::pair<const int, std::string>(70, "70"));
        util.assertEqual(*iter1, *iter2);
        // 7
        m1.insert(vec.begin(), vec.end());
        m2.insert(vec.begin(), vec.end());
        util.assertSequenceEqual(m1, m2);
        // 8
        m1.insert({{1, "1"}, {2, "2"}, {3, "3"}, {3, "3"}, {100, "100"}, {1024, "1024"}, {100, "100"}, {99, "99"}, {30143, "30143"}});
        m2.insert({{1, "1"}, {2, "2"}, {3, "3"}, {3, "3"}, {100, "100"}, {1024, "1024"}, {100, "100"}, {99, "99"}, {30143, "30143"}});
        util.assertSequenceEqual(m1, m2);
        // insert_or_assign
        // 1
        int key = 1; // existent key
        p1 = m1.insert_or_assign(key, "1_special");
        p2 = m2.insert_or_assign(key, "1_special");
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        key = 2048; // non-existent key
        p1 = m1.insert_or_assign(key, "2048_special");
        p2 = m2.insert_or_assign(key, "2048_special");
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        util.assertSequenceEqual(m1, m2);
        // 2
        // existent key
        p1 = m1.insert_or_assign(100, "100_special");
        p2 = m2.insert_or_assign(100, "100_special");
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        // non-existent key
        p1 = m1.insert_or_assign(32767, "32767_special");
        p2 = m2.insert_or_assign(32767, "32767_special");
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        util.assertSequenceEqual(m1, m2);
        // 3
        key = 2049;
        iter1 = m1.insert_or_assign(m1.begin(), key, "2049_special");
        iter2 = m2.insert_or_assign(m2.begin(), key, "2049_special");
        util.assertEqual(*iter1, *iter2);
        util.assertSequenceEqual(m1, m2);
        // 4
        iter1 = m1.insert_or_assign(m1.begin(), 100, "100_special");
        iter2 = m2.insert_or_assign(m2.begin(), 100, "100_special");
        util.assertEqual(*iter1, *iter2);
        util.assertSequenceEqual(m1, m2);
        // emplace
        m1.emplace(-100, "-100");
        m2.emplace(-100, "-100");
        util.assertSequenceEqual(m1, m2);
        // emplace_hint
        m1.emplace_hint(m1.begin(), -1001, "-1001");
        m2.emplace_hint(m2.begin(), -1001, "-1001");
        util.assertSequenceEqual(m1, m2);
        // try_emplace
        key = 1;
        // 1
        p1 = m1.try_emplace(key, "1_replace");
        p2 = m2.try_emplace(key, "1_replace");
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        // 2
        p1 = m1.try_emplace(2, "2_replace");
        p2 = m2.try_emplace(2, "2_replace");
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        // 3
        key = 3;
        iter1 = m1.try_emplace(m1.begin(), key, "3_replace");
        iter2 = m2.try_emplace(m2.begin(), key, "3_replace");
        util.assertEqual(*iter1, *iter2);
        // 4
        iter1 = m1.try_emplace(m1.begin(), 4, "4_replace");
        iter2 = m2.try_emplace(m2.begin(), 4, "4_replace");
        util.assertEqual(*iter1, *iter2);
        util.assertSequenceEqual(m1, m2);
        // erase
        // 1
        iter1 = m1.erase(m1.find(15));
        iter2 = m2.erase(m2.find(15));
        util.assertEqual(*iter1, *iter2);
        util.assertSequenceEqual(m1, m2);
        // 2
        iter1 = m1.erase(m1.find(10), m1.find(30));
        iter2 = m2.erase(m2.find(10), m2.find(30));
        util.assertEqual(*iter1, *iter2);
        util.assertSequenceEqual(m1, m2);
        // 3
        auto count1 = m1.erase(50);
        auto count2 = m2.erase(50);
        util.assertEqual(count1, count2);
        util.assertSequenceEqual(m1, m2);
        // swap
        tstd::map<int, std::string> tmp1(vec.begin(), vec.end());
        std::map<int, std::string> tmp2(vec.begin(), vec.end());
        m1.swap(tmp1);
        m2.swap(tmp2);
        util.assertSequenceEqual(m1, m2);
        util.assertSequenceEqual(tmp1, tmp2);
        swap(m1, tmp1);
        swap(m2, tmp2);
        util.assertSequenceEqual(m1, m2);
        util.assertSequenceEqual(tmp1, tmp2);
        tstd::swap(m1, tmp1);
        std::swap(m2, tmp2);
        util.assertSequenceEqual(m1, m2);
        util.assertSequenceEqual(tmp1, tmp2);
    }
    {
        // lookup
        tstd::map<int, std::string> m1(vec.begin(), vec.end());
        std::map<int, std::string> m2(vec.begin(), vec.end());
        // count
        auto sz1 = m1.count(10);
        auto sz2 = m2.count(10);
        util.assertEqual(sz1, sz2);
        // find
        util.assertEqual(*m1.find(10), *m2.find(10));
        util.assertEqual(m1.find(1000) == m1.end(), m2.find(1000) == m2.end());
        // contains
        util.assertEqual(m1.contains(10), m2.contains(10));
        util.assertEqual(m1.contains(999), m2.contains(999));
        // equal_range
        auto p1 = m1.equal_range(10);
        auto p2 = m2.equal_range(10);
        util.assertEqual(tstd::distance(p1.first, p1.second), tstd::distance(p2.first, p2.second));
        util.assertEqual(*p1.first, *p2.first);
        util.assertEqual(*p1.second, *p2.second);
        // lower_bound
        m1.erase(50);
        m2.erase(50);
        auto l1 = m1.lower_bound(50);
        auto l2 = m2.lower_bound(50);
        util.assertEqual(*l1, *l2);
        // upper_bound
        auto u1 = m1.upper_bound(50);
        auto u2 = m2.upper_bound(50);
        util.assertEqual(*u1, *u2);
        // const version
        {
            const tstd::map<int, std::string> m1(vec.begin(), vec.end());
            const std::map<int, std::string> m2(vec.begin(), vec.end());
            // find
            util.assertEqual(*m1.find(10), *m2.find(10));
            util.assertEqual(m1.find(1000) == m1.end(), m2.find(1000) == m2.end());
            // contains
            util.assertEqual(m1.contains(10), m2.contains(10));
            util.assertEqual(m1.contains(999), m2.contains(999));
            // equal_range
            auto p1 = m1.equal_range(10);
            auto p2 = m2.equal_range(10);
            util.assertEqual(tstd::distance(p1.first, p1.second), tstd::distance(p2.first, p2.second));
            util.assertEqual(*p1.first, *p2.first);
            util.assertEqual(*p1.second, *p2.second);
            // lower_bound
            auto l1 = m1.lower_bound(50);
            auto l2 = m2.lower_bound(50);
            util.assertEqual(*l1, *l2);
            // upper_bound
            auto u1 = m1.upper_bound(50);
            auto u2 = m2.upper_bound(50);
            util.assertEqual(*u1, *u2);
        }
    }
    // template version of erase/count/find/contains/equal_range/lower_bound/upper_bound
    {
        std::vector<std::pair<const Foo, std::string>> vecfoo;
        for (int i = 0; i < 100; ++i)
        {
            vecfoo.emplace_back(Foo(i), std::string("foo(") + std::to_string(i) + ")");
        }
        tstd::map<Foo, std::string> m1(vecfoo.begin(), vecfoo.end());
        std::map<Foo, std::string> m2(vecfoo.begin(), vecfoo.end());
        util.assertSequenceEqual(m1, m2);
        // erase
        auto size1 = m1.erase(10);
        // auto size2 = m2.erase(10); // this version is since C++ 23, so use another version for std::set
        auto size2 = m2.erase(Foo(10));
        util.assertEqual(size1, size2);
        util.assertSequenceEqual(m1, m2);
        // count
        auto count1 = m1.count(10);
        auto count2 = m2.count(10);
        util.assertEqual(count1, count2);
        count1 = m1.count(20);
        count2 = m2.count(20);
        util.assertEqual(count1, count2);
        // find
        auto iter1 = m1.find(10);
        auto iter2 = m2.find(10);
        util.assertEqual(iter1 == m1.end(), iter2 == m2.end());
        iter1 = m1.find(20);
        iter2 = m2.find(20);
        util.assertEqual(*iter1, *iter2);
        // contains
        util.assertEqual(m1.contains(50), m2.contains(50));
        util.assertEqual(m1.contains(500), m2.contains(500));
        // equal_range
        auto p1 = m1.equal_range(50);
        auto p2 = m2.equal_range(50);
        util.assertEqual(tstd::distance(p1.first, p1.second), tstd::distance(p2.first, p2.second));
        util.assertEqual(*p1.first, *p2.first);
        util.assertEqual(*p1.second, *p2.second);
        // lower_bound
        auto l1 = m1.lower_bound(50);
        auto l2 = m2.lower_bound(50);
        util.assertEqual(*l1, *l2);
        // upper_bound
        auto u1 = m1.upper_bound(50);
        auto u2 = m2.upper_bound(50);
        util.assertEqual(*u1, *u2);
        // const version
        {
            const tstd::map<Foo, std::string> m1(vecfoo.begin(), vecfoo.end());
            const std::map<Foo, std::string> m2(vecfoo.begin(), vecfoo.end());
            util.assertSequenceEqual(m1, m2);
            // count
            auto count1 = m1.count(10);
            auto count2 = m2.count(10);
            util.assertEqual(count1, count2);
            count1 = m1.count(20);
            count2 = m2.count(20);
            util.assertEqual(count1, count2);
            // find
            auto iter1 = m1.find(10);
            auto iter2 = m2.find(10);
            util.assertEqual(iter1 == m1.end(), iter2 == m2.end());
            iter1 = m1.find(20);
            iter2 = m2.find(20);
            util.assertEqual(*iter1, *iter2);
            // equal_range
            auto p1 = m1.equal_range(50);
            auto p2 = m2.equal_range(50);
            util.assertEqual(tstd::distance(p1.first, p1.second), tstd::distance(p2.first, p2.second));
            util.assertEqual(*p1.first, *p2.first);
            util.assertEqual(*p1.second, *p2.second);
            // lower_bound
            auto l1 = m1.lower_bound(50);
            auto l2 = m2.lower_bound(50);
            util.assertEqual(*l1, *l2);
            // upper_bound
            auto u1 = m1.upper_bound(50);
            auto u2 = m2.upper_bound(50);
            util.assertEqual(*u1, *u2);
        }
    }
    // observers
    {
        tstd::map<int, std::string> m1(vec.begin(), vec.end());
        std::map<int, std::string> m2(vec.begin(), vec.end());
        util.assertEqual(typeid(m1.key_comp()) == typeid(std::less<int>), true);
        util.assertEqual(typeid(m2.key_comp()) == typeid(std::less<int>), true);
        util.assertEqual(typeid(m1.value_comp()) == typeid(tstd::map<int, std::string>::value_compare), true);
        util.assertEqual(typeid(m2.value_comp()) == typeid(std::map<int, std::string>::value_compare), true);
    }
    // comparisons
    {
        // comparisons
        tstd::map<int, std::string> m1(vec.begin(), vec.end());
        tstd::map<int, std::string> m2(vec.begin(), vec.end());
        util.assertEqual(m1 == m2, true);
        util.assertEqual(m1 != m2, false);
        m1.erase(10);
        util.assertEqual(m1 != m2, true);
        util.assertEqual(m1 > m2, true);
        util.assertEqual(m1 >= m2, true);
        util.assertEqual(m1 < m2, false);
        util.assertEqual(m1 <= m2, false);
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