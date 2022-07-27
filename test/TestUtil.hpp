#ifndef TESTUTIL_HPP
#define TESTUTIL_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <source_location> // since C++20
#include <algorithm>

// parsing first argument: -d to show details
bool parseDetailFlag(int argc, char const *argv[])
{
    return argc >= 2 && std::string(argv[1]) == "-d";
}

// manipulator for printing first N elements of container
template<typename Container>
class PrintNElements
{
    friend std::ostream& operator<<(std::ostream& os, const PrintNElements& pn)
    {
        int i = 0;
        for (auto iter = pn.c.begin(); i < pn.num; ++i, ++iter)
        {
            os << *iter << " ";
        }
        if (pn.num < pn.c.size())
        {
            os << "...";
        }
        return os;
    }
public:
    PrintNElements(const Container& _c, std::size_t _num) : c(_c), num(_num)
    {
        if (c.size() < num)
        {
            num = c.size();
        }
    }
private:
    std::size_t num;
    const Container& c;
};

// test utilities
class TestUtil
{
public:
    TestUtil(bool _show, const std::string& _target) : passedCount(0), totalCount(0), showDetails(_show), target(_target) {}

    void showFinalResult(const std::source_location& loc = std::source_location::current())
    {
        std::cout << "Test result of " << target << ": "
            << passedCount << "/" << totalCount << " passed"
            << (passedCount == totalCount ? "" : " --------------------------> failed") << std::endl;
    }

    template<typename T1, typename T2>
    void assertEqual(const T1& t1, const T2& t2, const std::source_location& loc = std::source_location::current())
    {
        bool res = (t1 == t2);
        passedCount += (res ? 1 : 0);
        totalCount++;
        if (showDetails)
        {
            std::cout << std::boolalpha;
            std::cout << loc.file_name() << ":" << loc.line() << ": "
                << "assertEqual: " << "left value( " << t1 << " ), right value( " << t2 << " ) : "
                << (res ? "passed" : "==================== failed") << std::endl;
        }
    }

    template<typename T1, typename T2>
    void assertNotEqual(const T1& t1, const T2& t2, const std::source_location& loc = std::source_location::current())
    {
        bool res = (t1 != t2);
        passedCount += (res ? 1 : 0);
        totalCount++;
        if (showDetails)
        {
            std::cout << std::boolalpha;
            std::cout << loc.file_name() << ":" << loc.line() << ": "
                << "assertNotEqual: " << "left value( " << t1 << " ), right value( " << t2 << " ) : "
                << (res ? "passed" : "==================== failed") << std::endl;
        }
    }

    template<typename Container1, typename Container2>
    void assertSequenceEqual(const Container1& c1, const Container2& c2, const std::source_location& loc = std::source_location::current())
    {
        bool res = (c1.size() == c2.size());
        if (res)
        {
            res = std::equal(c1.begin(), c1.end(), c2.begin());
        }
        passedCount += (res ? 1 : 0);
        totalCount++;
        if (showDetails)
        {
            std::cout << loc.file_name() << ":" << loc.line() << ": "
                << "assertSequenceEqual: "
                << "\n\tleft value: " << PrintNElements(c1, 20) // class template argument deducing
                << "\n\tright value: " << PrintNElements(c2, 20) << "\n\t"
                << (res ? "passed" : "==================== failed") << std::endl;
        }
    }
private:
    int passedCount;
    int totalCount;
    bool showDetails;
    std::string target;
};

#endif