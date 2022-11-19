#include <ch19/vector.h>
#include <algorithm>

namespace ch19 {

template<typename T>
Vector<T>::Vector(std::size_t s)
    : sz{s}
    , elem{new T[s]}
    , space{s}
{
    for (int i = 0; i < sz; ++i) {
        elem[i] = 0.0;
    }
}

template<typename T>
Vector<T>::Vector(std::initializer_list<T> lst)
    : sz{lst.size()}
    , elem{new T[lst.size()]}
    , space{lst.size()}
{
    std::copy(lst.begin(), lst.end(), elem);
}

template<typename T>
Vector<T>::Vector(const Vector<T>& v)
    : sz{v.sz}
    , elem{new T[v.sz]}
    , space{v.space}
{
    std::copy(v.elem, v.elem + v.sz, elem);
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v)
{
    if (this == &v) return *this;   // self-assignment
    if (v.sz <= space) {
        std::copy(v.elem, v.elem + v.sz, elem);
        sz = v.sz;
    } else {
        auto p = new T[v.sz];
        std::copy(v.elem, v.elem + v.sz, p);
        delete[] elem;
        elem = p;
        space = sz = v.sz;
    }
    return *this;
}

template<typename T>
Vector<T>::Vector(Vector<T>&& v)
    : sz{v.sz}
    , elem{v.elem}
    , space{v.space}
{
    v.sz = 0;           // make v the empty vector
    v.elem = nullptr;
    v.space = 0;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& v)
{
    delete[] elem;      // throw away old elements
    elem = v.elem;      // steal v's elements
    space = sz = v.sz;
    v.sz = 0;           // make v the empty vector
    v.elem = nullptr;
    v.space = 0;
    return *this;
}

template<typename T>
void Vector<T>::resize(std::size_t new_size)
{
    reserve(new_size);
    for (auto i = sz; i < new_size; ++i) {
        elem[i] = 0;
    }
    sz = new_size;
}

template<typename T>
void Vector<T>::push_back(const T& d)
{
    if (space == 0) {
        reserve(8);
    } else if (sz == space) {
        reserve(2 * space);
    }
    elem[sz] = d;
    ++sz;
}

template<typename T>
void Vector<T>::reserve(std::size_t new_space)
{
    if (new_space <= space) return;
    auto p = new T[new_space];
    std::copy(elem, elem + new_space, p);
    delete[] elem;
    elem = p;
    space = new_space;
}

}
