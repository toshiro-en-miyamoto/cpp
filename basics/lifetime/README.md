# Object Lifetime

## Construction and destruction

Let's start with constructors and a destructor:

```c++
#include <iostream>
#include <boost/ut.hpp>
#include <my/hijack.h>

struct X
{
    char c_;

    X() : c_ {'~'}
    { std::cout << "new  ~\n"; }
    X(char c) : c_ {c}
    { std::cout << "new  " << c_ << '\n'; }
    ~X()
    { std::cout << "del  " << c_ << '\n'; }

    const auto addr() const
    { return this; }
};
```

> Note: `'~'` is used to mean that the value of member `c_` is unspecified.

Run the above code, then things happen under the hood:

```c++
int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "object lifetime"_test = [] {
        given("constructors") = [] {
            my::hijack hj(std::cout);
            when("constructed") = [] {
                X xA {'A'};
                X xB {'B'};
                expect(xA.addr() != xB.addr());
            };
            auto actual = hj.release();
            expect(actual ==
                "new  A\n"
                "new  B\n"
                "del  B\n"
                "del  A\n"
            ) << actual;
        };
    };
}
```

Findings:

- We call constructors (e.g., `X xA {'A'};`), when they *construct* objects - one object at a call (e.g., `new  A`).
- We don't call destructors, but they are implicitly invoked when the objects go out of scope (e.g., `del  A`).
- Constructors allocate distinct memory for each object (thus, `xA.addr() != xB.addr()`).
- Destructors deallocate memory for the objects.

## Copy semantics

Here an assignment `xA = xB` is added. Notice that the value of `xA` is `'B'` when destructed (the last `del  B`).

```c++
int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "object lifetime"_test = [] {
        given("constructors") = [] {
        };
        given("copy assign") = [] {
            my::hijack hj(std::cout);
            when("copied") = [] {
                X xA {'A'};
                auto xAaddr = xA.addr();
                X xB {'B'};
                auto xBaddr = xB.addr();
                xA = xB;
                expect(xAaddr == xA.addr());
                expect(xBaddr == xB.addr());
                expect(xA.addr() != xB.addr());
            };
            auto actual = hj.release();
            expect(actual ==
                "new  A\n"
                "new  B\n"
                "copy B over A\n"
                "del  B\n"
                "del  B\n"
            ) << actual;
        };
    };
}
```

Findings:

- does not change any propertes of `xB`; its value (`'B'`) and memory-address remain unchanged (`xBaddr == xB.addr()`).
- changes the value of `xA` (from `'A'` to `B`), but not the memory address (`xAaddr == xA.addr()`).

[Copy assignment](https://en.cppreference.com/w/cpp/language/copy_assignment): If your class does not declare any *copy assignment operator*, the compiler implicitly defines one and generates its function body for you. For `class` types, the implicitly-defined copy assignment operator performs *member-wise copy* assignment of the object's members.

Member-wise copy is also knwon as *shallow copy*. It is easy to define such simple a copy assignment operator.

```c++
struct X
{
    char c_;

    X() : c_ {'~'}
    { std::cout << "new  ~\n"; }
    X(char c) : c_ {c}
    { std::cout << "new  " << c_ << '\n'; }
    ~X()
    { std::cout << "del  " << c_ << '\n'; }
    X& operator=(const X& rhs)
    {
        std::cout   << "copy " << rhs.c_
                    << " over " << c_ << '\n';
        c_ = rhs.c_;
        return *this;
    }

    const auto addr() const
    { return this; }
};
```

Because the only member of the class `X` is `char c_`, shallow copy is enough for the class. If a class has a pointer referring to a distinct object, *deep copy* is required.

## Copy initialization

We often define *copy constructors* along with copy assignment operators.

```c++
struct X
{
    char c_;

    X() : c_ {'~'}
    { std::cout << "new  ~\n"; }
    X(char c) : c_ {c}
    { std::cout << "new  " << c_ << '\n'; }
    ~X()
    { std::cout << "del  " << c_ << '\n'; }
    X& operator=(const X& rhs)
    {
        std::cout   << "copy " << rhs.c_
                    << " over " << c_ << '\n';
        c_ = rhs.c_;
        return *this;
    }
    X(const X& rhs) : c_ {rhs.c_}
    { std::cout << "ctor " << c_ << '\n'; }

    const auto addr() const
    { return this; }
};
```

The copy constructor takes part in function call. Suppose we call a function `ident()` that takes the argument `arg` of type `T` and returns it as it is:

```c++
template<typename T>
T ident(T arg)
{
    return arg;
}

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "object lifetime"_test = [] {
        given("constructors") = [] {
        };
        given("copy assign") = [] {
        };
        given("copy constructor") = [] {
            my::hijack hj(std::cout);
            when("constructed") = [] {
                X xA1 {'A'};
                X xA2 {xA1};
                expect(xA1.addr() != xA2.addr());
            };
            auto actual = hj.release();
            expect(actual ==
                "new  A\n"
                "ctor A\n"
                "del  A\n"
                "del  A\n"
            ) << actual;
        };
        given("without move ctor & assign") = [] {
            my::hijack hj(std::cout);
            when("copied") = [] {
                X xA {'A'};
                X xB {'B'};
                auto xAaddr = xA.addr();
                xA = ident(xB);
                expect(xAaddr == xA.addr());
            };
            auto actual = hj.release();
            expect(actual ==
                "new  A\n"
                "new  B\n"
                // call to 'indent(xB)' copies xB to arg
                "ctor B\n"
                // 'return arg' creates a copy of arg
                "ctor B\n"
                // assign the copy to xA
                "copy B over A\n"
                // leaving indent()
                // a copy of arg is destructed
                "del  B\n"
                // arg is going out of scope
                "del  B\n"
                // xB and xA are going out of scope
                "del  B\n"
                "del  B\n"
            ) << actual;
        };
    };
}
```

Findings:

- The variable `xB` is copied to the `arg` variable of the function `ident()`, which is constructed by the copy constructor. Arguments are regarded as a local variable in the scope of the function body, and therefore get destructed when execution goes out of the scope.
- The `arg` variable is copied to another `X` object by `return arg;`. The object is the value returned by the `ident()`. The return value is a temporary object constructed by the copy constructor (see below), and therefore gets destructed once the value is copied to `xB`.

[Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization) is performed in the following situations:

| syntax            | explanation
|-------------------|-------------
| `f(`other`)`      | when passing an argument to a function by value
| `return` other`;` | when returning from a function that returns by value

The [`return`](https://en.cppreference.com/w/cpp/language/return) statement: Returning by value may involve construction and copy/move of a temporary object, unless copy elision is used.

## Move semantics

You might notice that when calling the function `ident()`, the preceeding code made copy operations three times:

1. `xB` is copied to `arg`
2. `arg` is copied to the return value
3. the return value is copied to `xA`

```c++
            expect(actual ==
                "new  A\n"
                "new  B\n"
                // call to 'indent(xB)' copies xB to arg
                "ctor B\n"
                // 'return arg' creates a copy of arg
                "ctor B\n"
                // assign the copy to xA
                "copy B over A\n"
                // leaving indent()
                "del  B\n"
                "del  B\n"
                "del  B\n"
                "del  B\n"
            ) << actual;
```

You can define [*move constructors*](https://en.cppreference.com/w/cpp/language/move_constructor) and [*move assignment operators*](https://en.cppreference.com/w/cpp/language/move_assignment): they typically *steal* the resources held by the argument, rather than make copies of them, and leave the argument in some valid but otherwise indeterminate state.

```c++
struct Y
{
    char c_;

    Y() : c_ {'~'}
    { std::cout << "new  ~\n"; }
    Y(char c) : c_ {c}
    { std::cout << "new  " << c_ << '\n'; }
    ~Y()
    { std::cout << "del  " << c_ << '\n'; }
    Y& operator=(const Y& rhs)
    {
        std::cout   << "copy " << rhs.c_
                    << " over " << c_ << '\n';
        c_ = rhs.c_;
        return *this;
    }
    Y(const Y& rhs) : c_ {rhs.c_}
    { std::cout << "ctor " << c_ << '\n'; }
    Y(Y&& rhs) : c_ { std::exchange(rhs.c_, '~') }
    { std::cout << "mtor " << c_ << '\n'; }
    Y& operator=(Y&& rhs)
    {
        std::cout   << "move " << rhs.c_
                    << " over " << c_ << '\n';
        c_ = std::exchange(rhs.c_, '~');
        return *this;
    }

    const auto addr() const
    { return this; }
};
```

Note that for simple classes such as the class `Y`, the move constructor and the move assignment operator resemble copy constructors and copy assignment operators in that they don't steel anything but just make copies. They wouldn't should deep copy be required.

With the move constructor and the move assignment operator,

1. `yB` is copied to `arg`
2. `arg` is *moved* to the return value
3. the return value is *moved* to `yA`

```c++
int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "object lifetime"_test = [] {
        given("constructors") = [] {
        };
        given("copy assign") = [] {
        };
        given("copy constructor") = [] {
        };
        given("without move ctor & assign") = [] {
        };
        given("move ctor & assign") = [] {
            my::hijack hj(std::cout);
            when("copied") = [] {
                Y yA {'A'};
                Y yB {'B'};
                auto yAaddr = yA.addr();
                yA = ident(yB);
                expect(yAaddr == yA.addr());
            };
            auto actual = hj.release();
            expect(actual ==
                "new  A\n"
                "new  B\n"
                "ctor B\n"
                "mtor B\n"
                "move B over A\n"
                "del  ~\n"
                "del  ~\n"
                "del  B\n"
                "del  B\n"
            ) << actual;
        };
    };
}
```
