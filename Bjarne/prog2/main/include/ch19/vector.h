#pragma once

#include <cstddef>

namespace ch19 {

class vector
{
    std::size_t sz;
    double* elem;
    std::size_t space;
public:
    vector();
    void reserve(std::size_t);
    std::size_t capacity() const { return space; }
    void resize(std::size_t);
};

}
