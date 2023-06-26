# Item 25: Use `std::move` on rvalue references, `std::forward` on universal references.

- Apply `std::move` to rvalue references and `std::forward` to universal references the last time each is used.
- Do the same thing for rvalue references and universal references being returned from functions that return by value.
- Never apply `std::move` or `std::forward` to local objects if they would otherwise be eligible for the return value optimization (RVO).

## Runtime performance

Consider a use of `Widget::setName`:

```c++
Widget w;
w.setName("Adela Novas");
```

If `setName` had simply been overloaded for `const` lvalues and for rvalues:

```c++
class Widget
{
public:
  void setName(const std::string& newName)
  { name = newName; }

  void setName(std::string&& newName)
  { name = std::move(newName); }

private:
  std::string name;
};
```

A call to `setName` would entail execution of:

- one `std::string` constructor (to create the temporary),
- one `std::string` move assignment operator (to move `newName` into `w.name`), and
- one `std::string` destructor (to destroy the temporary).

If `setName` takes a universal reference:

```c++
class Widget
{
public:
  template<typename T>
  void setName(T&& newName)
  { name = std::forward<T>(newName); }

private:
  std::string name;
};
```

the string literal `"Adela Novae"` would be passed to `setName`, where it would be conveyed to the assignment operator for the `std::string` inside `w`. `w`’s `name` data member would thus be assigned directly from the string literal; no temporary `std::string` objects would arise.

## Poor scalability of the design

The most serious problem with overloading on lvalues and rvalues is the poor scalability of the design. `Widget::setName` takes only one parameter, so only two overloads are necessary, but for functions taking more parameters, each of which could be an lvalue or an rvalue, the number of overloads grows geometrically: `n` parameters necessitates `2ⁿ` overloads.

The poster children for such functions are:

```c++
template<class T, class... Args>
shared_ptr<T> make_shared(Args&&... args);

template<class T, class... Args>
unique_ptr<T> make_unique(Args&&... args);
```

For functions like these, overloading on lvalues and rvalues is not an option: universal references are the only way to go.

## Returning by value

If you’re in a function that returns *by value*, and you’re returning an object bound to an rvalue reference or a universal reference, you’ll want to apply `std::move` or `std::forward` when you return the reference.

### Taking a rvalue reference

Consider an `operator+` function to add two matrices together, where the left-hand matrix is known to be an rvalue (and can hence have its storage reused to hold the sum of the matrices):

```c++
Matrix operator+(Matrix&& lhs, const Matrix& rhs)
{
  lhs + = rhs;
  return std::move(lhs);
}
```

By casting `lhs` to an rvalue in the return statement (via `std::move`), `lhs` will be moved into the function’s return value location. If the call to `std::move` were omitted, the fact that `lhs` is an lvalue would force compilers to instead copy it into the return value location.

If `Matrix` does not support moving, casting it to an rvalue won’t hurt, because the rvalue will simply be copied by `Matrix`’s copy constructor. If `Matrix` is later revised to support moving, `operator+` will automatically benefit the next time it is compiled.

### Taking a universal reference

Consider a function template `reduceAndCopy` that takes a possibly unreduced `Fraction` object, reduces it, and then returns a copy of the reduced value.

```c++
template<typename T>         
Fraction reduceAndCopy(T&& frac)
{
  frac.reduce();
  return std::forward<T>(frac);
}
```

If the original object is an rvalue, its value should be moved into the return value (thus avoiding the expense of making a copy), but if the original is an lvalue, an actual copy must be created.

If the call to `std::forward` were omitted, `frac` would be unconditionally copied into `reduceAndCopy`’s return value.

## Copy elision

Compilers may elide the copying (or moving) of a local object in a function that returns by value if

1. the type of the local object is the same as that returned by the function and
2. the local object is what’s being returned.

```c++
Widget makeWidget()
{
  Widget w;
  // ...
  return w;
}
```

Both conditions are fulfilled here, and you can trust me when I tell you that for this code, every decent C++ compiler will employ the *return value optimization* (RVO) to avoid copying `w`. The copying version of `makeWidget` doesn’t, in fact, copy anything.

Applying `std::move` to a local object would be a bad idea. The Standard requires that when the RVO is permitted, either copy elision takes place or `std::move` is implicitly applied to local objects being returned. So in the “copying” version of `makeWidget`, compilers must either elide the copying of `w` or they must treat the function as if it were written like this:

```c++
Widget makeWidget()
{
  Widget w;
  // ...
  return std::move(w);        // treat w as rvalue
}
```

The situation is similar for by-value function parameters. Compilers must treat them as rvalues if they’re returned. As a result, if your source code looks like this,

```c++
Widget makeWidget(Widget w)     // by-value parameter
{
  // ...
  return w;
}
```

compilers must treat it as if it had been written this way:

```c++
Widget makeWidget(Widget w)     // by-value parameter
{
  // ...
  return std::move(w);        // treat w as rvalue
}
```
