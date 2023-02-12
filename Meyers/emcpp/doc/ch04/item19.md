# Item 19: Use `std::shared_ptr` for shared-ownership resource management.

Why can’t we have the best of both worlds: a system that works automatically (like garbage collection), yet applies to all resources and has predictable timing (like destructors)?

A `std::shared_ptr` can tell whether it’s the last one pointing to a resource by consulting the resource’s *reference count*, a value associated with the resource that keeps track of how many `std::shared_ptrs` point to it.

- `std::shared_ptr` constructors increment this count,
- destructors decrement it, and
- copy assignment operators do both.

If `sp1` and `sp2` are `std::shared_ptrs` to different objects, the assignment `sp1 = sp2;` modifies `sp1` such that it points to the object pointed to by `sp2`. The net effect of the assignment is:

- the reference count for the object originally pointed to by `sp1` is decremented,
- while that for the object pointed to by `sp2` is incremented.

If a `std::shared_ptr` sees a reference count of zero after performing a decrement, no more `std::shared_ptr`s point to the resource, so the `std::shared_ptr` destroys it.

Move-constructing a `std::shared_ptr` from another `std::shared_ptr` sets the source `std::shared_ptr` to `null`, and that means that the old `std::shared_ptr` stops pointing to the resource at the moment the new `std::shared_ptr` starts. As a result, no reference count manipulation is required.

For `std::unique_ptr`, the type of the deleter is part of the type of the smart pointer. For `std::shared_ptr`, it’s not:

```c++
auto loggingDel = [](Widget* pw)
{
    makeLogEntry(pw);
    delete pw;
};

std::unique_ptr<Widget, decltype(loggingDel)>
upw(new Widget, loggingDel);

std::shared_ptr<Widget>
spw(new Widget, loggingDel);
```

## Control blocks

`std::shared_ptr` uses the *control block*:

- `std::nmake_shared` always creates a control block.
- A control block is created when a `std::shared_ptr` is consturcted from unique-ownership pointer.
- When a `std::shared_ptr` constructor is called with a raw pointer, it creates a control block.

```c++
auto pw = new Widget;
std::shared_ptr<Widget> spw1(pw, loggingDel);
std::shared_ptr<Widget> spw2(pw, loggingDel);
```

The constructor for `spw1` is called with a raw pointer, so it creates a control block. The constructor for `spw2` is called with the same raw pointer, so it also creates a control block (hence a reference count) for `*pw`. `*pw` thus has two reference counts, each of which will eventually become zero, and that will ultimately lead to an attempt to destroy `*pw twice`. The second destruction is responsible for the undefined behavior.

The above code must be written as:

```c++
std::shared_ptr<Widget> spw1(new Widget, loggingDel);
std::shared_ptr<Widget> spw2(spw1);
```

## `std::enable_shared_from_this`

Suppose our program uses `std::shared_ptr`s to manage `Widget` objects, and we have a data structure that keeps track of Widgets that have been processed:

```c++
std::vector<std::shared_ptr<Widget>> processedWidgets;
```

This code will compile, but it’s passing a raw pointer (`this`) to a container of `std::shared_ptr`s. The `std::shared_ptr` thus constructed will create a new control block for the pointed-to Widget (`*this`). it’s game, set, and match for undefined behavior.

```c++
class Widget
{
public:
    void process()
    {
        processWidgets.emplace_back(this);
    }
};
```

You want a class managed by `std::shared_ptr`s to be able to safely create a `std::shared_ptr` from a `this` pointer:

```c++
class Widget: public std::enable_shared_from_this<Widget>
{
public:
    void process()
    {
        processWidgets.emplace_back(shared_from_this());
    }

    template<typename... Ts>
    static std::shared_ptr<Widget> create(Ts&&... params);

private:
    // ctors
};
```

Internally, `shared_from_this` looks up the control block for the current object, and it creates a new `std::shared_ptr` that refers to that control block.

To prevent clients from calling member functions that invoke `shared_from_this` before a `std::shared_ptr` points to the object, classes inheriting from `std::enable_shared_from_this` often declare their constructors private and have clients create objects by calling factory functions that return `std::shared_ptr`s.

## Arrays

`std::shared_ptr` is designed only for pointers to single objects; there's no `std::shared_ptr<T[]>`.

Given the variety of C++11 alternatives to built-in arrays (e.g., `std::array`, `std::vector`, `std::string`), declaring a smart pointer to a dumb array is almost always a sign of bad design.
