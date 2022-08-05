#ifndef TESTUTIL_HPP
#define TESTUTIL_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <source_location> // since C++20
#include <algorithm>
#include <iterator>

// parsing first argument: -d to show details
bool parseDetailFlag(int argc, char const *argv[])
{
    return argc >= 2 && std::string(argv[1]) == "-d";
}

// manipulator for printing first N elements of a sequence
template<typename Iterator>
class PrintSequenceElements
{
    friend std::ostream& operator<<(std::ostream& os, const PrintSequenceElements& p)
    {
        int count = 0;
        auto iter = p.begin;
        for (; iter != p.end && count < p.num; ++count, ++iter)
        {
            os << *iter << " ";
        }
        if (iter != p.end)
        {
            os << "...";
        }
        return os;
    }
public:
    PrintSequenceElements(const Iterator& _begin, const Iterator& _end, std::size_t _num) : begin(_begin), end(_end), num(_num)
    {
    }
private:
    std::size_t num;
    const Iterator begin;
    const Iterator end;
};

template<typename Container>
PrintSequenceElements<typename Container::const_iterator> printContainerElememts(const Container& c, std::size_t num)
{
    return PrintSequenceElements<typename Container::const_iterator>(c.begin(), c.end(), num);
}

template<typename T>
PrintSequenceElements<T*> printArrayElements(T* arr, std::size_t size, std::size_t num)
{
    return PrintSequenceElements<T*>(arr, arr+size, num);
}

// test utilities
class TestUtil
{
public:
    TestUtil(bool _show, const std::string& _target, int _lineNumberWidth = 4) 
        : passedCount(0)
        , totalCount(0)
        , showDetails(_show)
        , target(_target)
        , lineNumberWidth(_lineNumberWidth)
    {
        if (showDetails)
        {
            std::cout << "Test of " << target << ": " << std::endl;
        }
    }

    void showFinalResult(const std::source_location& loc = std::source_location::current())
    {
        std::cout << std::boolalpha << std::dec;
        std::cout << "Test result of " << target << ": "
            << passedCount << "/" << totalCount << " passed"
            << (passedCount == totalCount ? "" : " --------------------------> failed") << std::endl;
        if (showDetails)
        {
            std::cout << std::endl;
        }
    }

    template<typename T1, typename T2>
    void assertEqual(const T1& t1, const T2& t2, const std::source_location& loc = std::source_location::current())
    {
        bool res = (t1 == t2);
        passedCount += (res ? 1 : 0);
        totalCount++;
        if (showDetails)
        {
            std::cout << std::boolalpha << std::dec;
            std::cout << loc.file_name() << ":" << std::setw(lineNumberWidth) << loc.line() << ": "
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
            std::cout << std::boolalpha << std::dec;
            std::cout << loc.file_name() << ":" << std::setw(lineNumberWidth) << loc.line() << ": "
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
            std::cout << std::boolalpha << std::dec;
            std::cout << loc.file_name() << ":" << std::setw(lineNumberWidth) << loc.line() << ": "
                << "assertSequenceEqual: " << (res ? "passed" : "==================== failed")
                << "\n\tleft value: " << printContainerElememts(c1, 20) // class template argument deducing
                << "\n\tright value: " << printContainerElememts(c2, 20) << std::endl;
        }
    }

    template<typename T1, typename T2>
    void assertArrayEqual(const T1* arr1, const T2* arr2, std::size_t size, const std::source_location& loc = std::source_location::current())
    {
        bool res = std::equal(arr1, arr1 + size, arr2);
        passedCount += (res ? 1 : 0);
        totalCount++;
        if (showDetails)
        {
            std::cout << std::boolalpha << std::dec;
            std::cout << loc.file_name() << ":" << std::setw(lineNumberWidth) << loc.line() << ": "
                << "assertArrayEqual: " << (res ? "passed" : "==================== failed")
                << "\n\tleft value: " << printArrayElements(arr1, size, 20)
                << "\n\tright value: " << printArrayElements(arr2, size, 20) << std::endl;
        }
    }

    // more generic version of assert sequence/array equal
    template<typename ForwardIterator1, typename ForwardIterator2>
    void assertRangeEqual(ForwardIterator1 b1, ForwardIterator1 e1, ForwardIterator2 b2, const std::source_location& loc = std::source_location::current())
    {
        bool res = std::equal(b1, e1, b2);
        passedCount += (res ? 1 : 0);
        totalCount++;
        if (showDetails)
        {
            std::cout << std::boolalpha << std::dec;
            std::cout << loc.file_name() << ":" << std::setw(lineNumberWidth) << loc.line() << ": "
                << "assertRangeEqual: " << (res ? "passed" : "==================== failed")
                << "\n\tleft value: " << PrintSequenceElements(b1, e1, 20)
                << "\n\tright value: " << PrintSequenceElements(b2, b2 + std::distance(b1, e1), 20)  << std::endl;
        }
    }
private:
    int passedCount;
    int totalCount;
    int lineNumberWidth;
    bool showDetails;
    std::string target;
};

#endif