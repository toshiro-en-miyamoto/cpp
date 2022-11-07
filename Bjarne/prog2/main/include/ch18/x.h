#pragma once

#include <string>
#include <iostream>

namespace my {

struct X
{
    int val;
    void out(const std::string& s, int nv)
    { std::cout << s << ":" << val << "(" << nv << ")\n"; }

    // default constructor
    X() { out("X()", 0); val = 0; }

    // a single argument constructor
    X(int v) { val = v; out("X(int)", v); }

    // copy constructor
    X(const X& x) {val = x.val; out("X(X&)", x.val); }

    // copy assignment
    X& operator=(const X& a)
    { out("X::operator=()", a.val); val = a.val; return *this; }

    // destructor
    ~X() { out("~X()", 0); }
};

}
