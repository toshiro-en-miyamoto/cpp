/*
At the point mark with [1] in the comments, we have the definition
of a class template called `handler`. This contains a single, public
method called `handle` that takes an argument of the `T` type and prints
its value to the console. Next, at point [2], we have the definition
of the class template called `parser`. This is similar to the previous
one, except for one key aspect: at point [3], it invokes a method
called `handle` on its argument. Because the type of the argument is
the template parameter `T`, it makes `handle` a dependent name.
Dependent names are looked up at the point of template instantiation,
so `handle` is not bound at this point. Continuing with the code, at
point [4], there is a template specialization for the `handler` class
template for the type `int`. As a specialization, this is a better match
for the dependent name. Therefore, when the template instantiation
happens at point [6], `handler<int>::handle` is the name that is bound
to the dependent name used at [3]. Running this program will print
"handler<int>: 42" to the console.
*/
#include <iostream>

template <typename T>
struct handler                            // [1]
{
  void handle(T value)
  {
    std::cout << "handler<T>: " << value << '\n';
  }
};
template <>
struct handler<double>
{
  void handle(double value)
  {
    std::cout << "handler<double>: " << value << '\n';
  }
};
template <typename T>
struct parser                             // [2]
{
  void parse(T arg)
  {
    arg.handle(42);                       // [3] dependent
  }
};
template <>
struct handler<int>                       // [4]
{
  void handle(int value)
  {
    std::cout << "handler<int>: " << value << '\n';
  }
};
int main()
{
  handler<int> h;                         // [5]
  parser<handler<int>> p;                 // [6]
  p.parse(h);
}
