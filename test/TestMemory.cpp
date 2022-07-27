#include <iostream>
#include <vector>
#include <unordered_map>
#include <tmemory.hpp>
#include "TestUtil.hpp"
using namespace std;

class Foo
{
public:
    static int count;
    Foo() { ++count; }
    Foo(int, int, int) { ++count; }
    Foo(double) { ++count; }
    Foo(const string&) { ++count; }
    Foo(const Foo&) { ++count; }
    ~Foo() { --count; }
};

int Foo::count = 0;

void testAllocator(bool showDetails)
{
    if (showDetails)
    {
        cout << "Test of allocator: " << endl;
    }
    TestUtil util(showDetails, "allocator");

    // allocation
    vector<int, tstd::allocator<int>> vec{1, 2, 3, 4, 5};
    vector<int> vec2{1, 2, 3, 4, 5};
    util.assertSequenceEqual(vec, vec2);

    // construction and destruction
    {
        vector<Foo, tstd::allocator<Foo>> vec;
        vec.emplace_back();
        vec.emplace_back(1, 2, 3);
        vec.emplace_back(2.0);
        vec.emplace_back("hello");
        vec.emplace_back(Foo());
        vec.pop_back();
        vec.pop_back();
        util.assertEqual(Foo::count, 3);
    }
    util.assertEqual(Foo::count, 0);
    
    // equal
    tstd::allocator<int> alloc;
    util.assertEqual(alloc == tstd::allocator<Foo>(), true);
    util.assertNotEqual(alloc != tstd::allocator<Foo>(), true);

    // final result
    util.showFinalResult();
    cout << endl;
}

int main(int argc, char const *argv[])
{
    bool showDetails = parseDetailFlag(argc, argv);
    testAllocator(showDetails);
    return 0;
}