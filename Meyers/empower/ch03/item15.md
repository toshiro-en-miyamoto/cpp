# Item 15: Use `constexpr` whenever possible

`constexpr` objects are `const`, and they have values that are known at compile time. They can be used as *integral constant expressions*, such as
- array sizes
- integral template arguments
- enumerator values
- alignment specifiers

```c++
constexpr auto arraySize = 10;
std::array<int, arraySize> data;
```

Note that `const` doesn’t offer the same guarantee as `constexpr`, because `const` objects need not be initialized with values known during compilation:

```c++
int sz;
...
const auto arraySize = sz;          // OK, const copy of sz
std::array<int, arraySize> data;    // error!
```

`constexpr` functions produce compile-time constants *when they are called with compile-time constants*. If the values of the arguments you pass to a `constexpr` function in such a context are known during compilation, the result will be computed during compilation.

```c++
constexpr int pow(int base, int exp) noexcept
{
  return (exp == 0 ? 1 : base * pow(base, exp - 1));
}

constexpr auto num = 5;
std::array<int, pow(3, num)> result;
```

When a `constexpr` function is called with one or more values that are not known during compilation, it acts like a normal function, computing its result at runtime.

```c++
// get these values at runtime
auto base = readFromDB("base");
auto exp = readFromDB("exponent");

auto baseToExp = pow(base, exp);
```

In C++14, the restrictions on `constexpr` functions are substantially looser, so the following implementation becomes possible:

```c++
constexpr int pow(int base, int exp) noexcept
{
  auto result = 1;
  for (int i = 0; i < exp; ++i) result *= base;

  return result;
}
```

Constructors and other member functions may be `constexpr`:

```c++
class Point {
public:
  constexpr Point(double xVal = 0, double yVal = 0) noexcept
    : x(xVal), y(yVal)
  {}

  constexpr double xValue() const noexcept { return x; }
  constexpr double yValue() const noexcept { return y; }

  constexpr void setX(double newX) noexcept { x = newX; }
  constexpr void setY(double newY) noexcept { y = newY; }

private:
  double x, y;
};
```

Here, the `Point` constructor can be declared `constexpr`, because if the arguments passed to it are known during compilation, the value of the data members of the constructed Point can also be known during compilation.

```c++
constexpr Point p1(9.4, 27.7);
```

Similarly, the getters `xValue` and `yValue` can be `constexpr`, because if they’re invoked on a `Point` object with a value known during compilation, the values of the data members `x` and `y` can be known during compilation. That makes it possible to write `constexpr` functions that call `Point`’s getters and to initialize `constexpr` objects with the results of such functions:

```c++
constexpr
Point midpoint(const Point& p1, const Point& p2) noexcept
{
  return {
    (p1.xValue() + p2.xValue()) / 2,
    (p1.yValue() + p2.yValue()) / 2
  };
}

constexpr auto mid = midpoint(p1, p2);
```

Even setters can be `constexpr`. That makes it possible to write functions like this:

```c++
constexpr Point reflection(const Point& p) noexcept
{
  Point result;

  result.setX(-p.xValue());
  result.setY(-p.yValue());

  return result;
}

constexpr Point p1(9.4, 27.7);
constexpr Point p2(28.8, 5.3);
constexpr auto mid = midpoint(p1, p2);

constexpr auto reflectedMid = reflection(mid);
```

It’s important to note that `constexpr` is part of an object’s or function’s interface. `constexpr` proclaims “I can be used in a context where C++ requires a constant expression.” If you declare an object or function `constexpr`, clients may use it in such contexts. If you later decide that your use of `constexpr` was a mistake and you remove it, you may cause arbitrarily large amounts of client code to stop compiling.
