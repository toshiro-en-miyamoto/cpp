#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "ring_buffer.hpp"
#include <array>
#include <numeric>

SCENARIO("Ring buffer: use cases") {
  GIVEN("three-element buffer") {}
    my::Ring_buffer<int, 3> buffer{};
    WHEN("pushing 1, 2, 3, 4 and 5") {
      buffer.push(1);
      buffer.push(2);
      buffer.push(3);
      buffer.push(4);
      buffer.push(5);
      THEN("the sum of elements should be 12") {
        int sum = 0;
        while(auto element = buffer.pop()) {
          sum += element.value();
        }
        CHECK(sum == 12);
      }
      THEN("you get an array of elements") {
        std::array<int, 3> numbers{};
        for(std::size_t i = 0; auto element = buffer.pop(); i++) {
          numbers[i] = element.value();
        }
        const int result = std::accumulate(std::begin(numbers), std::end(numbers), 0);
        CHECK(result == 12);
      }
    }
}
SCENARIO("Ring buffer: normal cases") {
  GIVEN("empty buffer") {
    my::Ring_buffer<int, 3> buffer{};
    WHEN("trying to get the element") {
      auto element = buffer.pop();
      THEN("its value should be empty") {
        CHECK(!element);
      }
    }
  }
  GIVEN("one elements in the buffer") {
    my::Ring_buffer<int, 3> buffer{};
    buffer.push(1);
    WHEN("invoking pop() twice") {
      auto element = buffer.pop();
      THEN("you get the first recorded element") {
        CHECK(element);
        CHECK(element.value() == 1);
      }
      element = buffer.pop();
      THEN("you get the optional.empty") {
        CHECK(!element);
      }
    }
  }
  GIVEN("two elements in the buffer") {
    my::Ring_buffer<int, 3> buffer{};
    buffer.push(1);
    buffer.push(2);
    WHEN("invoking pop() three times") {
      auto element = buffer.pop();
      THEN("you get the first recorded element") {
        CHECK(element);
        CHECK(element.value() == 1);
      }
      element = buffer.pop();
      THEN("you get the second recorded element") {
        CHECK(element);
        CHECK(element.value() == 2);
      }
      element = buffer.pop();
      THEN("you get the optional.empty") {
        CHECK(!element);
      }
    }
  }
  GIVEN("N elements in the buffer") {
    my::Ring_buffer<int, 3> buffer{};
    buffer.push(1);
    buffer.push(2);
    buffer.push(3);
    WHEN("invoking pop() N times") {
      auto element = buffer.pop();
      THEN("you get the first recorded element") {
        CHECK(element);
        CHECK(element.value() == 1);
      }
      element = buffer.pop();
      THEN("you get the second recorded element") {
        CHECK(element);
        CHECK(element.value() == 2);
      }
      element = buffer.pop();
      THEN("you get the thrid recorded element") {
        CHECK(element);
        CHECK(element.value() == 3);
      }
      element = buffer.pop();
      THEN("you get the optional.empty") {
        CHECK(!element);
      }
    }
  }
  GIVEN("N+1 elements in the buffer") {
    my::Ring_buffer<int, 3> buffer{};
    buffer.push(1);
    buffer.push(2);
    buffer.push(3);
    buffer.push(4);
    WHEN("invoking pop() N+1 times") {
      auto element = buffer.pop();
      THEN("you get the second recorded element") {
        CHECK(element);
        CHECK(element.value() == 2);
      }
      element = buffer.pop();
      THEN("you get the thrid recorded element") {
        CHECK(element);
        CHECK(element.value() == 3);
      }
      element = buffer.pop();
      THEN("you get the fourth recorded element") {
        CHECK(element);
        CHECK(element.value() == 4);
      }
      element = buffer.pop();
      THEN("you get the optional.empty") {
        CHECK(!element);
      }
    }
  }
}
