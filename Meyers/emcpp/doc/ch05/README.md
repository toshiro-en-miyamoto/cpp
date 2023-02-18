# Chapter 5. Rvalue References, Move Semantics, and Perfect Forwarding

Rvalue references are the underlying language mechanism that makes both move semantics and perfect forwarding possible.

A parameter is always an lvalue, even if its type is an rvalue reference. That is, given

```c++
void f(Widget&& w);
```

the parameter `w` is an lvalue, even though its type is rvalue-reference-to-`Widget`.

- [Item 23](./item23.md): Understand s`td::move` and `std::forward`.
