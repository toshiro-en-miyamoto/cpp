#pragma once

#include <cstddef>
#include <initializer_list>

namespace ch19 {

/**
 * invariant:
 *  if 0 ≤ n < sz, elem[n] is element n
 *  sz ≤ space;
 *  if sz < space there is space for (space - sz) doubles after
 *  elem[sz - 1]
 */

template<typename T>
class Vector
{
    std::size_t sz;
    T* elem;
    std::size_t space;
public:
    Vector() : sz{0}, elem{nullptr}, space{0} {}
    ~Vector() { delete[] elem; }

    explicit Vector(std::size_t);
    Vector(std::initializer_list<T>);

    Vector(const Vector&);
    Vector& operator=(const Vector&);

    Vector(Vector&&);
    Vector& operator=(Vector&&);

    T& operator[](std::size_t i) { return elem[i]; }
    const T& operator[](std::size_t i) const { return elem[i]; }

    std::size_t size() const { return sz; }
    std::size_t capacity() const { return space; }

    void resize(std::size_t);
    void push_back(const T&);
    void reserve(std::size_t);
};

class vector
{
    std::size_t sz;
    double* elem;
    std::size_t space;
public:
    vector() : sz{0}, elem{nullptr}, space{0} {}
    ~vector() { delete[] elem; }

    explicit vector(std::size_t);
    vector(std::initializer_list<double>);

    vector(const vector&);
    vector& operator=(const vector&);

    vector(vector&&);
    vector& operator=(vector&&);

    double& operator[](std::size_t i) { return elem[i]; }
    // const double& operator[](std::size_t i) const { return elem[i]; }
    double operator[](std::size_t i) const { return elem[i]; }

    std::size_t size() const { return sz; }
    std::size_t capacity() const { return space; }

    void resize(std::size_t);
    void push_back(double);
    void reserve(std::size_t);
};

}
