#include <iostream>
#include <cstddef>
#include <iterator>
#include <unordered_set>
#include <unordered_map>
#include <tunordered_set.hpp>
#include <tunordered_map.hpp>
#include "TestUtil.hpp"

void testUnorderedSet(bool showDetails);
void testUnorderedMultiset(bool showDetails);
void testUnorderedMap(bool showDetails);
void testUnorderedMultimap(bool showDetails);

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testUnorderedSet(showDetails);
    testUnorderedMultiset(showDetails);
    testUnorderedMap(showDetails);
    testUnorderedMultimap(showDetails);
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
    friend class std::hash<Foo>;
public:
    Foo(int _a = 0) : a(_a) {}
    Foo(const Foo& foo) = default;
    Foo& operator=(const Foo& foo) = default;
    bool operator==(const Foo& foo) const = default;
private:
    int a;
};

template<>
struct std::hash<Foo>
{
    std::size_t operator()(const Foo& foo) const
    {
        return std::hash<int>()(foo.a);
    }
};

void testUnorderedSet(bool showDetails)
{
    TestUtil util(showDetails, "unordered_set");
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::random_shuffle(vec.begin(), vec.end());
    // constructors
    {
        // 1
        tstd::unordered_set<int> s1;
        std::unordered_set<int> s2;
        util.assertSetEqual(s1, s2);
    }
    {
        // 2
        tstd::unordered_set<int> s1(20, std::hash<int>(), std::equal_to<int>(), tstd::allocator<int>());
        std::unordered_set<int> s2(20, std::hash<int>(), std::equal_to<int>(), std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    {
        // 3
        tstd::unordered_set<int> s1(20, tstd::allocator<int>());
        std::unordered_set<int> s2(20, std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    {
        // 4
        tstd::unordered_set<int> s1(20, std::hash<int>(), tstd::allocator<int>());
        std::unordered_set<int> s2(20, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    {
        // 5
        tstd::unordered_set<int> s1((tstd::allocator<int>()));
        std::unordered_set<int> s2((std::allocator<int>()));
        util.assertSetEqual(s1, s2);
    }
    {
        // 6
        tstd::unordered_set<int> s1(vec.begin(), vec.end());
        std::unordered_set<int> s2(vec.begin(), vec.end());
        util.assertSetEqual(s1, s2);
    }
    {
        // 7
        tstd::unordered_set<int> s1(vec.begin(), vec.end(), 100, tstd::allocator<int>());
        std::unordered_set<int> s2(vec.begin(), vec.end(), 100, std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    {
        // 8
        tstd::unordered_set<int> s1(vec.begin(), vec.end(), 100, std::hash<int>(), tstd::allocator<int>());
        std::unordered_set<int> s2(vec.begin(), vec.end(), 100, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(s1, s2);
        {
            // 9
            tstd::unordered_set<int> s1c(s1);
            std::unordered_set<int> s2c(s2);
            util.assertSetEqual(s1c, s2c);
        }
        {
            // 10
            tstd::unordered_set<int> s1c(s1, tstd::allocator<int>());
            std::unordered_set<int> s2c(s2, std::allocator<int>());
            util.assertSetEqual(s1c, s2c);
        }
        // 11
        tstd::unordered_set<int> s1m(std::move(s1));
        std::unordered_set<int> s2m(std::move(s2));
        util.assertSetEqual(s1, s2);
        util.assertSetEqual(s1m, s2m);
        // 12
        tstd::unordered_set<int> s1mm(std::move(s1m), tstd::allocator<int>());
        std::unordered_set<int> s2mm(std::move(s2m), std::allocator<int>());
        util.assertSetEqual(s1m, s2m);
        util.assertSetEqual(s1mm, s2mm);
    }
    {
        // 13
        tstd::unordered_set<int> s1{2, 1, 3, 4, 4, 10};
        std::unordered_set<int> s2{2, 1, 3, 4, 4, 10};
        util.assertSetEqual(s1, s2);
    }
    {
        // 14
        tstd::unordered_set<int> s1({2, 1, 3, 4, 4, 10}, 10, tstd::allocator<int>());
        std::unordered_set<int> s2({2, 1, 3, 4, 4, 10}, 10, std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    {
        // 15
        tstd::unordered_set<int> s1({2, 1, 3, 4, 4, 10}, 10, std::hash<int>(), tstd::allocator<int>());
        std::unordered_set<int> s2({2, 1, 3, 4, 4, 10}, 10, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    // assignment
    {
        tstd::unordered_set<int> s1(vec.begin(), vec.end());
        std::unordered_set<int> s2(vec.begin(), vec.end());
        tstd::unordered_set<int> s1c;
        std::unordered_set<int> s2c;
        // 1
        s1c = s1;
        s2c = s2;
        util.assertSetEqual(s1c, s2c);
        // 2
        s1c = std::move(s1);
        s2c = std::move(s2);
        util.assertSetEqual(s1c, s2c);
        util.assertSetEqual(s1, s2);
        // 3
        s1 = {2, 1, 3, 4, 4, 10};
        s2 = {2, 1, 3, 4, 4, 10};
        util.assertSetEqual(s1, s2);
    }
    // allocator
    {
        tstd::unordered_set<int> s1;
        util.assertEqual(s1.get_allocator() == tstd::allocator<int>(), true);
    }
    // iterators
    {
        tstd::unordered_set<int> s1(vec.begin(), vec.end());
        std::unordered_set<int> s2(vec.begin(), vec.end());
        util.assertSetEqual(s1.begin(), s1.end(), s2.cbegin(), s2.cend());
        util.assertSetEqual(s1.cbegin(), s1.cend(), s2.begin(), s2.end());
        // const version
        {
            const tstd::unordered_set<int> s1(vec.begin(), vec.end());
            const std::unordered_set<int> s2(vec.begin(), vec.end());
            util.assertSetEqual(s1.begin(), s1.end(), s2.cbegin(), s2.cend());
            util.assertSetEqual(s1.cbegin(), s1.cend(), s2.begin(), s2.cend());
        }
    }
    // size and capacity
    {
        tstd::unordered_set<int> s1(vec.begin(), vec.end());
        std::unordered_set<int> s2(vec.begin(), vec.end());
        util.assertEqual(s1.size(), s2.size());
        util.assertEqual(s1.empty(), s2.empty());
    }
    // modifiers
    {
        tstd::unordered_set<int> s1(vec.begin(), vec.end());
        std::unordered_set<int> s2(vec.begin(), vec.end());
        // clear
        s1.clear();
        s2.clear();
        util.assertSetEqual(s1, s2);
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
        util.assertSetEqual(s1, s2);
        // 6
        s1.insert({1, 2, 3, 3, 100, 1024, 100, 99, 30143});
        s2.insert({1, 2, 3, 3, 100, 1024, 100, 99, 30143});
        util.assertSetEqual(s1, s2);
        // emplace
        s1.emplace(-100);
        s2.emplace(-100);
        util.assertSetEqual(s1, s2);
        // emplace_hint
        s1.emplace_hint(s1.begin(), -1001);
        s2.emplace_hint(s2.begin(), -1001);
        util.assertSetEqual(s1, s2);
        // erase
        // 1
        iter1 = s1.erase(s1.find(15));
        iter2 = s2.erase(s2.find(15));
        util.assertSetEqual(s1, s2);
        // 2
        auto range1 = s1.equal_range(20);
        auto range2 = s2.equal_range(20);
        iter1 = s1.erase(range1.first, range1.second);
        iter2 = s2.erase(range2.first, range2.second);
        util.assertSetEqual(s1, s2);
        // 3
        auto count1 = s1.erase(50);
        auto count2 = s2.erase(50);
        util.assertEqual(count1, count2);
        util.assertSetEqual(s1, s2);
        // swap
        tstd::unordered_set<int> tmp1(vec.begin(), vec.end());
        std::unordered_set<int> tmp2(vec.begin(), vec.end());
        s1.swap(tmp1);
        s2.swap(tmp2);
        util.assertSetEqual(s1, s2);
        util.assertSetEqual(tmp1, tmp2);
        swap(s1, tmp1);
        swap(s2, tmp2);
        util.assertSetEqual(s1, s2);
        util.assertSetEqual(tmp1, tmp2);
        tstd::swap(s1, tmp1);
        std::swap(s2, tmp2);
        util.assertSetEqual(s1, s2);
        util.assertSetEqual(tmp1, tmp2);
    }
    {
        // lookup
        tstd::unordered_set<int> s1(vec.begin(), vec.end());
        std::unordered_set<int> s2(vec.begin(), vec.end());
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
        // const version
        {
            const tstd::unordered_set<int> s1(vec.begin(), vec.end());
            const std::unordered_set<int> s2(vec.begin(), vec.end());
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
        }
    }
    // template version of erase/count/find/contains/equal_range/lower_bound/upper_bound
    {
        std::vector<Foo> vecfoo;
        for (int i = 0; i < 100; ++i)
        {
            vecfoo.emplace_back(i);
        }
        tstd::unordered_set<Foo> s1(vecfoo.begin(), vecfoo.end());
        std::unordered_set<Foo> s2(vecfoo.begin(), vecfoo.end());
        util.assertSetEqual(s1, s2);
        // erase
        auto size1 = s1.erase(10);
        // auto size2 = s2.erase(10); // this version is since C++ 23, so use another version for std::unordered_set
        auto size2 = s2.erase(Foo(10));
        util.assertEqual(size1, size2);
        util.assertSetEqual(s1, s2);
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
        // const version
        {
            const tstd::unordered_set<Foo> s1(vecfoo.begin(), vecfoo.end());
            const std::unordered_set<Foo> s2(vecfoo.begin(), vecfoo.end());
            util.assertSetEqual(s1, s2);
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
        }
    }
    {
        // bucket interfaces
        tstd::unordered_set<int> s1(vec.begin(), vec.end(), 100);
        std::unordered_set<int> s2(vec.begin(), vec.end(), 100);
        // local iterators
        util.assertEqual(std::distance(s1.begin(1), s1.end(1)), std::distance(s2.begin(1), s2.end(1)));
        util.assertEqual(std::distance(s1.cbegin(10), s1.cend(10)), std::distance(s2.cbegin(10), s2.cend(10)));
        util.assertEqual(s1.bucket_count() >= 100, s2.bucket_count() >= 100);
        util.assertEqual(s1.bucket_size(1), s2.bucket_size(1));
        // hash policy
        util.assertEqual(s1.max_load_factor(), s2.max_load_factor());
        util.assertEqual(s1.load_factor() <= s1.max_load_factor(), true);
        s1.rehash(200);
        s2.rehash(200);
        util.assertEqual(s1.bucket_count() >= 200, s2.bucket_count() >= 200);
        s1.max_load_factor(0.8);
        s2.max_load_factor(0.8);
        util.assertEqual(s1.max_load_factor(), s2.max_load_factor());
        s1.rehash(300);
        s2.rehash(300);
        util.assertEqual(s1.bucket_count() >= 300, s2.bucket_count() >= 300);
        s1.reserve(400);
        s2.reserve(400);
        util.assertEqual(s1.bucket_count() >= 400, s2.bucket_count() >= 400);
    }
    // observers
    {
        tstd::unordered_set<int> s1(vec.begin(), vec.end());
        std::unordered_set<int> s2(vec.begin(), vec.end());
        util.assertEqual(typeid(s1.hash_function()) == typeid(std::hash<int>), true);
        util.assertEqual(typeid(s2.hash_function()) == typeid(std::hash<int>), true);
        util.assertEqual(typeid(s1.key_eq()) == typeid(std::equal_to<int>), true);
        util.assertEqual(typeid(s2.key_eq()) == typeid(std::equal_to<int>), true);
    }
    // comparisons
    {
        // comparisons
        tstd::unordered_set<int> s1(vec.begin(), vec.end());
        tstd::unordered_set<int> s2(vec.begin(), vec.end());
        util.assertEqual(s1 == s2, true);
        util.assertEqual(s1 != s2, false);
        s1.erase(10);
        util.assertEqual(s1 == s2, false);
        util.assertEqual(s1 != s2, true);
    }
    util.showFinalResult();
}

void testUnorderedMultiset(bool showDetails)
{
    TestUtil util(showDetails, "unordered_multiset");
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    vec.resize(300);
    std::copy(vec.begin(), vec.begin() + 100, vec.begin() + 100);
    std::copy(vec.begin(), vec.begin() + 100, vec.begin() + 200);
    std::random_shuffle(vec.begin(), vec.end());
    // constructors
    {
        // 1
        tstd::unordered_multiset<int> s1;
        std::unordered_multiset<int> s2;
        util.assertSetEqual(s1, s2);
    }
    {
        // 2
        tstd::unordered_multiset<int> s1(20, std::hash<int>(), std::equal_to<int>(), tstd::allocator<int>());
        std::unordered_multiset<int> s2(20, std::hash<int>(), std::equal_to<int>(), std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    {
        // 3
        tstd::unordered_multiset<int> s1(20, tstd::allocator<int>());
        std::unordered_multiset<int> s2(20, std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    {
        // 4
        tstd::unordered_multiset<int> s1(20, std::hash<int>(), tstd::allocator<int>());
        std::unordered_multiset<int> s2(20, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    {
        // 5
        tstd::unordered_multiset<int> s1((tstd::allocator<int>()));
        std::unordered_multiset<int> s2((std::allocator<int>()));
        util.assertSetEqual(s1, s2);
    }
    {
        // 6
        tstd::unordered_multiset<int> s1(vec.begin(), vec.end());
        std::unordered_multiset<int> s2(vec.begin(), vec.end());
        util.assertSetEqual(s1, s2);
    }
    {
        // 7
        tstd::unordered_multiset<int> s1(vec.begin(), vec.end(), 100, tstd::allocator<int>());
        std::unordered_multiset<int> s2(vec.begin(), vec.end(), 100, std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    {
        // 8
        tstd::unordered_multiset<int> s1(vec.begin(), vec.end(), 100, std::hash<int>(), tstd::allocator<int>());
        std::unordered_multiset<int> s2(vec.begin(), vec.end(), 100, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(s1, s2);
        {
            // 9
            tstd::unordered_multiset<int> s1c(s1);
            std::unordered_multiset<int> s2c(s2);
            util.assertSetEqual(s1c, s2c);
        }
        {
            // 10
            tstd::unordered_multiset<int> s1c(s1, tstd::allocator<int>());
            std::unordered_multiset<int> s2c(s2, std::allocator<int>());
            util.assertSetEqual(s1c, s2c);
        }
        // 11
        tstd::unordered_multiset<int> s1m(std::move(s1));
        std::unordered_multiset<int> s2m(std::move(s2));
        util.assertSetEqual(s1, s2);
        util.assertSetEqual(s1m, s2m);
        // 12
        tstd::unordered_multiset<int> s1mm(std::move(s1m), tstd::allocator<int>());
        std::unordered_multiset<int> s2mm(std::move(s2m), std::allocator<int>());
        util.assertSetEqual(s1m, s2m);
        util.assertSetEqual(s1mm, s2mm);
    }
    {
        // 13
        tstd::unordered_multiset<int> s1{2, 1, 3, 4, 4, 10};
        std::unordered_multiset<int> s2{2, 1, 3, 4, 4, 10};
        util.assertSetEqual(s1, s2);
    }
    {
        // 14
        tstd::unordered_multiset<int> s1({2, 1, 3, 4, 4, 10}, 10, tstd::allocator<int>());
        std::unordered_multiset<int> s2({2, 1, 3, 4, 4, 10}, 10, std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    {
        // 15
        tstd::unordered_multiset<int> s1({2, 1, 3, 4, 4, 10}, 10, std::hash<int>(), tstd::allocator<int>());
        std::unordered_multiset<int> s2({2, 1, 3, 4, 4, 10}, 10, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(s1, s2);
    }
    // assignment
    {
        tstd::unordered_multiset<int> s1(vec.begin(), vec.end());
        std::unordered_multiset<int> s2(vec.begin(), vec.end());
        tstd::unordered_multiset<int> s1c;
        std::unordered_multiset<int> s2c;
        // 1
        s1c = s1;
        s2c = s2;
        util.assertSetEqual(s1c, s2c);
        // 2
        s1c = std::move(s1);
        s2c = std::move(s2);
        util.assertSetEqual(s1c, s2c);
        util.assertSetEqual(s1, s2);
        // 3
        s1 = {2, 1, 3, 4, 4, 10};
        s2 = {2, 1, 3, 4, 4, 10};
        util.assertSetEqual(s1, s2);
    }
    // allocator
    {
        tstd::unordered_multiset<int> s1;
        util.assertEqual(s1.get_allocator() == tstd::allocator<int>(), true);
    }
    // iterators
    {
        tstd::unordered_multiset<int> s1(vec.begin(), vec.end());
        std::unordered_multiset<int> s2(vec.begin(), vec.end());
        util.assertSetEqual(s1.begin(), s1.end(), s2.cbegin(), s2.cend());
        util.assertSetEqual(s1.cbegin(), s1.cend(), s2.begin(), s2.end());
        // const version
        {
            const tstd::unordered_multiset<int> s1(vec.begin(), vec.end());
            const std::unordered_multiset<int> s2(vec.begin(), vec.end());
            util.assertSetEqual(s1.begin(), s1.end(), s2.cbegin(), s2.cend());
            util.assertSetEqual(s1.cbegin(), s1.cend(), s2.begin(), s2.cend());
        }
    }
    // size and capacity
    {
        tstd::unordered_multiset<int> s1(vec.begin(), vec.end());
        std::unordered_multiset<int> s2(vec.begin(), vec.end());
        util.assertEqual(s1.size(), s2.size());
        util.assertEqual(s1.empty(), s2.empty());
    }
    // modifiers
    {
        tstd::unordered_multiset<int> s1(vec.begin(), vec.end());
        std::unordered_multiset<int> s2(vec.begin(), vec.end());
        // clear
        s1.clear();
        s2.clear();
        util.assertSetEqual(s1, s2);
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
        util.assertSetEqual(s1, s2);
        // 6
        s1.insert({1, 2, 3, 3, 100, 1024, 100, 99, 30143});
        s2.insert({1, 2, 3, 3, 100, 1024, 100, 99, 30143});
        util.assertSetEqual(s1, s2);
        // emplace
        s1.emplace(-100);
        s2.emplace(-100);
        util.assertSetEqual(s1, s2);
        // emplace_hint
        s1.emplace_hint(s1.begin(), -1001);
        s2.emplace_hint(s2.begin(), -1001);
        util.assertSetEqual(s1, s2);
        // erase
        // 1
        iter1 = s1.erase(s1.find(15));
        iter2 = s2.erase(s2.find(15));
        util.assertSetEqual(s1, s2);
        // 2
        auto range1 = s1.equal_range(20);
        auto range2 = s2.equal_range(20);
        iter1 = s1.erase(range1.first, range1.second);
        iter2 = s2.erase(range2.first, range2.second);
        util.assertSetEqual(s1, s2);
        // 3
        auto count1 = s1.erase(50);
        auto count2 = s2.erase(50);
        util.assertEqual(count1, count2);
        util.assertSetEqual(s1, s2);
        // swap
        tstd::unordered_multiset<int> tmp1(vec.begin(), vec.end());
        std::unordered_multiset<int> tmp2(vec.begin(), vec.end());
        s1.swap(tmp1);
        s2.swap(tmp2);
        util.assertSetEqual(s1, s2);
        util.assertSetEqual(tmp1, tmp2);
        swap(s1, tmp1);
        swap(s2, tmp2);
        util.assertSetEqual(s1, s2);
        util.assertSetEqual(tmp1, tmp2);
        tstd::swap(s1, tmp1);
        std::swap(s2, tmp2);
        util.assertSetEqual(s1, s2);
        util.assertSetEqual(tmp1, tmp2);
    }
    {
        // lookup
        tstd::unordered_multiset<int> s1(vec.begin(), vec.end());
        std::unordered_multiset<int> s2(vec.begin(), vec.end());
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
        // const version
        {
            const tstd::unordered_multiset<int> s1(vec.begin(), vec.end());
            const std::unordered_multiset<int> s2(vec.begin(), vec.end());
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
        }
    }
    // template version of erase/count/find/contains/equal_range/lower_bound/upper_bound
    {
        std::vector<Foo> vecfoo;
        for (int i = 0; i < 100; ++i)
        {
            vecfoo.push_back(Foo(i));
            vecfoo.emplace_back(i);
        }
        tstd::unordered_multiset<Foo> s1(vecfoo.begin(), vecfoo.end());
        std::unordered_multiset<Foo> s2(vecfoo.begin(), vecfoo.end());
        util.assertSetEqual(s1, s2);
        // erase
        auto size1 = s1.erase(10);
        // auto size2 = s2.erase(10); // this version is since C++ 23, so use another version for std::unordered_multiset
        auto size2 = s2.erase(Foo(10));
        util.assertEqual(size1, size2);
        util.assertSetEqual(s1, s2);
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
        // const version
        {
            const tstd::unordered_multiset<Foo> s1(vecfoo.begin(), vecfoo.end());
            const std::unordered_multiset<Foo> s2(vecfoo.begin(), vecfoo.end());
            util.assertSetEqual(s1, s2);
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
        }
    }
    {
        // bucket interfaces
        tstd::unordered_multiset<int> s1(vec.begin(), vec.end(), 200);
        std::unordered_multiset<int> s2(vec.begin(), vec.end(), 200);
        // local iterators
        util.assertEqual(std::distance(s1.begin(1), s1.end(1)), std::distance(s2.begin(1), s2.end(1)));
        util.assertEqual(std::distance(s1.cbegin(10), s1.cend(10)), std::distance(s2.cbegin(10), s2.cend(10)));
        util.assertEqual(s1.bucket_count() >= 200, s2.bucket_count() >= 200);
        util.assertEqual(s1.bucket_size(1), s2.bucket_size(1));
        // hash policy
        util.assertEqual(s1.max_load_factor(), s2.max_load_factor());
        util.assertEqual(s1.load_factor() <= s1.max_load_factor(), true);
        s1.rehash(500);
        s2.rehash(500);
        util.assertEqual(s1.bucket_count() >= 500, s2.bucket_count() >= 500);
        s1.max_load_factor(0.8);
        s2.max_load_factor(0.8);
        util.assertEqual(s1.max_load_factor(), s2.max_load_factor());
        s1.rehash(600);
        s2.rehash(600);
        util.assertEqual(s1.bucket_count() >= 600, s2.bucket_count() >= 600);
        s1.reserve(800);
        s2.reserve(800);
        util.assertEqual(s1.bucket_count() >= 1000, s2.bucket_count() >= 1000);
    }
    // observers
    {
        tstd::unordered_multiset<int> s1(vec.begin(), vec.end());
        std::unordered_multiset<int> s2(vec.begin(), vec.end());
        util.assertEqual(typeid(s1.hash_function()) == typeid(std::hash<int>), true);
        util.assertEqual(typeid(s2.hash_function()) == typeid(std::hash<int>), true);
        util.assertEqual(typeid(s1.key_eq()) == typeid(std::equal_to<int>), true);
        util.assertEqual(typeid(s2.key_eq()) == typeid(std::equal_to<int>), true);
    }
    // comparisons
    {
        // comparisons
        tstd::unordered_multiset<int> s1(vec.begin(), vec.end());
        tstd::unordered_multiset<int> s2(vec.begin(), vec.end());
        util.assertEqual(s1 == s2, true);
        util.assertEqual(s1 != s2, false);
        s1.erase(10);
        util.assertEqual(s1 == s2, false);
        util.assertEqual(s1 != s2, true);
    }
    util.showFinalResult();
}

void testUnorderedMap(bool showDetails)
{
    TestUtil util(showDetails, "unordered_map");
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
        tstd::unordered_map<int, std::string> m1;
        std::unordered_map<int, std::string> m2;
        util.assertSetEqual(m1, m2);
    }
    {
        // 2
        tstd::unordered_map<int, std::string> m1(20, std::hash<int>(), std::equal_to<int>(), tstd::allocator<int>());
        std::unordered_map<int, std::string> m2(20, std::hash<int>(), std::equal_to<int>(), std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    {
        // 3
        tstd::unordered_map<int, std::string> m1(20, tstd::allocator<int>());
        std::unordered_map<int, std::string> m2(20, std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    {
        // 4
        tstd::unordered_map<int, std::string> m1(20, std::hash<int>(), tstd::allocator<int>());
        std::unordered_map<int, std::string> m2(20, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    {
        // 5
        tstd::unordered_map<int, std::string> m1((tstd::allocator<int>()));
        std::unordered_map<int, std::string> m2((std::allocator<int>()));
        util.assertSetEqual(m1, m2);
    }
    {
        // 6
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_map<int, std::string> m2(vec.begin(), vec.end());
        util.assertSetEqual(m1, m2);
    }
    {
        // 7
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end(), 100, tstd::allocator<int>());
        std::unordered_map<int, std::string> m2(vec.begin(), vec.end(), 100, std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    {
        // 8
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end(), 100, std::hash<int>(), tstd::allocator<int>());
        std::unordered_map<int, std::string> m2(vec.begin(), vec.end(), 100, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(m1, m2);
        {
            // 9
            tstd::unordered_map<int, std::string> m1c(m1);
            std::unordered_map<int, std::string> m2c(m2);
            util.assertSetEqual(m1c, m2c);
        }
        {
            // 10
            tstd::unordered_map<int, std::string> m1c(m1, tstd::allocator<int>());
            std::unordered_map<int, std::string> m2c(m2, std::allocator<int>());
            util.assertSetEqual(m1c, m2c);
        }
        // 11
        tstd::unordered_map<int, std::string> m1m(std::move(m1));
        std::unordered_map<int, std::string> s2m(std::move(m2));
        util.assertSetEqual(m1, m2);
        util.assertSetEqual(m1m, s2m);
        // 12
        tstd::unordered_map<int, std::string> s1mm(std::move(m1m), tstd::allocator<int>());
        std::unordered_map<int, std::string> s2mm(std::move(s2m), std::allocator<int>());
        util.assertSetEqual(m1m, s2m);
        util.assertSetEqual(s1mm, s2mm);
    }
    {
        // 13
        tstd::unordered_map<int, std::string> m1{{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}};
        std::unordered_map<int, std::string> m2{{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}};
        util.assertSetEqual(m1, m2);
    }
    {
        // 14
        tstd::unordered_map<int, std::string> m1({{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}}, 10, tstd::allocator<int>());
        std::unordered_map<int, std::string> m2({{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}}, 10, std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    {
        // 15
        tstd::unordered_map<int, std::string> m1({{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}}, 10, std::hash<int>(), tstd::allocator<int>());
        std::unordered_map<int, std::string> m2({{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}}, 10, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    // assignment
    {
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_map<int, std::string> m2(vec.begin(), vec.end());
        tstd::unordered_map<int, std::string> m1c;
        std::unordered_map<int, std::string> m2c;
        // 1
        m1c = m1;
        m2c = m2;
        util.assertSetEqual(m1c, m2c);
        // 2
        m1c = std::move(m1);
        m2c = std::move(m2);
        util.assertSetEqual(m1c, m2c);
        util.assertSetEqual(m1, m2);
        // 3
        m1 = {{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}, {10, "10"}};
        m2 = {{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}, {10, "10"}};
        util.assertSetEqual(m1, m2);
    }
    // allocator
    {
        tstd::unordered_map<int, std::string> m1;
        util.assertEqual(m1.get_allocator() == tstd::allocator<std::pair<const int, std::string>>(), true);
    }
    // element access
    {
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_map<int, std::string> m2(vec.begin(), vec.end());
        // at, operator[]
        util.assertEqual(m1.at(10), m2.at(10));
        util.assertEqual(m1[10], m2[10]);
        util.assertEqual(m1[100], m2[100]);
        m1[100] = m2[100] = "100_at";
        util.assertEqual(m1[100], m2[100]);
    }
    // iterators
    {
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_map<int, std::string> m2(vec.begin(), vec.end());
        util.assertSetEqual(m1.begin(), m1.end(), m2.cbegin(), m2.cend());
        util.assertSetEqual(m1.cbegin(), m1.cend(), m2.begin(), m2.end());
        // const version
        {
            const tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end());
            const std::unordered_map<int, std::string> m2(vec.begin(), vec.end());
            util.assertSetEqual(m1.begin(), m1.end(), m2.cbegin(), m2.cend());
            util.assertSetEqual(m1.cbegin(), m1.cend(), m2.begin(), m2.end());
        }
    }
    // size and capacity
    {
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_map<int, std::string> m2(vec.begin(), vec.end());
        util.assertEqual(m1.size(), m2.size());
        util.assertEqual(m1.empty(), m2.empty());
    }
    // modifiers
    {
        // clear
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_map<int, std::string> m2(vec.begin(), vec.end());
        m1.clear();
        m2.clear();
        util.assertSetEqual(m1, m2);
        // insert
        const std::pair<const int, std::string> ca(88, "88");
        std::pair<const int, std::string> a(99, "99");
        // 1
        auto p1 = m1.insert(ca);
        auto p2 = m2.insert(ca);
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        // 2
        p1 = m1.insert(std::pair<const int, std::string>(100, "100"));
        p2 = m2.insert(std::pair<const int, std::string>(100, "100"));
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        // 3
        p1 = m1.insert(a);
        p2 = m2.insert(a);
        util.assertEqual(*(p1.first), *(p2.first));
        util.assertEqual(p1.second, p2.second);
        // 4
        auto iter1 = m1.insert(m1.begin(), ca);
        auto iter2 = m2.insert(m2.begin(), ca);
        util.assertEqual(*iter1, *iter2);
        // 5
        iter1 = m1.insert(m1.begin(), std::pair<const int, std::string>(70, "70"));
        iter2 = m2.insert(m2.begin(), std::pair<const int, std::string>(70, "70"));
        util.assertEqual(*iter1, *iter2);
        // 6
        iter1 = m1.insert(m1.begin(), a);
        iter2 = m2.insert(m2.begin(), a);
        util.assertEqual(*iter1, *iter2);
        // 7
        m1.insert(vec.begin(), vec.end());
        m2.insert(vec.begin(), vec.end());
        util.assertSetEqual(m1, m2);
        // 8
        m1.insert({{1, "1"}, {2, "2"}, {3, "3"}, {3, "3"}, {100, "100"}, {1024, "1024"}, {100, "100"}, {99, "99"}, {30143, "30143"}});
        m2.insert({{1, "1"}, {2, "2"}, {3, "3"}, {3, "3"}, {100, "100"}, {1024, "1024"}, {100, "100"}, {99, "99"}, {30143, "30143"}});
        util.assertSetEqual(m1, m2);
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
        util.assertSetEqual(m1, m2);
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
        util.assertSetEqual(m1, m2);
        // 3
        key = 2049;
        iter1 = m1.insert_or_assign(m1.begin(), key, "2049_special");
        iter2 = m2.insert_or_assign(m2.begin(), key, "2049_special");
        util.assertEqual(*iter1, *iter2);
        util.assertSetEqual(m1, m2);
        // 4
        iter1 = m1.insert_or_assign(m1.begin(), 100, "100_special");
        iter2 = m2.insert_or_assign(m2.begin(), 100, "100_special");
        util.assertEqual(*iter1, *iter2);
        util.assertSetEqual(m1, m2);
        // emplace
        m1.emplace(-100, "-100");
        m2.emplace(-100, "-100");
        util.assertSetEqual(m1, m2);
        // emplace_hint
        m1.emplace_hint(m1.begin(), -1001, "-1001");
        m2.emplace_hint(m2.begin(), -1001, "-1001");
        util.assertSetEqual(m1, m2);
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
        util.assertSetEqual(m1, m2);
        // erase
        // 1
        iter1 = m1.erase(m1.find(15));
        iter2 = m2.erase(m2.find(15));
        util.assertSetEqual(m1, m2);
        // 2
        auto range1 = m1.equal_range(20);
        auto range2 = m2.equal_range(20);
        iter1 = m1.erase(range1.first, range1.second);
        iter2 = m2.erase(range2.first, range2.second);
        util.assertSetEqual(m1, m2);
        // 3
        auto count1 = m1.erase(50);
        auto count2 = m2.erase(50);
        util.assertEqual(count1, count2);
        util.assertSetEqual(m1, m2);
        // swap
        tstd::unordered_map<int, std::string> tmp1(vec.begin(), vec.end());
        std::unordered_map<int, std::string> tmp2(vec.begin(), vec.end());
        m1.swap(tmp1);
        m2.swap(tmp2);
        util.assertSetEqual(m1, m2);
        util.assertSetEqual(tmp1, tmp2);
        swap(m1, tmp1);
        swap(m2, tmp2);
        util.assertSetEqual(m1, m2);
        util.assertSetEqual(tmp1, tmp2);
        tstd::swap(m1, tmp1);
        std::swap(m2, tmp2);
        util.assertSetEqual(m1, m2);
        util.assertSetEqual(tmp1, tmp2);
    }
    {
        // lookup
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_map<int, std::string> m2(vec.begin(), vec.end());
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
        // const version
        {
            const tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end());
            const std::unordered_map<int, std::string> m2(vec.begin(), vec.end());
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
        }
    }
    // template version of erase/count/find/contains/equal_range/lower_bound/upper_bound
    {
        std::vector<std::pair<const Foo, std::string>> vecfoo;
        for (int i = 0; i < 100; ++i)
        {
            vecfoo.emplace_back(Foo(i), std::string("foo(") + std::to_string(i) + ")");
        }
        tstd::unordered_map<Foo, std::string> m1(vecfoo.begin(), vecfoo.end());
        std::unordered_map<Foo, std::string> m2(vecfoo.begin(), vecfoo.end());
        util.assertSetEqual(m1, m2);
        // erase
        auto size1 = m1.erase(10);
        // auto size2 = m2.erase(10); // this version is since C++ 23, so use another version for std::set
        auto size2 = m2.erase(Foo(10));
        util.assertEqual(size1, size2);
        util.assertSetEqual(m1, m2);
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
        // const version
        {
            const tstd::unordered_map<Foo, std::string> m1(vecfoo.begin(), vecfoo.end());
            const std::unordered_map<Foo, std::string> m2(vecfoo.begin(), vecfoo.end());
            util.assertSetEqual(m1, m2);
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
        }
    }
    {
        // bucket interfaces
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end(), 100);
        std::unordered_map<int, std::string> m2(vec.begin(), vec.end(), 100);
        // local iterators
        util.assertEqual(std::distance(m1.begin(1), m1.end(1)), std::distance(m2.begin(1), m2.end(1)));
        util.assertEqual(std::distance(m1.cbegin(10), m1.cend(10)), std::distance(m2.cbegin(10), m2.cend(10)));
        util.assertEqual(m1.bucket_count() >= 100, m2.bucket_count() >= 100);
        util.assertEqual(m1.bucket_size(1), m2.bucket_size(1));
        // hash policy
        util.assertEqual(m1.max_load_factor(), m2.max_load_factor());
        util.assertEqual(m1.load_factor() <= m1.max_load_factor(), true);
        m1.rehash(200);
        m2.rehash(200);
        util.assertEqual(m1.bucket_count() >= 200, m2.bucket_count() >= 200);
        m1.max_load_factor(0.8);
        m2.max_load_factor(0.8);
        util.assertEqual(m1.max_load_factor(), m2.max_load_factor());
        m1.rehash(300);
        m2.rehash(300);
        util.assertEqual(m1.bucket_count() >= 300, m2.bucket_count() >= 300);
        m1.reserve(400);
        m2.reserve(400);
        util.assertEqual(m1.bucket_count() >= 400, m2.bucket_count() >= 400);
    }
    // observers
    {
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_map<int, std::string> m2(vec.begin(), vec.end());
        util.assertEqual(typeid(m1.hash_function()) == typeid(std::hash<int>), true);
        util.assertEqual(typeid(m2.hash_function()) == typeid(std::hash<int>), true);
        util.assertEqual(typeid(m1.key_eq()) == typeid(std::equal_to<int>), true);
        util.assertEqual(typeid(m2.key_eq()) == typeid(std::equal_to<int>), true);
    }
    // comparisons
    {
        // comparisons
        tstd::unordered_map<int, std::string> m1(vec.begin(), vec.end());
        tstd::unordered_map<int, std::string> m2(vec.begin(), vec.end());
        util.assertEqual(m1 == m2, true);
        util.assertEqual(m1 != m2, false);
        m1.erase(10);
        util.assertEqual(m1 == m2, false);
        util.assertEqual(m1 != m2, true);
    }
    util.showFinalResult();
}

void testUnorderedMultimap(bool showDetails)
{
    TestUtil util(showDetails, "unordered_multimap");
    std::vector<std::pair<const int, std::string>> vec;
    {
        std::vector<int> tmp(100);
        std::iota(tmp.begin(), tmp.end(), 1);
        std::random_shuffle(tmp.begin(), tmp.end());
        for (auto& elem : tmp)
        {
            vec.emplace_back(elem, std::to_string(elem));
        }
        for (int i = 0; i < 100; ++i)
        {
            vec.emplace_back(vec[i]);
        }
        for (int i = 0; i < 200; ++i)
        {
            vec.emplace_back(vec[i]);
        }
        // repeat every element for 4 times
    }
    // constructors
    {
        // 1
        tstd::unordered_multimap<int, std::string> m1;
        std::unordered_multimap<int, std::string> m2;
        util.assertSetEqual(m1, m2);
    }
    {
        // 2
        tstd::unordered_multimap<int, std::string> m1(20, std::hash<int>(), std::equal_to<int>(), tstd::allocator<int>());
        std::unordered_multimap<int, std::string> m2(20, std::hash<int>(), std::equal_to<int>(), std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    {
        // 3
        tstd::unordered_multimap<int, std::string> m1(20, tstd::allocator<int>());
        std::unordered_multimap<int, std::string> m2(20, std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    {
        // 4
        tstd::unordered_multimap<int, std::string> m1(20, std::hash<int>(), tstd::allocator<int>());
        std::unordered_multimap<int, std::string> m2(20, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    {
        // 5
        tstd::unordered_multimap<int, std::string> m1((tstd::allocator<int>()));
        std::unordered_multimap<int, std::string> m2((std::allocator<int>()));
        util.assertSetEqual(m1, m2);
    }
    {
        // 6
        tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end());
        util.assertSetEqual(m1, m2);
    }
    {
        // 7
        tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end(), 100, tstd::allocator<int>());
        std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end(), 100, std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    {
        // 8
        tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end(), 100, std::hash<int>(), tstd::allocator<int>());
        std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end(), 100, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(m1, m2);
        {
            // 9
            tstd::unordered_multimap<int, std::string> m1c(m1);
            std::unordered_multimap<int, std::string> m2c(m2);
            util.assertSetEqual(m1c, m2c);
        }
        {
            // 10
            tstd::unordered_multimap<int, std::string> m1c(m1, tstd::allocator<int>());
            std::unordered_multimap<int, std::string> m2c(m2, std::allocator<int>());
            util.assertSetEqual(m1c, m2c);
        }
        // 11
        tstd::unordered_multimap<int, std::string> m1m(std::move(m1));
        std::unordered_multimap<int, std::string> s2m(std::move(m2));
        util.assertSetEqual(m1, m2);
        util.assertSetEqual(m1m, s2m);
        // 12
        tstd::unordered_multimap<int, std::string> s1mm(std::move(m1m), tstd::allocator<int>());
        std::unordered_multimap<int, std::string> s2mm(std::move(s2m), std::allocator<int>());
        util.assertSetEqual(m1m, s2m);
        util.assertSetEqual(s1mm, s2mm);
    }
    {
        // 13
        tstd::unordered_multimap<int, std::string> m1{{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}};
        std::unordered_multimap<int, std::string> m2{{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}};
        util.assertSetEqual(m1, m2);
    }
    {
        // 14
        tstd::unordered_multimap<int, std::string> m1({{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}}, 10, tstd::allocator<int>());
        std::unordered_multimap<int, std::string> m2({{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}}, 10, std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    {
        // 15
        tstd::unordered_multimap<int, std::string> m1({{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}}, 10, std::hash<int>(), tstd::allocator<int>());
        std::unordered_multimap<int, std::string> m2({{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}}, 10, std::hash<int>(), std::allocator<int>());
        util.assertSetEqual(m1, m2);
    }
    // assignment
    {
        tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end());
        tstd::unordered_multimap<int, std::string> m1c;
        std::unordered_multimap<int, std::string> m2c;
        // 1
        m1c = m1;
        m2c = m2;
        util.assertSetEqual(m1c, m2c);
        // 2
        m1c = std::move(m1);
        m2c = std::move(m2);
        util.assertSetEqual(m1c, m2c);
        util.assertSetEqual(m1, m2);
        // 3
        m1 = {{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}, {10, "10"}};
        m2 = {{2, "2"}, {1, "1"}, {3, "3"}, {4, "4"}, {4, "4"}, {10, "10"}};
        util.assertSetEqual(m1, m2);
    }
    // allocator
    {
        tstd::unordered_multimap<int, std::string> m1;
        util.assertEqual(m1.get_allocator() == tstd::allocator<std::pair<const int, std::string>>(), true);
    }
    // iterators
    {
        tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end());
        util.assertSetEqual(m1.begin(), m1.end(), m2.cbegin(), m2.cend());
        util.assertSetEqual(m1.cbegin(), m1.cend(), m2.begin(), m2.end());
        // const version
        {
            const tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end());
            const std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end());
            util.assertSetEqual(m1.begin(), m1.end(), m2.cbegin(), m2.cend());
            util.assertSetEqual(m1.cbegin(), m1.cend(), m2.begin(), m2.end());
        }
    }
    // size and capacity
    {
        tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end());
        util.assertEqual(m1.size(), m2.size());
        util.assertEqual(m1.empty(), m2.empty());
    }
    // modifiers
    {
        // clear
        tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end());
        m1.clear();
        m2.clear();
        util.assertSetEqual(m1, m2);
        // insert
        const std::pair<const int, std::string> ca(88, "88");
        std::pair<const int, std::string> a(99, "99");
        // 1
        auto iter1 = m1.insert(ca);
        auto iter2 = m2.insert(ca);
        util.assertEqual(*iter1, *iter2);
        // 2
        iter1 = m1.insert(std::pair<const int, std::string>(100, "100"));
        iter2 = m2.insert(std::pair<const int, std::string>(100, "100"));
        util.assertEqual(*iter1, *iter2);
        // 3
        iter1 = m1.insert(a);
        iter2 = m2.insert(a);
        util.assertEqual(*iter1, *iter2);
        // 4
        iter1 = m1.insert(m1.begin(), ca);
        iter2 = m2.insert(m2.begin(), ca);
        util.assertEqual(*iter1, *iter2);
        // 5
        iter1 = m1.insert(m1.begin(), std::pair<const int, std::string>(70, "70"));
        iter2 = m2.insert(m2.begin(), std::pair<const int, std::string>(70, "70"));
        util.assertEqual(*iter1, *iter2);
        // 6
        iter1 = m1.insert(m1.begin(), a);
        iter2 = m2.insert(m2.begin(), a);
        util.assertEqual(*iter1, *iter2);
        // 7
        m1.insert(vec.begin(), vec.end());
        m2.insert(vec.begin(), vec.end());
        util.assertSetEqual(m1, m2);
        // 8
        m1.insert({{1, "1"}, {2, "2"}, {3, "3"}, {3, "3"}, {100, "100"}, {1024, "1024"}, {100, "100"}, {99, "99"}, {30143, "30143"}});
        m2.insert({{1, "1"}, {2, "2"}, {3, "3"}, {3, "3"}, {100, "100"}, {1024, "1024"}, {100, "100"}, {99, "99"}, {30143, "30143"}});
        util.assertSetEqual(m1, m2);
        // emplace
        m1.emplace(-100, "-100");
        m2.emplace(-100, "-100");
        util.assertSetEqual(m1, m2);
        // emplace_hint
        m1.emplace_hint(m1.begin(), -1001, "-1001");
        m2.emplace_hint(m2.begin(), -1001, "-1001");
        util.assertSetEqual(m1, m2);
        // erase
        // 1
        iter1 = m1.erase(m1.find(15));
        iter2 = m2.erase(m2.find(15));
        util.assertSetEqual(m1, m2);
        // 2
        auto range1 = m1.equal_range(20);
        auto range2 = m2.equal_range(20);
        iter1 = m1.erase(range1.first, range1.second);
        iter2 = m2.erase(range2.first, range2.second);
        util.assertSetEqual(m1, m2);
        // 3
        auto count1 = m1.erase(50);
        auto count2 = m2.erase(50);
        util.assertEqual(count1, count2);
        util.assertSetEqual(m1, m2);
        // swap
        tstd::unordered_multimap<int, std::string> tmp1(vec.begin(), vec.end());
        std::unordered_multimap<int, std::string> tmp2(vec.begin(), vec.end());
        m1.swap(tmp1);
        m2.swap(tmp2);
        util.assertSetEqual(m1, m2);
        util.assertSetEqual(tmp1, tmp2);
        swap(m1, tmp1);
        swap(m2, tmp2);
        util.assertSetEqual(m1, m2);
        util.assertSetEqual(tmp1, tmp2);
        tstd::swap(m1, tmp1);
        std::swap(m2, tmp2);
        util.assertSetEqual(m1, m2);
        util.assertSetEqual(tmp1, tmp2);
    }
    {
        // lookup
        tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end());
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
        // const version
        {
            const tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end());
            const std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end());
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
        }
    }
    // template version of erase/count/find/contains/equal_range/lower_bound/upper_bound
    {
        std::vector<std::pair<const Foo, std::string>> vecfoo;
        for (int i = 0; i < 100; ++i)
        {
            vecfoo.emplace_back(Foo(i), "foo(" + std::to_string(i) + ")_0");
            vecfoo.emplace_back(Foo(i), "foo(" + std::to_string(i) + ")_1");
        }
        tstd::unordered_multimap<Foo, std::string> m1(vecfoo.begin(), vecfoo.end());
        std::unordered_multimap<Foo, std::string> m2(vecfoo.begin(), vecfoo.end());
        util.assertSetEqual(m1, m2);
        // erase
        auto size1 = m1.erase(10);
        // auto size2 = m2.erase(10); // this version is since C++ 23, so use another version for std::set
        auto size2 = m2.erase(Foo(10));
        util.assertEqual(size1, size2);
        util.assertSetEqual(m1, m2);
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
        // do not guarantee order, implementation dependent, not test anymore.
        // util.assertEqual(*iter1, *iter2);
        // contains
        util.assertEqual(m1.contains(50), m2.contains(50));
        util.assertEqual(m1.contains(500), m2.contains(500));
        // equal_range
        auto p1 = m1.equal_range(50);
        auto p2 = m2.equal_range(50);
        util.assertEqual(tstd::distance(p1.first, p1.second), tstd::distance(p2.first, p2.second));
        util.assertSetEqual(p1.first, p1.second, p2.first, p2.second);
        // const version
        {
            const tstd::unordered_multimap<Foo, std::string> m1(vecfoo.begin(), vecfoo.end());
            const std::unordered_multimap<Foo, std::string> m2(vecfoo.begin(), vecfoo.end());
            util.assertSetEqual(m1, m2);
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
            // do not guarantee order, implementation dependent, not test anymore.
            // util.assertEqual(*iter1, *iter2);
            // equal_range
            auto p1 = m1.equal_range(50);
            auto p2 = m2.equal_range(50);
            util.assertEqual(tstd::distance(p1.first, p1.second), tstd::distance(p2.first, p2.second));
            util.assertSetEqual(p1.first, p1.second, p2.first, p2.second);
        }
    }
    {
        // bucket interfaces
        tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end(), 400);
        std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end(), 400);
        // local iterators
        util.assertEqual(std::distance(m1.begin(1), m1.end(1)), std::distance(m2.begin(1), m2.end(1)));
        util.assertEqual(std::distance(m1.cbegin(10), m1.cend(10)), std::distance(m2.cbegin(10), m2.cend(10)));
        util.assertEqual(m1.bucket_count() >= 400, m2.bucket_count() >= 400);
        util.assertEqual(m1.bucket_size(1), m2.bucket_size(1));
        // hash policy
        util.assertEqual(m1.max_load_factor(), m2.max_load_factor());
        util.assertEqual(m1.load_factor() <= m1.max_load_factor(), true);
        m1.rehash(500);
        m2.rehash(500);
        util.assertEqual(m1.bucket_count() >= 500, m2.bucket_count() >= 500);
        m1.max_load_factor(0.8);
        m2.max_load_factor(0.8);
        util.assertEqual(m1.max_load_factor(), m2.max_load_factor());
        m1.rehash(600);
        m2.rehash(600);
        util.assertEqual(m1.bucket_count() >= 600, m2.bucket_count() >= 600);
        m1.reserve(800);
        m2.reserve(800);
        util.assertEqual(m1.bucket_count() >= 1000, m2.bucket_count() >= 1000);
    }
    // observers
    {
        tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end());
        std::unordered_multimap<int, std::string> m2(vec.begin(), vec.end());
        util.assertEqual(typeid(m1.hash_function()) == typeid(std::hash<int>), true);
        util.assertEqual(typeid(m2.hash_function()) == typeid(std::hash<int>), true);
        util.assertEqual(typeid(m1.key_eq()) == typeid(std::equal_to<int>), true);
        util.assertEqual(typeid(m2.key_eq()) == typeid(std::equal_to<int>), true);
    }
    // comparisons
    {
        // comparisons
        tstd::unordered_multimap<int, std::string> m1(vec.begin(), vec.end());
        tstd::unordered_multimap<int, std::string> m2(vec.begin(), vec.end());
        util.assertEqual(m1 == m2, true);
        util.assertEqual(m1 != m2, false);
        m1.erase(10);
        util.assertEqual(m1 == m2, false);
        util.assertEqual(m1 != m2, true);
    }
    util.showFinalResult();
}
