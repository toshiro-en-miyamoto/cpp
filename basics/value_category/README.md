# [Value categories](https://en.cppreference.com/w/cpp/language/value_category)

With the introduction of move semantics in C++11, value categories were redefined to characterize two independent properties of expressions:

- has identity: it's possible to determine whether the expression refers to the same entity as another expression, such as by comparing addresses of the objects or the functions they identify (obtained directly or indirectly);
- can be moved from: move constructor, move assignment operator, or another function overload that implements move semantics can bind to the expression.

In C++11, expressions that:

| identity      | movable       | called        | stands for
|---------------|---------------|---------------|----------------
| have identity | not movable   | **lvalue**    | left-hand-side value
| have identity | movable       | **xvalue**    | eXpiring value
| no identity   | movable       | **prvalue**   | pure rvalue
| have identity | (whichever)   | *glvalue*     | generalized lvalue
| (whichever)   | movable       | *rvalue*      | right-hand-side value

Expressions that do not have identity and cannot be moved from are not used.

|                   | has identity  | no identity   |           |
|-------------------|:-------------:|:-------------:|:---------:|
| **is movable**    | xvalue        | prvalue       | rvalue    |
| **not movable**   | lvalue        | (n/a)         |           |
|                   | glvalue       |               |           |
