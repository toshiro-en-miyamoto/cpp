// g++ -c -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion storage_class.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"

// a function at global scope
int local_counting() {
  // a static variable in a function
  // retains its state between calls
  static int counter = 0;
  return ++counter;
}

TEST_CASE("local counting") {
  CHECK(local_counting() == 1);
  CHECK(local_counting() == 2);
  CHECK(local_counting() == 3);
}

// a variable at global scope
int global_counter = 0;

TEST_CASE("global counting") {
  CHECK(++global_counter == 1);
  CHECK(++global_counter == 2);
  CHECK(++global_counter == 3);
}

// a struct at global scope
struct Counter {
  // a static data member, one copy of the
  // member is shared by all instances
  static int value;
};

// A static member must be defined at file scope.
int Counter::value = 0;

TEST_CASE("counter struct") {
  Counter c1;
  Counter c2;
  c1.value++;
  c2.value++;
  CHECK(c1.value == c2.value);
  CHECK(c1.value == 2);
}
