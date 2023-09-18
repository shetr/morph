
#ifndef MORPH_BOUNDS_HPP
#define MORPH_BOUNDS_HPP

#include "Types.hpp"

namespace Morph {

template<typename T, i64 NUMBER>
class not_number
{
private:
    T m_value;
public:
    not_number(T value) : m_value(value) {
        assert(value != NUMBER);
    }
    T get() const { return m_value; }
};

template<typename T>
using not_zero = not_number<T, 0>;

}

#endif // MORPH_BOUNDS_HPP