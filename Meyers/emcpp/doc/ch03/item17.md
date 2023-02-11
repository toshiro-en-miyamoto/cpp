# Item 17: Understand special member function generation

The *special member functions* are the ones that C++ is willing to generate on its own.

```c++
class StringTable {
public:
    StringTable() {}

private:
    std::map<int, std::string> values;
};
```

Assuming that the class declares no copy operations, no move operations, and no destructor, compilers will automatically generate these functions if they are used.

But suppose that sometime later, it’s decided that logging the default construction and the destruction of such objects would be useful.

```c++
class StringTable {
public:
    StringTable()
    { makeLogEntry("creating"); }

    ~StringTable()
    { makeLogEntry("destroying"); }

private:
    std::map<int, std::string> values;
};
```

Declaring a destructor:

- prevents the move operations from being generated
- does not affect creation of the copy operations

## [Implicitly-declared copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor)

If no user-defined copy constructors are provided for a class type (`struct`, `class`, or `union`), the compiler will *always* **declare** a copy constructor as a non-explicit `inline public` member of its class.

A class can have multiple copy constructors, e.g. both `T::T(const T&)` and `T::T(T&)`.

If some user-defined copy constructors are present, the user may still force the generation of the implicitly declared copy constructor with the keyword `default`.

The implicitly-declared or defaulted copy constructor for class `T` is defined as **deleted** if any of the following conditions are true:

- `T` has non-static data members that cannot be copied (have deleted, inaccessible, or ambiguous copy constructors);
- `T` has direct or virtual base class that cannot be copied (has deleted, inaccessible, or ambiguous copy constructors);
- `T` has direct or virtual base class or a non-static data member with a deleted or inaccessible destructor;
- `T` is a union-like class and has a variant member with non-trivial copy constructor;
- `T` has a data member of rvalue reference type;
- `T` has a user-defined move constructor or move assignment operator (this condition only causes the implicitly-declared, not the defaulted, copy constructor to be deleted).

## [Implicitly-declared copy assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment)

If no user-defined copy assignment operators are provided for a class type (`struct`, `class`, or `union`), the compiler will *always* **declare** one as an `inline public` member of the class.

A class can have multiple copy assignment operators, e.g. both `T& T::operator=(T&)` and `T& T::operator=(T)`. If some user-defined copy assignment operators are present, the user may still force the generation of the implicitly declared copy assignment operator with the keyword `default`.

An implicitly-declared copy assignment operator for class `T` is defined as **deleted** if any of the following is true:

- `T` has a user-declared move constructor;
- `T` has a user-declared move assignment operator.

Otherwise, it is defined as *defaulted*.

A defaulted copy assignment operator for class `T` is defined as **deleted** if any of the following is true:

- `T` has a non-static data member of a const-qualified non-class type (or array thereof);
- `T` has a non-static data member of a reference type;
- `T` has a non-static data member or a direct base class that cannot be copy-assigned (overload resolution for the copy assignment fails, or selects a deleted or inaccessible function);
- `T` is a union-like class, and has a variant member whose corresponding assignment operator is non-trivial.

## [Implicitly-declared move constructor](https://en.cppreference.com/w/cpp/language/move_constructor)

If no user-defined move constructors are provided for a class type (`struct`, `class`, or `union`), and all of the following is true:

- there are no user-declared copy constructors;
- there are no user-declared copy assignment operators;
- there are no user-declared move assignment operators;
- there is no user-declared destructor.

then the compiler will **declare** a move constructor as a non-explicit `inline public` member of its class with the signature `T::T(T&&)`.

A class can have multiple move constructors, e.g. both `T::T(const T&&)` and `T::T(T&&)`. If some user-defined move constructors are present, the user may still force the generation of the implicitly declared move constructor with the keyword `default`.

The implicitly-declared or defaulted move constructor for class T is defined as **deleted** if any of the following is true:

- `T` has non-static data members that cannot be moved (have deleted, inaccessible, or ambiguous move constructors);
- `T` has direct or virtual base class that cannot be moved (has deleted, inaccessible, or ambiguous move constructors);
- `T` has direct or virtual base class or a non-static data member with a deleted or inaccessible destructor;
- `T` is a union-like class and has a variant member with non-trivial move constructor.

A defaulted move constructor that is deleted is ignored by overload resolution (otherwise it would prevent copy-initialization from rvalue).

## [Implicitly-declared move assignment operator](https://en.cppreference.com/w/cpp/language/move_assignment)

If no user-defined move assignment operators are provided for a class type (`struct`, `class`, or `union`), and all of the following is true:

- there are no user-declared copy constructors;
- there are no user-declared move constructors;
- there are no user-declared copy assignment operators;
- there is no user-declared destructor,

then the compiler will **declare** a move assignment operator as an `inline public` member of its class with the signature `T& T::operator=(T&&)`.

A class can have multiple move assignment operators, e.g. both `T& T::operator=(const T&&)` and `T& T::operator=(T&&)`. If some user-defined move assignment operators are present, the user may still force the generation of the implicitly declared move assignment operator with the keyword `default`.

The implicitly-declared or defaulted move assignment operator for class `T` is defined as **deleted** if any of the following is true:

- `T` has a non-static data member that is `const`;
- `T` has a non-static data member of a reference type;
- `T` has a non-static data member or a direct base class that cannot be move-assigned (has deleted, inaccessible, or ambiguous move assignment operator).

A deleted implicitly-declared move assignment operator is ignored by overload resolution.
