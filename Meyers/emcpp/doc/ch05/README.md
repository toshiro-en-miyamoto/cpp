# Chapter 5. Rvalue References, Move Semantics, and Perfect Forwarding

Rvalue references are the underlying language mechanism that makes both move semantics and perfect forwarding possible.

A parameter is always an lvalue, even if its type is an rvalue reference. That is, given

```c++
void f(Widget&& w);
```

the parameter `w` is an lvalue, even though its type is rvalue-reference-to-`Widget`.

- [Item 23](./item23.md): Understand s`td::move` and `std::forward`.
- [Item 24](./item24.md): Distinguish universal references from rvalue references.
- [Item 25](./item25.md): Use `std::move` on rvalue references, `std::forward` on universal references.
- Item 26: Avoid overloading on universal references.
