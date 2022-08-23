#include <iostream>
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
    return 0;
}

void testUnorderedSet(bool showDetails)
{
    TestUtil util(showDetails, "unordered_set");
    {
        tstd::unordered_set<int> s;
    }
    util.showFinalResult();
}

void testUnorderedMultiset(bool showDetails)
{
    TestUtil util(showDetails, "unordered_map");
    {
        tstd::unordered_multiset<int> s;
    }
    util.showFinalResult();
}

void testUnorderedMap(bool showDetails)
{
    TestUtil util(showDetails, "unordered_multiset");
    {
        tstd::unordered_map<int, int> m;
    }
    util.showFinalResult();
}

void testUnorderedMultimap(bool showDetails)
{
    TestUtil util(showDetails, "unordered_multimap");
    {
        tstd::unordered_multimap<int, int> m;
    }
    util.showFinalResult();
}

