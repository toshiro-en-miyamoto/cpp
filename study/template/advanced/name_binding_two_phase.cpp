/*
[1] The parse member function calls a function called `init` and the intention
is that it’s the base-class method `init` that is invoked here. However, the
compiler will issue an error, because it’s not able to find `init`. The reason
this happens is that `init` is a non-dependent name (as it does not depend on a
template parameter). Therefore, it must be known at the point of the definition
of the `parser` template. Although a `base_parser<T>::init` exists, the compiler
cannot assume it’s what we want to call because the primary template
`base_parser` can be later specialized and `init` can be defined as something else
(such as a type, or a variable, or another function, or it may be missing entirely).
Therefore, name lookup does not happen in the base class, only on its enclosing
scope, and there is no function called `init` in `parser`.

This problem can be fixed by making `init` a dependent name. This can be done
either by prefixing with `this->` or with `base_parser<T>::`. By turning `init`
into a dependent name, its name binding is moved from the point of template
definition to the point of template instantiation.

[2] Let’s consider that a specialization of `base_parser` for the `int` type is
made available after the definition of the `parser` class template.
[3] Furthermore, let’s consider the following use of the parser class template.
When you run this program, the following text will be printed to the console:
`specialized init`; `parse`; `init`; `parse`.

The reason for this behavior is that
- `p1` is an instance of `parser<int>` and there is a specialization of its base
class, `base_parser<int>` that implements the `init` function and prints
"specialized init" to the console.
- On the other hand, `p2` is an instance of `parser<double>`. Since a
specialization of `base_parser` for the `double` type is not available, the
`init` function from the primary template is being called and this only prints
`init` to the console.
*/
#include <iostream>

template <typename T>
struct base_parser
{
  void init()
  {
    std::cout << "init\n";
  }
};
template <typename T>
struct parser : base_parser<T>
{
  void parse()
  {
    // init()                             // [1]
    this->init();
    std::cout << "parse\n";
  }
};
template <>
struct base_parser<int>                   // [2]
{
  void init()
  {
    std::cout << "specialized init\n";
  }
};
int main()
{
  parser<int> p1;
  p1.parse();
  parser<double> p2;                      // [3]
  p2.parse();                             // [3]
}
