# Doctest

Download the [latest version](https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h) which is just a single header and include it in your source files

## Getting started

[This](https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md) will compile to a complete executable which responds to command line arguments.

```c++
// g++ -std=c++20 factorial.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

int factorial1(int number) {
  return number <= 1
    ? number
    : factorial1(number - 1) * number;
}

TEST_CASE("testing the factorial function") {
  CHECK(factorial1(0) == 1);
  CHECK(factorial1(1) == 1);
  CHECK(factorial1(2) == 2);
  CHECK(factorial1(3) == 6);
  CHECK(factorial1(10) == 3628800);
}
```

If you just run it with no arguments it will execute all test cases.

```bash
~ $ g++ -std=c++20 factorial.cpp
~ $ ./a.out
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===========================================================
factorial.cpp:11:
TEST CASE:  testing the factorial function

factorial.cpp:12: ERROR: CHECK( factorial1(0) == 1 ) is NOT correct!
  values: CHECK( 0 == 1 )

===========================================================
[doctest] test cases: 1 | 0 passed | 1 failed | 0 skipped
[doctest] assertions: 5 | 4 passed | 1 failed |
[doctest] Status: FAILURE!
```

All we did was `#define` one identifier and `#include` one header and we got everything - even an implementation of `main()` that will respond to command line arguments. Usually it's a good idea to have a dedicated implementation file that just like that.

- Test cases with the `TEST_CASE` macro takes one argument - a free form *test name*. The test name doesn't have to be unique.
- Write our individual test assertions using the `CHECK()` macro.

You can run sets of tests by specifying a wildcard-ed test name or a tag expression.

## Subcases

For each `SUBCASE()` the `TEST_CASE()` is executed from the start - so as we enter each subcase we know that the size is 5 and the capacity is at least 5.

```c++
// g++ -std=c++20 subcase.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>

TEST_CASE("vectors can be sized and resized") {
  std::vector<int> v(5);

  REQUIRE(v.size() == 5);
  REQUIRE(v.capacity() >= 5);

  SUBCASE("adding to the vector increases its size") {
    v.push_back(1);

    CHECK(v.size() == 6);
    CHECK(v.capacity() >= 6);
  }
  
  SUBCASE("reserving increases just the capacity") {
    v.reserve(6);

    CHECK(v.size() == 5);
    CHECK(v.capacity() >= 6);
  }
}
```

We enforce those requirements with the `REQUIRE()` macros at the top level so we can be confident in them. If a `REQUIRE()` fails - execution of the test stops.

> If a `CHECK()` fails the test is marked as failed but the execution continues.
