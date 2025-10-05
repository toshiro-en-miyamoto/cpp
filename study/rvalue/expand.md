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
Point&& expand(Point&& p) {
  p.x++;
  p.y++;
  p.z++;
  // an rvalue reference cannot bind to an lvalue
  // Point&& cannot bind to Point&& p (p is an lvalue)
  // return p;
  return std::move(p);
}
int main() {
  Point a{1, 2, 3};
  // an rvalue reference cannot bind to an lvalue
  // Point&& p cannot bind to Point a (a is an lvalue)
  // auto b = expand(a);
  auto b = expand(std::move(a));
}
```

`std::move` produces an xvalue expression that identifies its argument t. It is exactly equivalent to a `static_cast` to an rvalue reference type. ([C++ reference: move](https://www.cppreference.com/w/cpp/utility/move.html))

An rvalue reference refers to a temporary object, which the user of the reference can modify, assuming that the object will never be used again. (Stroustrup 2013: §7.7.2. Rvalue References)

```c++
Point&& expand(Point&& p) {
//  pushq	%rbp
//  movq	%rsp, %rbp
//  movq	%rdi, -8(%rbp)    // -8(%rbp) ⬅︎ &&p
  p.x++;
//  movq	-8(%rbp), %rax    // %eax ⬅︎ &&p
//  movl	(%rax), %ecx      // %ecx ⬅︎ p->x
//  addl	$1, %ecx          // %ecx ⬅︎ %ecx + 1
//  movl	%ecx, (%rax)      // p->x ⬅︎ %ecx
  p.y++;
//  movq	-8(%rbp), %rax    // %eax ⬅︎ &&p
//  movl	4(%rax), %ecx     // %ecx ⬅︎ p->y
//  addl	$1, %ecx          // %ecx ⬅︎ %ecx + 1
//  movl	%ecx, 4(%rax)     // p->y ⬅︎ %ecx
  p.z++;
//  movq	-8(%rbp), %rax    // %eax ⬅︎ &&p
//  movl	8(%rax), %ecx     // %ecx ⬅︎ p->z
//  addl	$1, %ecx          // %ecx ⬅︎ %ecx + 1
//  movl	%ecx, 8(%rax)     // p->z ⬅︎ %ecx
  return std::move(p);
//  movq	-8(%rbp), %rax    // %rax ⬅︎ &&p
//  popq	%rbp
//  retq
}
int main() {
//  pushq	%rbp
//  movq	%rsp, %rbp
//  subq	$32, %rsp
  Point a{1, 2, 3};
//  .section	__TEXT,__const
//  l___const.main.a:
//  .long	1   ## 0x1        // a[0]
//  .long	2   ## 0x2        // a[1]
//  .long	3   ## 0x3        // a[2]
//  movq	l___const.main.a(%rip), %rax
//  movq	%rax, -12(%rbp)   // -12(%rbp) ⬅︎ &&a[0]
//  movl	l___const.main.a+8(%rip), %eax
//  movl	%eax, -4(%rbp)    //  -4(%rbp) ⬅︎ &&a[2]
  auto b = expand(std::move(a));
//  leaq	-12(%rbp), %rdi   // %rdi ⬅︎ -12(%rbp):&&a[0]
//  callq	__Z6expandO5Point   // call Point::expand()
//  movq	(%rax), %rcx      // &rcx ⬅︎ &&Point[0]
//  movq	%rcx, -24(%rbp)   // &b[0]:-24(%rbp) ⬅︎ &&Point[0]
//  movl	8(%rax), %eax     // &rax ⬅︎ &&Point[2]
//  movl	%eax, -16(%rbp)   // &b[2]:-16(%rbp) ⬅︎ &&Point[2]

//  xorl	%eax, %eax        // rc ⬅︎ 0
//  addq	$32, %rsp
//  popq	%rbp
//  retq
}
```


