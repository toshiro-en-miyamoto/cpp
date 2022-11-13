#include <ch19/vector.h>

ch19::vector::vector()
    : sz{0}
    , elem{nullptr}
    , space{0}
{}

void ch19::vector::reserve(std::size_t new_space)
{
    if (new_space <= space) return;
    double* p = new double[new_space];
    for (int i = 0; i < new_space; ++i) {
        p[i] = elem[i];
    }
    delete[] elem;
    elem = p;
    space = new_space;
}

void ch19::vector::resize(std::size_t new_size)
{
    reserve(new_size);
    for (auto i = sz; i < new_size; ++i) {
        elem[i] = 0;
    }
    sz = new_size;
}
