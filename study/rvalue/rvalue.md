# Rvalue

References:
- [Stroustrup 2013] Stroustrup. The C++ programming language, 4th edition.
- [C++ reference]

## Value categories

Each C++ expression is characterized by two independent properties: a *type* and a *value category*. Each expression has some non-reference type, and each expression belongs to exactly one of the three primary value categories:

| value category          | identity?       | movable?              |
|:------------------------|:----------------|:----------------------|
| lvalue                  | has identity    | cannot be moved from  |
| xvalue ("pure rvalue")  | has identity    | can be moved from     |
| prvalue ("pure rvalue") | has no identity | can be moved from     |

> An *rvalue* expression is either prvalue or xvalue.

With the introduction of move semantics in C++11, value categories were redefined to characterize two independent properties of expressions:

- *has identity*: it's possible to determine whether the expression refers to the same entity as another expression, such as by comparing addresses of the objects/functions they identify;
- *can be moved from*: move constructor, move assignment operator, or another function overload that implements move semantics can bind to the expression.

The following expressions are xvalue expressions:

- a function call or an overloaded operator expression, whose return type is *rvalue reference* to object, such as `std::move(x)`;
- any expression that designates a temporary object, after temporary materialization; (since C++ 11)

The following expressions are prvalue expressions:

- a literal (except for string literal), such as `42`, `true` or `nullptr`;
- a function call or an overloaded operator expression, whose return type is non-reference, such as `str.substr(1, 2)`, `str1 + str2`, or `it++`;
- the value of an operand of a built-in operator, such as `a++` and `a--`;
- the built-in address-of expression, such as `&a`;

([C++ reference: value categories](https://www.cppreference.com/w/cpp/language/value_category.html))

## References

Like a pointer, a *reference* is an alias for an object, is usually implemented to hold a machine address of an object, and does not impose performance overhead compared to pointers, but it differs from a pointer in that:

- You access a reference with exactly the same syntax as the name of an object.
- A reference always refers to the object to which it was initialized.
- There is no **null reference**, and we may assume that a reference refers to an object.

```c++
int var = 1;
int& r1 {var};                // OK: r1 initialized
int& r2;                      // error: initializer missing
double& dr = 1;               // error: lvalue needed
const double& cdr {1};        // OK
```

The main use of references is for specifying arguments and return values for functions in general and for overloaded operators in particular.

The basic idea of having more than one kind of reference is to support different uses of objects:

- A non-`const` *lvalue reference* refers to an object, to which the user of the reference can write.
- A `const` lvalue reference refers to a constant, which is immutable from the point of view of the user of the reference.
- An *rvalue reference* refers to a temporary object, which the user of the reference can (and typically will) modify, assuming that the object will never be used again.

(Stroustrup 2013: §7.7. References)

## Assignment operators

Copy assignment replaces the contents of the object `a` with a copy of the contents of `b` (`b` is not modified). For class types, this is performed in a special member function, described in *copy assignment operator*.

Move assignment replaces the contents of the object `a` with the contents of `b`, avoiding copying if possible (`b` may be modified). For class types, this is performed in a special member function, described in *move assignment operator*.

For non-class types, copy and move assignment are indistinguishable and are referred to as direct assignment.

([C++ reference: Assignment operators](https://en.cppreference.com/w/cpp/language/operator_assignment.html))

Move assignment operators typically transfer the resources held by the argument (e.g. pointers to dynamically-allocated objects, file descriptors, TCP sockets, thread handles, etc.), rather than make copies of them, and leave the argument in some valid but otherwise indeterminate state.

- Since move assignment doesn’t change the lifetime of the argument, the destructor will typically be called on the argument at a later point.
- For example, move-assigning from a `std::string` or from a `std::vector` may result in the argument being left empty.
- A move assignment is less, not more restrictively defined than ordinary assignment; where ordinary assignment must leave two copies of data at completion, move assignment is required to leave only one.

For non-union class types, the move assignment operator performs full member-wise move assignment of the object's direct bases and immediate non-static members, in their declaration order, using built-in assignment for the scalars, member-wise move-assignment for arrays, and move assignment operator for class types (called non-virtually).

([C++ reference: Move assignment operator](https://www.cppreference.com/w/cpp/language/move_operator.html))
