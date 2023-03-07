# Item 32: Use init capture to move objects into closures.

Lambda expressions —*lambdas*— are such a convenient way to create function objects.

- A *lambda expression* is just that: an expression. It’s part of the source code.
- A *closure* is the runtime object created by a lambda. Depending on the capture mode, closures hold copies of or references to the captured data.
- A *closure class* is a class from which a closure is instantiated. Each lambda causes compilers to generate a unique closure class.

Sometimes neither by-value capture nor by-reference capture is what you want:

- a move-only object (e.g., a `std::unique_ptr`)
- an object that's expensive to copy but cheap to move

The absence of move capture was recognized as a shortcoming even as C++11 was adopted. [C++14] introduced a new capture mechanism that’s so flexible, capture-by-move is only one of the tricks it can perform. The new capability is called *init capture*.

```c++
class Widget {
public:
    bool isValidated() const;
    bool isProcessed() const;
    bool isArchived() const;
};

auto pw = std::make_unique<Widget>();

auto func = [pw = std::move(pw)] {      // init caprure
    return pw->isValidated()
        && pw->isArchived();
};
```

In the init capture,

- to the left of the `=`
  - is the name of the data member in the closure class you’re specifying
  - has the scope of the closure class
- to the right of the `=`
  - is the initializing expression
  - has the same scope as where the lambda is being defined

In the example above `[pw = std::move(pw)]`:

- the name `pw` on the left of the `=` refers to a data member in the closure class, while
- the name `pw` on the right refers to the object declared above the lambda, i.e., the variable initialized by the call to `std::make_unique`.

The example above is the code where after the Widget is created by `std::make_unique` and before the `std::unique_ptr` to that `Widget` is captured by the lambda, the `Widget` is modified in some way.

If no such modification is necessary, the closure class’s data member can be directly initialized by `std::make_unique`:

```c++
auto func = [pw = std::make_unique<Widget>()] {
    return pw->isValidated()
        && pw->isArchived();
};
```

This should make clear that the C++14 notion of “capture” is considerably generalized from C++11, because in C++11, it’s not possible to capture the result of an expression.
