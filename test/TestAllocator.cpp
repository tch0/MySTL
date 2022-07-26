#include <iostream>
#include <tallocator.hpp>
#include <vector>
#include "TestUtil.hpp"
using namespace std;

class Foo
{
public:
    static int count;
    Foo() { ++count; }
    Foo(const Foo&) { ++count; }
    ~Foo() { --count; }
};

int Foo::count = 0;

int main(int argc, char const *argv[])
{
    TestUtil util(parseDetailFlag(argc, argv));

    // allocation
    vector<int, tstd::allocator<int>> vec{1, 2, 3, 4, 5};
    vector<int> vec2{1, 2, 3, 4, 5};
    util.assertSequenceEqual(vec, vec2);

    // construction and destruction
    {
        vector<Foo, tstd::allocator<Foo>> vec(10);
    }
    util.assertEqual(Foo::count, 0);
    
    // equal
    tstd::allocator<int> alloc;
    util.assertEqual(alloc == tstd::allocator<Foo>(), true);
    util.assertEqual(alloc != tstd::allocator<Foo>(), false);

    // final result
    util.showFinalResult();
    cout << endl;
    return 0;
}