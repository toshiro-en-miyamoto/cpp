enum struct Semantics {
  default_constructed,
  value_constructed,
  copy_constructed,
  copy_assigned,
  move_constructed,
  move_assigned
};
#include <utility>
struct Point {
  Semantics s {Semantics::default_constructed};
  int x {0};
  int y {0};
  int z {0};

  Point() {
    s = Semantics::default_constructed;
  }
  Point(int xx, int yy, int zz) : x {xx}, y {yy}, z {zz} {
    s = Semantics::value_constructed;
  }
  Point(const Point& p) : x {p.x}, y {p.y}, z {p.z} {
    s = Semantics::copy_constructed;
  }
  Point& operator=(const Point& p) {
    x = p.x; y = p.y; z = p.z;
    s = Semantics::copy_assigned;
    return *this;
  }
  Point(Point&& p) : x {p.x}, y {p.y}, z {p.z} {
    p.x = 0; p.y = 0; p.z = 0;
    s = Semantics::move_constructed;
  }
  Point& operator=(Point&& p) {
    std::swap(x, p.x);
    std::swap(y, p.y);
    std::swap(z, p.z);
    s = Semantics::move_assigned;
    return *this;
  }
};

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
TEST_CASE("constructors") {
  Point zero;
  CHECK(zero.s == Semantics::default_constructed);
  Point a {2, 3, 4};
  CHECK(a.s == Semantics::value_constructed);
  Point b = {5, 6, 7};
  CHECK(b.s == Semantics::value_constructed);
}
TEST_CASE("swapping by copying") {
  Point a {2, 3, 4};
  Point b {5, 6, 7};
  
  const Point tmp = a;
  CHECK(tmp.s == Semantics::copy_constructed);
  a = b;
  CHECK(a.s == Semantics::copy_assigned);
  b = tmp;
  CHECK(b.s == Semantics::copy_assigned);

  CHECK(a.x == 5);
  CHECK(a.y == 6);
  CHECK(a.z == 7);
  CHECK(b.x == 2);
  CHECK(b.y == 3);
  CHECK(b.z == 4);
}
TEST_CASE("swapping by moving") {
  Point a {2, 3, 4};
  Point b {5, 6, 7};
  
  Point tmp = std::move(a);
  CHECK(tmp.s == Semantics::move_constructed);
  a = std::move(b);
  CHECK(a.s == Semantics::move_assigned);
  b = std::move(tmp);
  CHECK(b.s == Semantics::move_assigned);

  CHECK(a.x == 5);
  CHECK(a.y == 6);
  CHECK(a.z == 7);
  CHECK(b.x == 2);
  CHECK(b.y == 3);
  CHECK(b.z == 4);
}
