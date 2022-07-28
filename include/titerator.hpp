#ifndef IITERATOR_HPP
#define IITERATOR_HPP

#include <cstddef>

namespace tstd
{

// deprecated in C++17
template<
    typename Category,
    typename T,
    typename Distance = std::ptrdiff_t,
    typename Pointer = T*,
    typename Reference = T&>
class iterator
{
public:
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};

} // namespace tstd


#endif // IITERATOR_HPP