# Vectors and Arrays

We want to get to the point where we can program using types that provide exactly the properties we want based on logical needs. To get there we have to overcome a number of fundamental constrains related to access to the bare machine, such as:

- An object in memory is of fixed size.
- An object in memory is in one specific place.
- The computer provides only a few fundamental operations on such objects (such as copying a word, adding the value from two words, etc.).

Every higher-level type (`string`, `vector`, `list`, `map`, etc.) in every language is somehow built from the same machine priitives and reflects a variety of resolutions to the fundamental problems described here.

- Initialization operations
  - to the default value
  - to a specified value
- Copy operations
  - on initialization
  - on assignment
- Releasing resource operations
  - implicit release
  - explicit release

## Initialization

```c++
class vector
{
    int sz;
    double* elem;
public:
    vector();                           // default constructor
    explicit vector(int size);          // initialization to the default value
    vector(initializer_list<double>);   // initialization to specified values
};

void f()
{
    // three elements 1.0, 2.0, 3.0
    vector v1 {1, 2, 3};

    // three elements each with the default value 0.0
    vector v2(3);
}
```

`vector v2(3)` works only because we have the default value. So, having a default constructor is often useful. For every type `T`, `T{}` is the default value. For example:

- `double{}` is 0.0
- `std::string{}` is the empty string `""`
- `std::vector<int>{}` is the empty `vector` of `int`s

## Explicit constructor

A constructor that takes a single argument defines a conversion from its argument type to its class.

```c++
struct X
{
    int val;

    X(int v) : val{v} {}
};
```

However, implicit conversions should be used sparingly and with caution, because they can cause unexpected and undesirable effects.

```c++
class vector
{
    vector(int);
}

vector v = 10;      // odd: makes a vector of 10 doubles
v = 20;             // eh? assigns a new vector of 20 doubles

void f(const vector&);
f(10);              // eh? calls f() with a new vector of 10 doubles
```

Fortunately, it is simple to suppress this use of a constructor as an implicit conversion. A constructor-defined `explicit` provides only the usual construction semantics and not the implicit conversions.

To avoid surprising conversions, we — and the standard — define `vector`’s single-argument constructors to be `explicit`. It’s a pity that constructors are not `explicit` by default.

```c++
class vector
{
    explicit vector(int);
}

vector v = 10;      // error: no int-to-vector conversion
v = 20;             // error
vector v(10);       // OK: makes a vector of 10 doubles

void f(const vector&);
f(10);              // error
f(vector(10));      // OK: calls f() with a new vector of 10 doubles
```

## Copying

Copying is an issue in most programs and in most programming languages. The basic issue is whether you copy a pointer (or reference) or copy the information pointed to (referred to):

- *Shallow copy* copies only a pointer so that the two pointers now refer to the same object.
- *Deep copy* copies what a pointer points to so that the two pointers now refer to distinct objects.

Types that provide shallow copy (like pointers and references) are said to have pointer semantics or *reference semantics* (they copy addresses). Types that provide deep copy (like `string` and `vector`) are said to have *value semantics* (they copy the values pointed to).

From a user perspective, types with value semantics behave as if no pointers were involved — just values that can be copied. One way of thinking of types with value semantics is that they “work just like integers” as far as copying is concerned.

```c++
class vector
{
    int sz;
    double* elem;
public:
    vector();                           // default constructor
    explicit vector(int size);          // initialization to the default value
    vector(initializer_list<double>);   // initialization to specified values

    vector(const vector&);              // copy by initialization
    vector& operator=(const vector&);   // copy by assignment
};

void f()
{
    // three elements 1.0, 2.0, 3.0
    vector v1 {1, 2, 3};

    // three elements each with the default value 0.0
    vector v2(3);

    // copy on initialization
    vector v3 {v1};

    // copy on assignment
    v2 = v3;
}
```

Suppose we have a function `copy()` taking an argument passed by value and returning a value:

```c++
struct X
{
    int val;
    void out(const std::string& s, int nv)
    { std::cout << s << ":" << val << "(" << nv << ")\n"; }

    // default constructor
    X() : val{0} { out("X()", 0); }

    // a single argument constructor
    X(int v) : val{v} { out("X(int)", v); }

    // copy constructor
    X(const X& x) : val{x.val} { out("X(X&)", x.val); }

    // copy assignment
    X& operator=(const X& a)
    { out("X::operator=()", a.val); val = a.val; return *this; }

    // destructor
    ~X() { out("~X()", 0); }
};

X copy(X a) { return a; }
X copy2(X a) { return X{a.val+1}; }
```

As described in the following two sections, passing an argument to functions involves an initialization on copy and value return involves another initialization to produce the return value: 

```c++
X loc{4};
X loc2{loc};
loc = X{5};

std::cout << "copy by value and return\n";
loc2 = copy(loc);

std::cout << "calling copy2(loc)\n";
loc2 = copy2(loc);

output:
"copy by value and return\n"
    "X(X&):5(5)\n"          // copying the argument value
    "X(X&):5(5)\n"          // initializing a return value
    "X::operator=():4(5)\n" // copy assignment
    "~X():5(0)\n"           // the return value is a temporary
    "~X():5(0)\n"           // the argument is a local variable

"calling copy2(loc)\n"
    "X(X&):5(5)\n"          // copying the argument value
    "X(int):6(6)\n"         // initializing a return value
    "X::operator=():5(6)\n" // copy assignment
    "~X():6(0)\n"           // the return value is a temporary
    "~X():5(0)\n"           // the argument is a local variable
```

## Returning a value (§8.5.2)

We return a value from a function using a `return` statement:

```c++
T f()
{
    V v;
    // ...
    return v;
}

T x = f();
```

Here, the value returned is exactly the value we would have gotten by initializing a variable of type `T` with a value of type `V`:

```c++
    V v;
    // ...
    T t{v};     // initialize t with v
```

That is, value return is a form of *initialization*.

## Pass-by-vale (§8.5.3)

The simplest way of passing an argument to a function is to give the function a copy of the value you use as the argument. An argument of a fuction `f()` is a local variable in `f()` that's *initialized* each time `f()` is called.

```c++
int f(int x)
{
    x = x+1;
    return x;
}
int g()
{
    int xx = 0;
    expect(f(xx) == 1);
    expect(xx == 0);
}
```

Since a copy is passed, the `x=x+1` in `f()` does not change the value `xx` passed in the call. Pass-by-value is pretty straightforward abd its cost is the cost of copying the value.

## Essential operations

A class needs a *destructor* if it aquires resources.

```c++
class vector
{
    int sz;
    double* elem;
public:
    vector();                           // default constructor
    explicit vector(int size);          // initialization to the default value
    vector(initializer_list<double>);   // initialization to specified values
    ~vector();                          // destructor

    vector(const vector&);              // copy by initialization
    vector& operator=(const vector&);   // copy by assignment
};
```

A resource is something you get from somewhere and that you must give back once you have finished using it. For example:

- memory from the free store
- files
- locks
- thread handles
- sockets

Another sign that a class needs a destructor is simply that it has members that are pointers or references. If a class has a pointer or a reference member, it often needs a destructor and copy operations.

A class that needs a destructor almost always also need a copy constructor and a copy assignment. The reason is simply that if an object has aquired a resource (and has a pointer member pointing to it), the default meaning of copy (shallow, memberwise copy) is almost certainly wrong.

Similarly, a class that needs a destructor almost always also needs a move constructor and a move assignment. The reason is simply that if an object has acquired a resource (and has a pointer member pointing to it), the default meaning of copy (shallow, memberwise copy) is almost certainly wrong and the usual remedy (copy operations that duplicate the complete object state) can be expensive.

A destructor is called whenever an object of its class is destroyed; that happens when
- names go out of scope,
- the program terminates, or
- `delete` is used on a pointer to an object.

## Access to `vector` elements

We want our usual subscript notation: `v[i]`.

```c++
class vector
{
    int sz;
    double* elem;
public:
    vector();                           // default constructor
    explicit vector(int size);          // initialization to the default value
    vector(initializer_list<double>);   // initialization to specified values
    ~vector();                          // destructor

    vector(const vector&);              // copy by initialization
    vector& operator=(const vector&);   // copy by assignment

    double operator[](int n);           // return value; a problem
};
```

Letting the subscript operator return a value enables reading but not writing of elements. `v[3]` is a floating-point value, not a floating-point variable.

```c++
vector v(10);
double x = v[2];    // fine
v[3] = x;           // error: v[3] is not an lvalue
```

Returning a reference from the subscript operator solves this problem:

```c++
class vector
{
    int sz;
    double* elem;
public:
    vector();                           // default constructor
    explicit vector(int size);          // initialization to the default value
    vector(initializer_list<double>);   // initialization to specified values
    ~vector();                          // destructor

    vector(const vector&);              // copy by initialization
    vector& operator=(const vector&);   // copy by assignment

    int size() { return sz; }
    double& operator[](int n);          // return reference; still not enough
};
```

Now we can write:

```c++
vector v(10);
for (int i = 0; i < v.size(); ++i) {
    v[i] = i;
}
```

## Overloading on `const`

The `operator[]()` defined so far has a problem: it cannot be invoked for a `const vector`.

```c++
void f(const vector& cv)
{
    double d = cv[1];       // error
    cv[1] = 2.0;            // error
}
```

The reason is that our `vector::operator[]()` could potentially change a `vector`. It doesn’t, but the compiler doesn’t know that because we “forgot” to tell it. The solution is to provide a version that is a `const` member function

```c++
class vector
{
    int sz;
    double* elem;
public:
    vector();                           // default constructor
    explicit vector(int size);          // initialization to the default value
    vector(initializer_list<double>);   // initialization to specified values
    ~vector();                          // destructor

    vector(const vector&);              // copy by initialization
    vector& operator=(const vector&);   // copy by assignment

    int size() { return sz; }
    double& operator[](int n);          // for non-const vectors
    double operator[](int n) const;     // for const vectors
};
```

We could equally well have returned a `const double&`, but since a `double` is a small object there would be no point in returning a reference, so we decided to pass it back by value.

## Arrays

An *array* is a homogeneous sequence of objects allocated in contiguous memory; that is, all elements of an array have the same type and there are no gaps between the objects of the sequence. The elements of an array are numbered from 0 upward.

```c++
void f(int n)
{
    char lac[20];       // local array; lives until the end of scope
    int lai[60];
    double lad[n];      // error: array size not a constant
}
```

Note the limitation: the number of elements of a named array must be known at compile time. If you want the number of elements to be a variable, you must put it on the free store and access it through a pointer. That's what `vector` does with its array of lements.

Note that the pointer you get from treating the name of an array as a pointer to its first element is a value and not a variable, so you cannot assign to it:

```c++
char ac[10];
ac = new char [20];         // error: no assignment to array name
&ac[0] = new char [20];     // error: no assignment to pointer value
```

As a consequence of this implicit array-name-to-pointer conversion, you can’t even copy arrays using assignment:

```c++
int x[100];
int y[100];
// ...
x = y;                      // error
int z[100] = y;             // error
```

If we want assignment, we have to use something like the standard library `vector`. The `vector` equivalent to the copying code above is

```c++
vector<int> x(100);
vector<int> y(100);
// ...
x = y;                      // copy 100 ints
```

An array of `char`s can be initialized with a string literal.

```c++
char ac[] = "Beorn";
expect(ac[5] == 0);
```

Count those characters. There are five, but `ac` becomes an array of six characters because the compiler adds a terminating zero character at the end of a string literal.

Only character arrays can be initialized by literal strings, but all arrays can be initialized by a list of values of their element type.

```c++
int ai[] = { 1, 2, 3, 4, 5, 6 };        // array of 6 ints
int ai2[100] = {0,1,2,3,4,5,6,7,8,9};   // the last 90 elements are 0
double ad[100] = {};                    // all elements initialized to 0.0
char chars[] = {'a', 'b', 'c'};         // no terminating 0!
```

Note that the number of elements of `ai` is six (not seven) and the number of elements for `chars` is three (not four) — the “add a 0 at the end” rule is for literal character strings only.
