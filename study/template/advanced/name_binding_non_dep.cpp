/*
The name `handle` is a non-dependent name because it does not depend
on any template parameter. Therefore, name lookup and binding are
performed at this point. `handle` must be a function known at point [3]
and the function defined at [2] is the only match. After the class
template definition, at point [5] we have the definition of an overload
for the function `handle`, which takes an integer as its argument. This
is a better match for `handle(42)`, but it comes after the name binding
has been performed, and therefore it will be ignored. In the `main`
function, at point [6], we have an instantiation of the `parser` class
template for the type `int`. Upon calling the `parse` function, the text
"processing a double: 42" will be printed to the console output.
*/
#include <iostream>

template <typename T>
struct handler{};                         // [1]

void handle(double value)                 // [2]
{
  std::cout << "processing a double: " << value << '\n';
}
template <typename T>
struct parser                             // [3]
{
  void parse()
  {
    handle(42);                           // [4] non-dependent
  }
};
void handle(int value)                    // [5]
{
  std::cout << "processing an int: " << value << '\n';
}
int main()
{
  parser<int> p;                          // [6]
  p.parse();
}
