#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <random>
#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>
#include <typeinfo>
#include <string>
#include <tstl_hashtable.hpp>
#include <tutility.hpp>
#include <titerator.hpp>
#include "TestUtil.hpp"

// test of hash table implementation

void testHashtableImpl(bool showDetails);

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testHashtableImpl(showDetails);
    std::cout << std::endl;
    return 0;
}

class Foo
{
    friend class foo_hash;
    friend class foo_equal;
    friend std::ostream& operator<<(std::ostream& os, const Foo& foo)
    {
        os << foo.to_string();
        return os;
    }
public:
    Foo(double _x) : x(_x) {}
    std::string to_string() const
    {
        return "foo(" + std::to_string(x) + ")";
    }
    bool operator==(const Foo& rhs) const = default;
private:
    double x;
};

class foo_hash
{
public:
    std::size_t operator()(const Foo& foo) const
    {
        static std::hash<double> hasher;
        return hasher(foo.x);
    }
};

class foo_equal
{
public:
    bool operator()(const Foo& foo1, const Foo& foo2) const
    {
        return foo1.x == foo2.x;
    }
};

void testHashtableImpl(bool showDetails)
{
    TestUtil util(showDetails, "tstd::impl::hash_table");
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 1);
    std::random_shuffle(vec.begin(), vec.end());

    using ht_int = tstd::impl::hash_table<int, int, tstd::impl::identity>;
    // constructors
    {
        // 1
        ht_int ht(50);
        util.assertEqual(ht.empty(), true);
        util.assertEqual(ht.size(), 0);
        util.assertEqual(ht.bucket_count(), 50);
    }
    {
        // 2
        ht_int ht(vec.begin(), vec.end(), 20);
        util.assertEqual(ht.empty(), false);
        util.assertEqual(ht.size(), 100);
        util.assertEqual(ht.bucket_count() >= 100, true);
        util.assertEqual(ht.load_factor() <= ht.max_load_factor(), true);
        util.assertSetEqual(vec, ht);
    }
    {
        // 3 : copy constructor
        ht_int ht1(vec.begin(), vec.end(), 20);
        ht_int ht2(ht1);
        util.assertEqual(ht1 == ht2, true);
        util.assertSetEqual(ht1, ht2);
    }
    {
        // 4: move constructor
        ht_int ht1(vec.begin(), vec.end(), 100);
        ht_int ht2(std::move(ht1));
        util.assertEqual(ht2.size(), 100);
        util.assertEqual(ht2.load_factor() <= ht2.max_load_factor(), true);
        util.assertEqual(ht1.empty(), true);
        util.assertEqual(ht1.size(), 0);
        // 5: move constructor
        ht_int ht3(std::move(ht2), tstd::allocator<int>());
        util.assertEqual(ht3.size(), 100);
        util.assertEqual(ht3.load_factor() <= ht3.max_load_factor(), true);
        util.assertEqual(ht2.empty(), true);
        util.assertEqual(ht2.size(), 0);
    }
    // assignment
    {
        // 1. copy assignment
        ht_int ht1(vec.begin(), vec.end(), 100);
        ht_int ht2(10);
        ht2 = ht1;
        util.assertEqual(ht2.size(), 100);
        util.assertEqual(ht2.load_factor() <= ht2.max_load_factor(), true);
        util.assertEqual(ht1.empty(), false);
        util.assertEqual(ht1.size(), 100);
        util.assertEqual(ht1 == ht2, true);
        util.assertSetEqual(ht1, ht2);
        // 2. move assignment
        ht_int ht3(10);
        ht3 = std::move(ht1);
        util.assertEqual(ht1.empty(), true);
        util.assertEqual(ht1.size(), 0);
        util.assertEqual(ht2 == ht3, true);
        util.assertSetEqual(ht2, ht3);
        // 3. initializer_list
        ht1 = {1, 2, 3, 4, 1, 1, 1};
        util.assertEqual(ht1.empty(), false);
        util.assertEqual(ht1.size(), 4);
        util.assertSetEqual(ht1, std::vector<int>{1, 2, 3, 4});
    }
    // allocator
    {
        ht_int ht1(vec.begin(), vec.end(), 100);
        util.assertEqual(ht1.get_allocator() == tstd::allocator<int>(), true);
    }
    // iterators
    {
        ht_int ht1(vec.begin(), vec.end(), 100);
        util.assertSetEqual(vec, ht1);
        util.assertSetEqual(vec.begin(), vec.end(), ht1.begin(), ht1.end());
        util.assertSetEqual(vec.begin(), vec.end(), ht1.cbegin(), ht1.cend());
        // const version
        {
            const ht_int ht1(vec.begin(), vec.end(), 100);
            util.assertSetEqual(vec.begin(), vec.end(), ht1.begin(), ht1.end());
        }
    }
    // size and capacity
    {
        ht_int ht1(vec.begin(), vec.end(), 100);
        util.assertEqual(ht1.size(), 100);
        util.assertEqual(ht1.empty(), false);
    }
    // modifiers
    {
        ht_int ht(vec.begin(), vec.end(), 100);
        // clear
        ht.clear();
        util.assertEqual(ht.empty(), true);
        util.assertEqual(ht.size(), 0);
        // insert
        // 1
        for (int i = 1; i <= 50; ++i)
        {
            ht.insert(i);
        }
        // 2
        for (int i = 51; i <= 100; ++i)
        {
            ht.insert(std::move(i));
        }
        util.assertEqual(ht.size(), 100);
        util.assertSetEqual(ht, vec);
        // empalce
        ht.clear();
        for (int i = 1; i <= 100; ++i)
        {
            ht.emplace(i);
        }
        util.assertSetEqual(ht, vec);
        // erase
        std::list<int> tmplist(vec.begin(), vec.end());
        tmplist.remove(20);
        tmplist.remove(10);
        tmplist.remove(100);
        tmplist.remove(1);
        // 1
        ht.erase(ht.find(20));
        ht.insert(101);
        ht.erase(ht.find(101));
        // 2
        auto p = ht.equal_range(10);
        ht.erase(p.first, p.second);
        // 3
        auto count = ht.erase(100);
        util.assertEqual(count, 1);
        ht.insert(101);
        count = ht.erase(101);
        util.assertEqual(count, 1);
        count = ht.erase(1);
        util.assertEqual(count, 1);
        util.assertSetEqual(ht, tmplist);
        // swap
        ht_int htc(ht);
        ht_int ht2(vec.begin(), vec.end(), 100);
        ht_int ht2c(ht2);
        ht.swap(ht2);
        util.assertEqual(htc == ht2, true);
        util.assertEqual(ht2c == ht, true);
        util.assertSetEqual(htc, ht2);
        util.assertSetEqual(ht2c, ht);
        swap(ht, ht2);
        util.assertEqual(htc == ht, true);
        util.assertEqual(ht2c == ht2, true);
        util.assertSetEqual(htc, ht);
        util.assertSetEqual(ht2c, ht2);
    }
    // random batch modification test
    {
        ht_int ht(20);
        ht.max_load_factor(5.0); // create more collisions
        for (auto elem : vec)
        {
            ht.insert(elem);
        }
        std::list<int> tmplist(vec.begin(), vec.end());
        for (int i = 13; i < 26; ++i)
        {
            ht.erase(i);
            tmplist.remove(i);
        }
        for (int i = 51; i < 89; ++i)
        {
            ht.erase(ht.find(i));
            tmplist.remove(i);
        }
        for (int i = 67; i < 81; ++i)
        {
            ht.insert(i);
            tmplist.push_back(i);
        }
        util.assertSetEqual(ht, tmplist);
    }
    // lookup
    {
        // find
        ht_int ht(vec.begin(), vec.end(), 100);
        auto iter = ht.find(10);
        util.assertEqual(*iter, 10);
        // equal_range
        auto p = ht.equal_range(10);
        util.assertEqual(*p.first, 10);
        util.assertEqual(std::distance(p.first, p.second), 1);
        // const version
        {
            const ht_int ht(vec.begin(), vec.end(), 100);
            auto iter = ht.find(10);
            util.assertEqual(*iter, 10);
            // equal_range
            auto p = ht.equal_range(10);
            util.assertEqual(*p.first, 10);
            util.assertEqual(std::distance(p.first, p.second), 1);
        }
    }
    // bucket interfaces and hash policy
    // highly implementation defined, test according to it's implementation
    // may changed after modifying implementation
    {
        // bucket interfaces
        ht_int ht(20);
        ht.max_load_factor(5.0); // create more collisions
        for (auto elem : vec)
        {
            ht.insert(elem);
        }
        // local iterators
        util.assertEqual(std::distance(ht.begin(10), ht.end(10)), 5);
        util.assertEqual(std::distance(ht.cbegin(10), ht.cend(10)), 5);
        // const version
        {
            ht_int ht(20);
            ht.max_load_factor(5.0); // create more collisions
            for (auto elem : vec)
            {
                ht.insert(elem);
            }
            util.assertEqual(std::distance(ht.begin(10), ht.end(10)), 5);
            util.assertEqual(std::distance(ht.cbegin(10), ht.cend(10)), 5);
        }
        util.assertEqual(ht.bucket_count(), 20);
        util.assertEqual(ht.bucket_size(5), std::distance(ht.begin(5), ht.end(5)));
        util.assertEqual(ht.bucket_size(5), 5);
        util.assertEqual(ht.bucket(5), 5);
        // hash policy
        util.assertEqual(ht.max_load_factor(), 5.0);
        util.assertEqual(ht.load_factor() <= ht.max_load_factor(), true);
        ht.rehash(100);
        util.assertEqual(ht.bucket_count(), 100);
        ht.max_load_factor(0.5);
        ht.reserve(100);
        util.assertEqual(ht.bucket_count(), 200);
    }
    // observers
    {
        ht_int ht(vec.begin(), vec.end(), 100);
        util.assertEqual(typeid(ht.hash_function()) == typeid(std::hash<int>), true);
        util.assertEqual(typeid(ht.key_eq()) == typeid(ht_int::key_equal), true);
        util.assertEqual(ht.key_eq()(1, 1), true);
    }

    // other template arguments test
    // Multi/KeyOfValue/Hash/KeyEqual
    using ht_foo = tstd::impl::hash_table<Foo, std::pair<const Foo, std::string>,
        tstd::impl::first_of_pair<const Foo, std::string>, true, foo_hash, foo_equal,
        tstd::allocator<std::pair<const Foo, std::string>>>;
    std::vector<std::pair<const Foo, std::string>> vec_foo;
    {
        for (auto& elem : vec)
        {
            vec_foo.emplace_back(elem, Foo(elem).to_string() + "_0");
            vec_foo.emplace_back(elem, Foo(elem).to_string() + "_1");
            vec_foo.emplace_back(elem, Foo(elem).to_string() + "_2");
            vec_foo.emplace_back(elem, Foo(elem).to_string() + "_3");
        }
    }
    // constructors
    {
        // 1
        ht_foo ht(50);
        util.assertEqual(ht.empty(), true);
        util.assertEqual(ht.size(), 0);
        util.assertEqual(ht.bucket_count(), 50);
    }
    {
        // 2
        ht_foo ht(vec_foo.begin(), vec_foo.end(), 20);
        util.assertEqual(ht.empty(), false);
        util.assertEqual(ht.size(), vec_foo.size());
        util.assertEqual(ht.bucket_count() >= vec_foo.size(), true);
        util.assertEqual(ht.load_factor() <= ht.max_load_factor(), true);
        util.assertSetEqual(vec_foo, ht);
    }
    {
        // 3 : copy constructor
        ht_foo ht1(vec_foo.begin(), vec_foo.end(), 20);
        ht_foo ht2(ht1);
        util.assertEqual(ht1 == ht2, true);
        util.assertSetEqual(ht1, ht2);
    }
    {
        // 4: move constructor
        ht_foo ht1(vec_foo.begin(), vec_foo.end(), 100);
        ht_foo ht2(std::move(ht1));
        util.assertEqual(ht2.size(), vec_foo.size());
        util.assertEqual(ht2.load_factor() <= ht2.max_load_factor(), true);
        util.assertEqual(ht1.empty(), true);
        util.assertEqual(ht1.size(), 0);
        // 5: move constructor
        ht_foo ht3(std::move(ht2), tstd::allocator<std::pair<const Foo, std::string>>());
        util.assertEqual(ht3.size(), vec_foo.size());
        util.assertEqual(ht3.load_factor() <= ht3.max_load_factor(), true);
        util.assertEqual(ht2.empty(), true);
        util.assertEqual(ht2.size(), 0);
    }
    // assignment
    {
        // 1. copy assignment
        ht_foo ht1(vec_foo.begin(), vec_foo.end(), 100);
        ht_foo ht2(10);
        ht2 = ht1;
        util.assertEqual(ht2.size(), vec_foo.size());
        util.assertEqual(ht2.load_factor() <= ht2.max_load_factor(), true);
        util.assertEqual(ht1.empty(), false);
        util.assertEqual(ht1.size(), vec_foo.size());
        util.assertEqual(ht1 == ht2, true);
        util.assertSetEqual(ht1, ht2);
        // 2. move assignment
        ht_foo ht3(10);
        ht3 = std::move(ht1);
        util.assertEqual(ht1.empty(), true);
        util.assertEqual(ht1.size(), 0);
        util.assertEqual(ht2 == ht3, true);
        util.assertSetEqual(ht2, ht3);
        // 3. initializer_list
        ht1 = {{1, "foo(1)"}, {2, "foo(2)"}, {3, "foo(3)"}, {4, "foo(4)"}, {1, "foo(1)"}, {1, "foo(1)"}, {1, "foo(1)"}};
        util.assertEqual(ht1.empty(), false);
        util.assertEqual(ht1.size(), 7);
        std::vector<std::pair<const Foo, std::string>> tmp_vec{{1, "foo(1)"}, {2, "foo(2)"}, {3, "foo(3)"}, {4, "foo(4)"}, {1, "foo(1)"}, {1, "foo(1)"}, {1, "foo(1)"}};
        util.assertSetEqual(ht1, tmp_vec);
    }
    // allocator
    {
        ht_foo ht1(vec_foo.begin(), vec_foo.end(), 100);
        util.assertEqual(ht1.get_allocator() == tstd::allocator<std::pair<const Foo, std::string>>(), true);
    }
    // iterators
    {
        ht_foo ht1(vec_foo.begin(), vec_foo.end(), 100);
        util.assertSetEqual(vec_foo, ht1);
        util.assertSetEqual(vec_foo.begin(), vec_foo.end(), ht1.begin(), ht1.end());
        util.assertSetEqual(vec_foo.begin(), vec_foo.end(), ht1.cbegin(), ht1.cend());
        // const version
        {
            const ht_foo ht1(vec_foo.begin(), vec_foo.end(), 100);
            util.assertSetEqual(vec_foo.begin(), vec_foo.end(), ht1.begin(), ht1.end());
            util.assertSetEqual(vec_foo.begin(), vec_foo.end(), ht1.cbegin(), ht1.cend());
        }
    }
    // size and capacity
    {
        ht_foo ht1(vec_foo.begin(), vec_foo.end(), 100);
        util.assertEqual(ht1.size(), vec_foo.size());
        util.assertEqual(ht1.empty(), false);
    }
    // modifiers
    {
        ht_foo ht(vec_foo.begin(), vec_foo.end(), 100);
        // clear
        ht.clear();
        util.assertEqual(ht.empty(), true);
        util.assertEqual(ht.size(), 0);
        // insert
        // 1
        for (int i = 1; i <= 50; ++i)
        {
            auto p = std::make_pair(Foo(i), Foo(i).to_string() + "_0");
            ht.insert(p);
            p = std::make_pair(Foo(i), Foo(i).to_string() + "_1");
            ht.insert(p);
            p = std::make_pair(Foo(i), Foo(i).to_string() + "_2");
            ht.insert(p);
            p = std::make_pair(Foo(i), Foo(i).to_string() + "_3");
            ht.insert(p);
        }
        // 2
        for (int i = 51; i <= 100; ++i)
        {
            ht.insert(std::make_pair(Foo(i), Foo(i).to_string() + "_0"));
            ht.insert(std::make_pair(Foo(i), Foo(i).to_string() + "_1"));
            ht.insert(std::make_pair(Foo(i), Foo(i).to_string() + "_2"));
            ht.insert(std::make_pair(Foo(i), Foo(i).to_string() + "_3"));
        }
        util.assertEqual(ht.size(), vec_foo.size());
        util.assertSetEqual(ht, vec_foo);
        // empalce
        ht.clear();
        for (int i = 1; i <= 100; ++i)
        {
            ht.emplace(i, Foo(i).to_string() + "_0");
            ht.emplace(i, Foo(i).to_string() + "_1");
            ht.emplace(i, Foo(i).to_string() + "_2");
            ht.emplace(i, Foo(i).to_string() + "_3");
        }
        util.assertSetEqual(ht, vec_foo);
        // erase
        std::list<std::pair<const Foo, std::string>> tmplist(vec_foo.begin(), vec_foo.end());
        tmplist.remove(std::make_pair(Foo(20), Foo(20).to_string() + "_0"));
        tmplist.remove_if([](const std::pair<const Foo, std::string>& p) -> bool { return p.first == 10; });
        tmplist.remove_if([](const std::pair<const Foo, std::string>& p) -> bool { return p.first == 100; });
        tmplist.remove_if([](const std::pair<const Foo, std::string>& p) -> bool { return p.first == 1; });
        // 1
        ht.erase(ht.find(20));
        ht.insert(std::make_pair(Foo(101), "foo(101)_t"));
        ht.erase(ht.find(101));
        // 2
        auto p = ht.equal_range(10);
        ht.erase(p.first, p.second);
        // 3
        auto count = ht.erase(100);
        util.assertEqual(count, 4);
        ht.insert(std::make_pair(Foo(101), "foo(101)_t"));
        count = ht.erase(101);
        util.assertEqual(count, 1);
        count = ht.erase(1);
        util.assertEqual(count, 4);
        util.assertEqual(ht.size(), tmplist.size());
        util.assertSetEqual(ht, tmplist);
        // swap
        ht_foo htc(ht);
        ht_foo ht2(vec_foo.begin(), vec_foo.end(), 100);
        ht_foo ht2c(ht2);
        ht.swap(ht2);
        util.assertEqual(htc == ht2, true);
        util.assertEqual(ht2c == ht, true);
        util.assertSetEqual(htc, ht2);
        util.assertSetEqual(ht2c, ht);
        swap(ht, ht2);
        util.assertEqual(htc == ht, true);
        util.assertEqual(ht2c == ht2, true);
        util.assertSetEqual(htc, ht);
        util.assertSetEqual(ht2c, ht2);
    }
    // random batch modification test
    {
        ht_foo ht(20);
        ht.max_load_factor(5.0); // create more collisions
        for (auto elem : vec_foo)
        {
            ht.insert(elem);
        }
        std::list<std::pair<const Foo, std::string>> tmplist(vec_foo.begin(), vec_foo.end());
        for (int i = 13; i < 26; ++i)
        {
            ht.erase(Foo(i)); // erase the first one
            tmplist.remove_if([i](const std::pair<const Foo, std::string>& p) -> bool { return p.first == i; });
        }
        for (int i = 51; i < 89; ++i)
        {
            ht.erase(ht.find(i)); // erase the found first one(aka the one with _0 posfix)
            tmplist.remove(std::make_pair(Foo(i), Foo(i).to_string() + "_0"));
        }
        for (int i = 67; i < 81; ++i)
        {
            ht.emplace(i, std::to_string(i) + "_emplace");
            tmplist.emplace_back(i, std::to_string(i) + "_emplace");
        }
        util.assertSetEqual(ht, tmplist);
    }
    // lookup
    {
        // find
        ht_foo ht(vec_foo.begin(), vec_foo.end(), 100);
        auto iter = ht.find(10);
        util.assertEqual(iter->first, Foo(10));
        // equal_range
        auto p = ht.equal_range(10);
        util.assertEqual(p.first->first, 10);
        util.assertEqual(std::distance(p.first, p.second), 4);
        // const version
        {
            const ht_foo ht(vec_foo.begin(), vec_foo.end(), 100);
            auto iter = ht.find(10);
             util.assertEqual(iter->first, Foo(10));
            // equal_range
            auto p = ht.equal_range(10);
            util.assertEqual(p.first->first, 10);
            util.assertEqual(std::distance(p.first, p.second), 4);
        }
    }
    // bucket interfaces and hash policy
    // highly implementation defined, test according to it's implementation
    // may changed after modifying implementation
    {
        // bucket interfaces
        ht_foo ht(80);
        ht.max_load_factor(5.0); // create more collisions
        for (auto elem : vec_foo)
        {
            ht.insert(elem);
        }
        util.assertEqual(ht.bucket_count(), 80);
        util.assertEqual(ht.bucket_size(5), std::distance(ht.begin(5), ht.end(5)));
        // hash policy
        util.assertEqual(ht.max_load_factor(), 5.0);
        util.assertEqual(ht.load_factor() <= ht.max_load_factor(), true);
        ht.rehash(100);
        util.assertEqual(ht.bucket_count(), 100);
        ht.max_load_factor(0.5);
        ht.reserve(500);
        util.assertEqual(ht.bucket_count(), 1000);
    }
    // observers
    {
        ht_foo ht(vec_foo.begin(), vec_foo.end(), 100);
        util.assertEqual(typeid(ht.hash_function()) == typeid(foo_hash), true);
        util.assertEqual(typeid(ht.key_eq()) == typeid(foo_equal), true);
        util.assertEqual(ht.key_eq()(Foo(1.0), Foo(1.0)), true);
    }
    util.showFinalResult();
}
