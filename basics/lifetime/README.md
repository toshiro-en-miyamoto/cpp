# Object Lifetime

## Construction and destruction

Let's start with constructors and a destructor:

```c++
#include <string>
#include <iostream>

static constexpr std::string_view tab{"    "}, comment{"// "};

void print(const std::string& name, char c1, const void* a1) {
    std::cout << tab << comment << name
    << " X{'" << c1 << "'} @ " << a1 << '\n';
}

class X
{
    char c;
public:
    X() : c{'~'} {
        print("new ", '~', this);
    }
    X(char arg) : c{arg} {
        print("new ", c, this);
    }
    ~X() {
        print("del ", c, this);
    }
};

int main()
{
    X x1;
    X x2{'2'};
}
```

> Note: `'~'` is used to mean that the value of member `c` is unspecified.

Run the above code, then things happen under the hood:

```c++
int main()
{
    X x1;
    // new  X{'~'} @ 0x7fff03eac4c6

    X x2{'2'};
    // new  X{'2'} @ 0x7fff03eac4c7
}
    // del  X{'2'} @ 0x7fff03eac4c7
    // del  X{'~'} @ 0x7fff03eac4c6
```

Findings:

- We call constructors (e.g., `X x1{'1'};`), when they *construct* objects - one object at a call.
- We don't call destructors, but they are implicitly invoked when the objects go out of scope.
- Constructors allocate memory for the objects (e.g., `0x7fff03eac4c6` for `x1`).
- Destructors deallocate memory for the objects.

## Copy semantics

Here an assignment `x1 = x2` is added. Notice that the value at `0x7fffd42e8556` is `'~'` upon construction, but `'2'` when `x1` is destructed.

```c++
int main()
{
    X x1;
    // new  X{'~'} @ 0x7fffd42e8556

    X x2{'2'};
    // new  X{'2'} @ 0x7fffd42e8557

    x1 = x2;
}
    // del  X{'2'} @ 0x7fffd42e8557
    // del  X{'2'} @ 0x7fffd42e8556
```

Findings:

- does not change any propertes of `x2`; its value (`'2'`) and memory-address (`0x7ffeeb9074d7`) remain unchanged.
- changes the value of `x1` (from `'~'` to `2`), but not the memory address.

[Copy assignment](https://en.cppreference.com/w/cpp/language/copy_assignment): If your class does not declare any *copy assignment operator*, the compiler implicitly defines one and generates its function body for you. For `class` types, the implicitly-defined copy assignment operator performs *member-wise copy* assignment of the object's members.

Member-wise copy is also knwon as *shallow copy*. It is easy to define such simple a copy assignment operator.

```c++
class X
{
    char c;
public:
    X() : c{'~'} {
        print("new ", '~', this);
    }
    X(char arg) : c{arg} {
        print("new ", c, this);
    }
    ~X() {
        print("del ", c, this);
    }
    X& operator=(const X& x) {          // copy assignment operator
        c = x.c;
        print("copy", c, this, &x);
        return *this;
    }
};

int main()
{
    X x1;
    // new  X{'~'} @ 0x7ffeeb9074d6

    X x2{'2'};
    // new  X{'2'} @ 0x7ffeeb9074d7

    x1 = x2;
    // copy X{'2'} @ 0x7ffeeb9074d6 <= X{'2'} @ 0x7ffeeb9074d7
}
    // del  X{'2'} @ 0x7ffeeb9074d7
    // del  X{'2'} @ 0x7ffeeb9074d6
```

Because the only member of the class `X` is `char c`, shallow copy is enough for the class. We will study shortly the case where a class has a pointer referring to a distinct object, where *deep copy* is required.

## Copy initialization

We often define *copy constructors* along with copy assignment operators.

```c++
class X
{
    char c;
public:
    X() : c{'~'} {
        print("new ", '~', this);
    }
    X(char arg) : c{arg} {
        print("new ", c, this);
    }
    ~X() {
        print("del ", c, this);
    }
    X(const X& x) : c{x.c} {            // copy constructor
        print("ctor", c, this, &x);
    }
    X& operator=(const X& x) {          // copy assignment operator
        c = x.c;
        print("copy", c, this, &x);
        return *this;
    }
};
```

The copy constructor takes part in function call. Suppose we call a function `ident()` that takes the argument `arg` of type `X` and returns it as it is:

```c++
X ident(X arg)
{
    return arg;
}

int main()
{
    X x1;
    // new  X{'~'} @ 0x7ffe4c9b9ed4

    X x2{'2'};
    // new  X{'2'} @ 0x7ffe4c9b9ed5

    x1 = ident(x2);
    // ctor X{'2'} @ 0x7ffe4c9b9ed6 <= X{'2'} @ 0x7ffe4c9b9ed5
    // ctor X{'2'} @ 0x7ffe4c9b9ed7 <= X{'2'} @ 0x7ffe4c9b9ed6
    // copy X{'2'} @ 0x7ffe4c9b9ed4 <= X{'2'} @ 0x7ffe4c9b9ed7
    // del  X{'2'} @ 0x7ffe4c9b9ed7
    // del  X{'2'} @ 0x7ffe4c9b9ed6

    x1 = x2;
    // copy X{'2'} @ 0x7ffe4c9b9ed4 <= X{'2'} @ 0x7ffe4c9b9ed5
}
    // del  X{'2'} @ 0x7ffe4c9b9ed5
    // del  X{'2'} @ 0x7ffe4c9b9ed4
```

Findings:

- The variable `x2` (at `0x7ffcf14a2a85`) is copied to an `X` object at `0x7ffcf14a2a86`. The object is the `arg` argument of the function `ident()`, which is constructed by the copy constructor. Arguments are regarded as a local variable in the scope of the function body, and therefore get destructed when execution goes out of the scope.
- The `arg` variable (at `0x7ffe4c9b9ed6`) is copied to another `X` object at `0x7ffe4c9b9ed7`. The object is the value returned by the `ident()`. The return value is a temporary object constructed by the copy constructor (see below), and therefore gets destructed once the value is copied to `x1`.

[Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization) is performed in the following situations:

| syntax            | explanation
|-------------------|-------------
| `f(`other`)`      | when passing an argument to a function by value
| `return` other`;` | when returning from a function that returns by value

The [`return`](https://en.cppreference.com/w/cpp/language/return) statement: Returning by value may involve construction and copy/move of a temporary object, unless copy elision is used.

## Move semantics

You might notice that when calling the function `ident()`, the preceeding code made copy operations three times:

1. `x2` is copied to `arg`
2. `arg` is copied to the return value
3. the return value is copied to `x1`

```c++
    x1 = ident(x2);
    // ctor X{'2'} @ 0x7ffeb0a3c966 <= X{'2'} @ 0x7ffeb0a3c965 [1]
    // ctor X{'2'} @ 0x7ffeb0a3c967 <= X{'2'} @ 0x7ffeb0a3c966 [2]
    // copy X{'2'} @ 0x7ffeb0a3c964 <= X{'2'} @ 0x7ffeb0a3c967 [3]
    // del  X{'2'} @ 0x7ffeb0a3c967
    // del  X{'2'} @ 0x7ffeb0a3c966
```

You can define [*move constructors*](https://en.cppreference.com/w/cpp/language/move_constructor) and [*move assignment operators*](https://en.cppreference.com/w/cpp/language/move_assignment): they typically *steal* the resources held by the argument, rather than make copies of them, and leave the argument in some valid but otherwise indeterminate state.

```c++
class X
{
    char c;
public:
    X() : c{'~'} {
        print("new ", '~', this);
    }
    X(char arg) : c{arg} {
        print("new ", c, this);
    }
    ~X() {
        print("del ", c, this);
    }
    X(const X& x) : c{x.c} {
        print("ctor", c, this, &x);
    }
    X& operator=(const X& x) {
        c = x.c;
        print("copy", c, this, &x);
        return *this;
    }
    X(X&& x) : c{x.c} {                 // move constructor
        x.c = '~';      // leave x in the moved-from state
        print("mtor", c, this, &x);
    }
    X& operator=(X&& x) {               // move assignment operator
        c = x.c;
        x.c = '~';      // leave x in the moved-from state
        print("move", c, this, &x);
        return *this;
    }
};
```

Note that for simple classes such as the class `X`, the move constructor and the move assignment operator resemble copy constructors and copy assignment operators in that they don't steel anything but just make copies. They wouldn't should deep copy be required.

With the move constructor and the move assignment operator,

1. `x2` is copied to `arg`
2. `arg` is *moved* to the return value
3. the return value is *moved* to `x1`

```c++
    x1 = ident(x2);
    // ctor X{'2'} @ 0x7ffd2612dfc6 <= X{'2'} @ 0x7ffd2612dfc5 [1]
    // mtor X{'2'} @ 0x7ffd2612dfc7 <= X{'2'} @ 0x7ffd2612dfc6 [2]
    // move X{'2'} @ 0x7ffd2612dfc4 <= X{'2'} @ 0x7ffd2612dfc7 [3]
    // del  X{'~'} @ 0x7ffd2612dfc7
    // del  X{'~'} @ 0x7ffd2612dfc6
```
