struct Point {
  int x, y, z;
};
#include <cstddef>
#include <utility>
Point&& expand(Point&& p) {
  p.x++;
  p.y++;
  p.z++;
  // an rvalue reference cannot bind to an lvalue
  // Point&& cannot bind to Point&& p (p is an lvalue)
  // return p;
  return std::move(p);
}
int main() {
  Point a{1, 2, 3};
  // an rvalue reference cannot bind to an lvalue
  // Point&& p cannot bind to Point a (a is an lvalue)
  // auto b = expand(a);
  auto b = expand(std::move(a));
}
