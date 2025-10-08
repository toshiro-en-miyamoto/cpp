# Rvalue

References:
- [Stroustrup 2013] Stroustrup. The C++ programming language, 4th edition.
- [C++ reference]

## References

Like a pointer, a *reference* is an alias for an object, is usually implemented to hold a machine address of an object, and does not impose performance overhead compared to pointers, but it differs from a pointer in that:

- You access a reference with exactly the same syntax as the name of an object.
- A reference always refers to the object to which it was initialized.
- There is no **null reference**, and we may assume that a reference refers to an object.

The main use of references is for specifying arguments and return values for functions in general and for overloaded operators in particular.

(Stroustrup 2013: §7.7. References)

## Value categories

Each expression belongs to exactly one of the three primary value categories: prvalue, xvalue, and lvalue. ([C++ reference: value categories](https://www.cppreference.com/w/cpp/language/value_category.html))

Function call expressions ([C++ reference: value categories](https://www.cppreference.com/w/cpp/language/value_category.html))

- whose return type is non-reference, are prvalue expressions
- whose return type is rvalue reference, are xvalue expressions
- whose return type is lvalue reference, are lvalue expressions

## Binding

An rvalue reference can bind to an rvalue, but not to an lvalue. (Stroustrup 2013: §7.7.2. Rvalue References)

```c++
struct Point {
  int x, y, z;
};
#include <cstddef>
#include <utility>
Point expand(Point&& p) {
  p.x++;
  p.y++;
  p.z++;
  return p;
}
int main() {
  Point a{1, 2, 3};
  // an rvalue reference cannot bind to an lvalue
  // Point&& p cannot bind to Point a (a is an lvalue)
  // auto b = expand(a); --- compile error
  auto b = expand(std::move(a));
}
```

`std::move` produces an xvalue expression that identifies its argument t. It is exactly equivalent to a `static_cast` to an rvalue reference type. ([C++ reference: move](https://www.cppreference.com/w/cpp/utility/move.html))

An rvalue reference refers to a temporary object, which the user of the reference can modify, assuming that the object will never be used again. (Stroustrup 2013: §7.7.2. Rvalue References)

```c++
Point expand(Point&& p) {
//  pushq %rbp
//  movq %rsp, %rbp
//  movq %rdi, -24(%rbp)    # Point&& p
  p.x++;
//  movq -24(%rbp), %rax
//  movl (%rax), %ecx       # %ecx ⬅︎ p.x
//  incl %ecx
//  movl %ecx, (%rax)       # p.x ⬅︎ p.x + 1
  p.y++;
//  movq -24(%rbp), %rax
//  movl 4(%rax), %ecx      # %ecx ⬅︎ p.y
//  incl %ecx
//  movl %ecx, 4(%rax)      # p.y ⬅︎ p.y + 1
  p.z++;
//  movq -24(%rbp), %rax
//  movl 8(%rax), %ecx      # %ecx ⬅︎ p.z
//  incl %ecx
//  movl %ecx, 8(%rax)      # p.z ⬅︎ p.z + 1
  return p;
//  movq -24(%rbp), %rax
//  movl 8(%rax), %ecx
//  movl %ecx, -8(%rbp)     # -8(%rbp) ⬅︎ new p.z
//  movq (%rax), %rax
//  movq %rax, -16(%rbp)    # -16(%rbp) ⬅︎ new p.x
//  movl -8(%rbp), %eax
//  movl %eax, -32(%rbp)    # -32(%rbp) ⬅︎ -8(%rbp)
//  movq -16(%rbp), %rax
//  movq %rax, -40(%rbp)    # -40(%rbp) ⬅︎ -16(%rbp)
//  movq -40(%rbp), %rax
//  movl -32(%rbp), %edx

//  popq %rbp
//  retq
}
int main() {
  Point a{1, 2, 3};
  auto b = expand(std::move(a));
}
```