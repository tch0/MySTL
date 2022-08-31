#ifndef TESTUTIL_HPP
#define TESTUTIL_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <source_location> // since C++20
#include <algorithm>
#include <iterator>
#include <utility>

// parsing first argument: -d to show details
bool parseDetailFlag(int argc, char const *argv[])
{
    return argc >= 2 && std::string(argv[1]) == "-d";
}

// operator << for pair
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p)
{
    os << "(" << p.first << ", " << p.second << ")";
    return os;
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
    TestUtil(bool _show, const std::string& _target, int _lineNumberWidth = 4, int _maxSequenceLength = 20) 
        : passedCount(0)
        , totalCount(0)
        , showDetails(_show)
        , target(_target)
        , lineNumberWidth(_lineNumberWidth)
        , maxSequenceLength(_maxSequenceLength)
    {
        if (showDetails)
        {
            std::cout << "Test of " << target << ": " << std::endl;
        }
    }

    void showFinalResult()
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
        bool res = std::equal(c1.begin(), c1.end(), c2.begin());
        passedCount += (res ? 1 : 0);
        totalCount++;
        if (showDetails)
        {
            std::cout << std::boolalpha << std::dec;
            std::cout << loc.file_name() << ":" << std::setw(lineNumberWidth) << loc.line() << ": "
                << "assertSequenceEqual: " << (res ? "passed" : "==================== failed")
                << "\n\tleft sequence: " << printContainerElememts(c1, maxSequenceLength)
                << "\n\tright sequence: " << printContainerElememts(c2, maxSequenceLength) << std::endl;
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
                << "\n\tleft array: " << printArrayElements(arr1, size, maxSequenceLength)
                << "\n\tright array: " << printArrayElements(arr2, size, maxSequenceLength) << std::endl;
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
                << "\n\tleft range: " << PrintSequenceElements(b1, e1, maxSequenceLength)
                << "\n\tright range: " << PrintSequenceElements(b2, std::next(b2, std::distance(b1, e1)), maxSequenceLength)  << std::endl;
        }
    }
    template<typename ForwardIterator1, typename ForwardIterator2>
    void assertRangeEqual(ForwardIterator1 b1, ForwardIterator1 e1, ForwardIterator2 b2, ForwardIterator2 e2, const std::source_location& loc = std::source_location::current())
    {
        bool res = std::distance(b1, e1) == std::distance(b2, e2) && std::equal(b1, e1, b2);
        passedCount += (res ? 1 : 0);
        totalCount++;
        if (showDetails)
        {
            std::cout << std::boolalpha << std::dec;
            std::cout << loc.file_name() << ":" << std::setw(lineNumberWidth) << loc.line() << ": "
                << "assertRangeEqual: " << (res ? "passed" : "==================== failed")
                << "\n\tleft range: " << PrintSequenceElements(b1, e1, maxSequenceLength)
                << "\n\tright range: " << PrintSequenceElements(b2, e2, maxSequenceLength)  << std::endl;
        }
    }
    // assert a sequence is sorted
    template<typename InputIterator, typename Compare = std::less<typename std::iterator_traits<InputIterator>::value_type>>
    void assertSorted(InputIterator b, InputIterator e, const Compare& cmp = Compare(), const std::source_location& loc = std::source_location::current())
    {
        bool res = std::is_sorted(b, e, cmp);
        passedCount += (res ? 1 : 0);
        totalCount++;
        if (showDetails)
        {
            std::cout << std::boolalpha << std::dec;
            std::cout << loc.file_name() << ":" << std::setw(lineNumberWidth) << loc.line() << ": "
                << "assertSorted: " << (res ? "passed" : "==================== failed")
                << "\n\tsequence: " << PrintSequenceElements(b, e, maxSequenceLength) << std::endl;
        }
    }
    // assert two set is equal, do not consider order of elements.
    template<typename Container1, typename Container2>
    void assertSetEqual(const Container1& c1, const Container2& c2, const std::source_location& loc = std::source_location::current())
    {
        bool res = (std::size(c1) == std::size(c2) && std::is_permutation(c1.begin(), c1.end(), c2.begin()));
        passedCount += (res ? 1 : 0);
        totalCount++;
        if (showDetails)
        {
            std::cout << std::boolalpha << std::dec;
            std::cout << loc.file_name() << ":" << std::setw(lineNumberWidth) << loc.line() << ": "
                << "assertSetEqual: " << (res ? "passed" : "==================== failed")
                << "\n\tleft set: " << printContainerElememts(c1, maxSequenceLength)
                << "\n\tright set: " << printContainerElememts(c2, maxSequenceLength) << std::endl;
        }
    }
    template<typename ForwardIterator1, typename ForwardIterator2>
    void assertSetEqual(ForwardIterator1 b1, ForwardIterator1 e1, ForwardIterator2 b2, ForwardIterator2 e2, const std::source_location& loc = std::source_location::current())
    {
        bool res = (std::distance(b1, e1) == std::distance(b2, e2) && std::is_permutation(b1, e1, b2));
        passedCount += (res ? 1 : 0);
        totalCount++;
        if (showDetails)
        {
            std::cout << std::boolalpha << std::dec;
            std::cout << loc.file_name() << ":" << std::setw(lineNumberWidth) << loc.line() << ": "
                << "assertSetEqual: " << (res ? "passed" : "==================== failed")
                << "\n\tleft set: " << PrintSequenceElements(b1, e1, maxSequenceLength)
                << "\n\tright set: " << PrintSequenceElements(b2, e2, maxSequenceLength)  << std::endl;
        }
    }
private:
    int passedCount;
    int totalCount;
    int lineNumberWidth; // output width of line number
    int maxSequenceLength; // max output length of a sequence
    bool showDetails;
    std::string target;
};

#endif